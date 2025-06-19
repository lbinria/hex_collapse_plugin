#include "hex_collapse.h"

#include <states.h>
#include <renderer.h>
// Ultimaille
#include <ultimaille/all.h>

#if defined(__linux__) || defined(__APPLE__)

extern "C" {
	HexCollapseComponent* allocator(Hexahedra &hex, Renderer &renderer, InputState &st) {
		return new HexCollapseComponent(hex, renderer, st);
	}

	void deleter(HexCollapseComponent *ptr) {
		delete ptr;
	}
}

#endif

#ifdef WIN32
extern "C"
{
	// __declspec (dllexport) Tatooine *allocator()
	// {
	// 	return new Tatooine();
	// }

	// __declspec (dllexport) void deleter(Tatooine *ptr)
	// {
	// 	delete ptr;
	// }
}
#endif