// =============================================================================
// Ptree Loader
// =============================================================================
// An utility class that enables loading Boost.PropertyTree with support for "include" directives.
// Boost.PropertyTree supports four file formats for loading values : XML / JSON / INI / INFO
// However, only one of those formats (INFO) supports "include" directive functionality
// natively and it is also limited to absolute paths.
// This class enhances support for "include" functionality
// and allows to use it with several file formats (XML/JSON/INFO).
//
// This is achieved by reserving a special key, "IncludeFile", which is interpreted as "include" directive.
// Ptree files are loaded recursively from locations pointed by "IncludeFile" keys.
// Filepaths can be absolute or relative (to the parent file).
//
// INI format doesn't allow duplicate keys so it is not supported for "include" functionality.
//
// This class also provides utility methods for printing ptree content and diagnostic.
//
// @author Dwoggurd (2024)
// =============================================================================
#ifndef PtreeLoader_H
#define PtreeLoader_H

// -----------------------------------------------------------------------------
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <filesystem>
#include <exception>

// -----------------------------------------------------------------------------
namespace ptree_loader
{
namespace bpt = boost::property_tree;
namespace fs  = std::filesystem;

// -----------------------------------------------------------------------------
enum class PtreeFileFormat
{
    xml,
    json,
    info
};

// -----------------------------------------------------------------------------
// PtreeLoader declaration
// -----------------------------------------------------------------------------
template<PtreeFileFormat T>
class PtreeLoader
{
public:
    /// Constructs PtreeLoader
    /// @param root ptree to load into
    explicit PtreeLoader( bpt::ptree& root ) : root( root ) {}
    PtreeLoader( const PtreeLoader& )             = delete;
    PtreeLoader& operator=( const PtreeLoader& )  = delete;
    PtreeLoader( PtreeLoader&& )                  = delete;
    PtreeLoader& operator=( PtreeLoader&& )       = delete;
    ~PtreeLoader()                                = default;

    /// Load ptree from file
    /// @param fsPath Absolute or relative file path
    void Load( const fs::path& fsPath );

    /// Dump diagnostic
    std::string DumpDiag() const;

    /// Dump ptree content
    std::string DumpPtree() const;

private:
    void Load( const fs::path& fsPath, const fs::path& fsParentPath );
    void Reader( const std::string& fname, bpt::ptree& pt );
    void Writer( std::ostream& stream, const bpt::ptree& pt ) const;

private:
    /// Special key that represents include file
    static constexpr const char* includeKey{ "IncludeFile" };

    /// Recursive include loop detector
    static constexpr const int depthLimit{ 20 };

    bpt::ptree&        root;
    std::stringstream  diagnostic;
    int                depth;
};

// -----------------------------------------------------------------------------
// PtreeLoader definition
// -----------------------------------------------------------------------------
template<PtreeFileFormat T>
void PtreeLoader<T>::Load( const fs::path& fsPath )
{
    depth = 0;
    Load( fsPath, fsPath.is_relative() ? fs::current_path() : "" );
}

// -----------------------------------------------------------------------------
template<PtreeFileFormat T>
void PtreeLoader<T>::Load( const fs::path& fsPath, const fs::path& fsParentPath )
{
    if ( ++depth > depthLimit )
    {
        diagnostic << "Recursive include loop depected. Exiting..." << '\n';
        return;
    }

    const fs::path fsEffectivePath{
        fs::weakly_canonical( fsPath.is_absolute() ? fsPath : fsParentPath / fsPath ) };

    if ( !fs::exists( fsEffectivePath ) )
    {
        diagnostic << "Path not found: " << fsEffectivePath.string() << '\n';
        return;
    }

    diagnostic << "Loading: " << fsEffectivePath.string() << '\n';

    // Temporary ptree to load current config
    bpt::ptree subtree;

    try
    {
        Reader( fsEffectivePath.string(), subtree );
    }
    catch ( const std::exception& e )
    {
        diagnostic << "Error: " << e.what() << '\n';
        return;
    }

    // Merge children from subtree into root tree at ptPath
    for ( const auto& kv : subtree )
    {
        // Add duplicate keys, don't replace.
        root.add_child( kv.first, kv.second );

        if ( kv.first == includeKey )
        {
            // Handle IncludeFile
            Load( kv.second.data(), fsEffectivePath.parent_path() );
        }
    }
}

// -----------------------------------------------------------------------------
#define PTREE_PARSER( FF )                                                                        \
                                                                                                  \
template<>                                                                                        \
void PtreeLoader<PtreeFileFormat::FF>::Reader( const std::string& fname, bpt::ptree& pt )         \
{                                                                                                 \
    bpt::FF ## _parser::read_ ## FF( fname, pt );                                                 \
}                                                                                                 \
                                                                                                  \
template<>                                                                                        \
void PtreeLoader<PtreeFileFormat::FF>::Writer( std::ostream& stream, const bpt::ptree& pt ) const \
{                                                                                                 \
    bpt::FF ## _parser::write_ ## FF( stream, pt );                                               \
}

// -----------------------------------------------------------------------------

PTREE_PARSER( xml )
PTREE_PARSER( json )
PTREE_PARSER( info )

#undef PTREE_PARSER

// -----------------------------------------------------------------------------
template<PtreeFileFormat T>
std::string PtreeLoader<T>::DumpDiag() const
{
    std::string delim( 80, '=' );

    return delim + '\n' + diagnostic.str() + delim + '\n';
}

// -----------------------------------------------------------------------------
template<PtreeFileFormat T>
std::string PtreeLoader<T>::DumpPtree() const
{
    std::stringstream ss;
    std::string delim( 80, '=' );

    ss << delim << '\n';
    Writer( ss, root );
    ss << '\n' << delim << '\n';
    return ss.str();
}

// -----------------------------------------------------------------------------
}; // namespace ptree_loader
// -----------------------------------------------------------------------------
#endif // PtreeLoader
