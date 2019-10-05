#include "C:\Users\sasho\Downloads\libsquish-code-r110-trunk\squish.h"

using namespace squish;

extern "C" void DecompressImage(u8 * rgba, int width, int height, void const* blocks, int flags)
{
	squish::DecompressImage(rgba, width, height, blocks, flags);
}