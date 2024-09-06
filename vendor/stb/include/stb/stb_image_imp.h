#ifndef STB_IMP_H
#define STB_IMP_H

// STB IMAGE IMPLEMENTATION

namespace stb {
	// load stb image
	unsigned char* load(const char* filename, int* x, int* y, int* comp, int req_comp);
	// Clear image
	void clear(void* retval_from_image_load);
	// Flip image vertically on load
	void set_flip_vertically_on_load(bool b);
}
#endif
