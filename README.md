# Ptree Loader
An utility class that enables loading [Boost.PropertyTree](https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree.html) with support for "include" directives.  
This is a header-only class.

## What it does
__Boost.PropertyTree__ supports four file formats for loading values: XML/JSON/INI/INFO  
([How to Populate a Property Tree](https://www.boost.org/doc/libs/1_85_0/doc/html/property_tree/parsers.html)).
However, only one of those formats (INFO) supports "include" directive functionality natively and it is also limited to absolute paths.
This class enhances support for "include" functionality and allows to use it with several file formats (XML/JSON/INFO).

This is achieved by reserving a special key, "__IncludeFile__", which is interpreted as "include" directive.

Ptree files are loaded recursively from locations pointed by __IncludeFile__ keys.
Filepaths can be absolute or relative (to the parent file).

INI format doesn't allow duplicate keys so it is not supported for "include" functionality.

This class also provides utility methods for printing ptree content and diagnostic.


## Used technogolies
- C++23
- Boost 1.70.0 (and higher)
- CMake
- GCC14 & MSVC2022

## Example
It is recommended to use INFO format for Boost.PropertyTree files.

Advantages:
- simple
- native support for ptree structure (because it was created for that library)
- allows comments
- round-trips

Create PtreeLoader object for desired format with ptree to load into, then call Load() method.
```cpp
#include "PtreeLoader.h"

// Create ptree
boost::property_tree::tree pt;
// Instantiate PtreeLoader for INFO format
ptree_loader::PtreeLoader<ptree_loader::PtreeFileFormat::info> loader(pt);

// Call Load() with path to the root file
// absolute or relative to the current working directory
loader.Load("file1.info");

// Print some diagnostic
std::print("{}", loader.DumpDiag());
std::print("{}", loader.DumpPtree());
```

file1.info
```
IncludeFile file2.info
Data
{
    field1 100
    field2 200
}
```
file2.info
```
weight1 26
weight2 45
```
Resulting ptree state:
```
IncludeFile file2.info
weight1 26
weight2 45
Data
{
    field1 100
    field2 200
}
```

More examples: [Example](Ptree%20Loader/Example)

##
Dwoggurd (2024)
