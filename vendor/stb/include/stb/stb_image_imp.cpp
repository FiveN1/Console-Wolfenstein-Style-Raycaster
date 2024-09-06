#include"pch.h"
#include"stb_image_imp.h"

#define STB_IMAGE_IMPLEMENTATION   // use of stb functions once and for all
#include"stb_image.h"

unsigned char* stb::load(const char* filename, int* x, int* y, int* comp, int req_comp) {
	return stbi_load(filename, x, y, comp, req_comp);
}

void stb::clear(void* retval_from_image_load) {
	stbi_image_free(retval_from_image_load);
}

void stb::set_flip_vertically_on_load(bool shoud_flip) {
	stbi_set_flip_vertically_on_load(shoud_flip);
}