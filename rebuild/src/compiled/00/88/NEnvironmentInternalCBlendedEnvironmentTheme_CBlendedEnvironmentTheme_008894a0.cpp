// NEnvironmentInternal::CBlendedEnvironmentTheme::CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme& other) -- compiler-generated copy constructor: time of day, the 19 colours and 30 scalers as blocks, then each of the 3 texture sets' two CArrays deep-copied (allocate x.size() words, memmove)
// Leaf: ??0CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@ABV01@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008894A0, 301 bytes incl. int3 padding (299 of code), thiscall, `ret 4`,
//   returns this.
// Identity: the catalog's "LargeStruct_CopyWithNestedArrays" is descriptive. This is the out-of-line copy constructor
//   the landed __uninitialized_copy 0x00889880 calls for every 0x1F4-byte NEnvironmentInternal::CBlendedEnvironmentTheme
//   (vector<CBlendedEnvironmentTheme> copy / regrowth); its layout is the one of the landed default constructor
//   0x008864A0 and assignment 0x006B5DF0: TimeOfDay +0, Colours CRGBFloatColour[19] +4, Scalers float[30] +0x134,
//   TextureSets CIDrawEnvironmentTextureSet[3] +0x1AC (Textures CArray<long> +0, Blends CArray<float> +0xC; the
//   retail CArray is a 12-byte STLport vector).
// Callees: malloc 0x00BFEA0E (STLport malloc allocator), memmove 0x00BFEAE6 (__copy_trivial).
// Shape: the implicit memberwise copy: one dword, `rep movsd` x 0x4C and x 0x1E for the POD arrays, then the 3-element
//   array of texture sets as a counted loop, each CArray copy inlining STLport `vector(const vector& x) :
//   _Vector_base(x.size(), x.get_allocator())` (three zero stores, allocate, start/finish/end stores) and
//   `_M_finish = __uninitialized_copy(x.begin(), x.end(), _M_start, __true_type())` = __copy_trivial (memmove + n).
extern "C" void* __cdecl malloc(unsigned int size);                                 // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);    // 0x00BFEAE6

#include "engine/CArray.h"


class CRGBFloatColour { public:
    float R, G, B, A;
};

struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                     // +0x00
    CArray<float> Blends;                       // +0x0C
};

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    float                       TimeOfDay;      // +0x000
    CRGBFloatColour             Colours[19];    // +0x004
    float                       Scalers[30];    // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3]; // +0x1AC
    CBlendedEnvironmentTheme();                 // 0x008864A0
};
}

// Emits the implicit copy constructor out of line (as the retail vector helpers call it).
void CopyConstructTheme(void* where, const NEnvironmentInternal::CBlendedEnvironmentTheme& theme)
{
    new (where) NEnvironmentInternal::CBlendedEnvironmentTheme(theme);
}