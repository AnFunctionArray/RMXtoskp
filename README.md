# RMXtoskp

## Prerequisites
 Basically you would need to download this [package](https://sourceforge.net/p/libsquish/code/HEAD/tarball) and change the include path in cppsucks.cpp. This library is used for decompression of dxt compressed textures from the Zone file.

Additionally you would need to download sketchup SDK and include it in the include path plus mklink in the headers sub-directory from SketchupAPI to slapi. Or you can just replace slapi in rmxtoskp.c with SketchupAPI.

## Oddities

You would notices I've included the header files inside several functions - that is because the structures declarations are relative. They need variable declarations to be present. [Based pointers](https://docs.microsoft.com/en-us/cpp/cpp/based-pointers-cpp?view=vs-2019) for more info. Also I've prefixed them with [__ptr32](https://docs.microsoft.com/en-us/cpp/cpp/ptr32-ptr64?view=vs-2019) since the file format uses 32 bit words for offsets. Also you would notice some superfluous parentheses around declarators and comma operators in place of semicolon - the later is actually needed when being the inner statement of a loop or conditional statement but other than that this is a trade-mark. I've also used old style function definitions.


## Note to msvc users 
*actually the only users I would get since I'm ussing based pointers*

You would get some initial errors because of an union inside a structure in some sketchup header - you would need to extract it in file scope to make things work.


## Usage

###### arguments
1. rmx file
2. zone file
3. input skp file (if specified apply patches to game files, otherwise write to new_model.skp extracted data)

first 

Considerting you are reading this I've probably provided you with a sample. If not you would need nakamichi680 ungmx tool to appy to unlz-ed level from the cache directory - it's in the root of the game folder. You would obviosuly need to start up the game with the level you want to extract first.
