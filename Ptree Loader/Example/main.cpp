// =============================================================================
// Ptree Loader
// =============================================================================
// Usage example for Ptree Loader
//
// @author Dwoggurd (2024)
// =============================================================================

#include <print>
#include <PtreeLoader.h>
#include <filesystem>

// -----------------------------------------------------------------------------
template<ptree_loader::PtreeFileFormat T>
void TestPtreeLoader( const std::filesystem::path &fsPath )
{
    boost::property_tree::ptree pt;
    ptree_loader::PtreeLoader<T> ptLoader( pt );

    ptLoader.Load( fsPath );

    std::print( "{}", ptLoader.DumpDiag() );
    std::print( "{}", ptLoader.DumpPtree() );
}

// -----------------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    std::print(
        "--------------------------------\n"
        "|         Ptree Loader         |\n"
        "--------------------------------\n" );

    if ( argc < 2 )
    {
        std::print( "Usage: PtreeLoader <filename>" );
        return 0;
    }

    const std::filesystem::path fsPath{ argv[1] };
    const std::string ext{ fsPath.extension().string() };

    // Test PtreeLoader for various file formats (xml/json/info)
    if ( ext == ".xml" )
    {
        std::print( "Assuming XML format...\n" );
        TestPtreeLoader<ptree_loader::PtreeFileFormat::xml>( fsPath );
    }
    else if ( ext == ".json" )
    {
        std::print( "Assuming JSON format...\n" );
        TestPtreeLoader<ptree_loader::PtreeFileFormat::json>( fsPath );
    }
    else if ( ext == ".info" )
    {
        std::print( "Assuming INFO format...\n" );
        TestPtreeLoader<ptree_loader::PtreeFileFormat::info>( fsPath );
    }

    return 0;
}

// -----------------------------------------------------------------------------
