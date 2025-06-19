#include "hex_collapse.h"

#include <states.h>
#include <renderer.h>
// Ultimaille
#include <ultimaille/all.h>

#if defined(__linux__) || defined(__APPLE__)

extern "C" {
	HexCollapseComponent* allocator(IApp &app) {
		return new HexCollapseComponent(app);
	}

	void deleter(HexCollapseComponent *ptr) {
		delete ptr;
	}
}

#endif

#ifdef WIN32
extern "C"
{
	__declspec (dllexport) HexCollapseComponent* allocator(IApp &app) {
		return new HexCollapseComponent(app);
	}

	__declspec (dllexport) void deleter(HexCollapseComponent *ptr) {
		delete ptr;
	}

}
#endif