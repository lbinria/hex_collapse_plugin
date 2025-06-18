#include "hex_collapse.h"

#if defined(__linux__) || defined(__APPLE__)

extern "C" {
	HexCollapseComponent* allocator() {
		return new HexCollapseComponent;
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