// std::__destroy_aux(CBlendedEnvironmentTheme* first, CBlendedEnvironmentTheme* last, const __false_type&) -- destroy a range of blended environment themes: each frees the six vector blocks of its three texture sets
// Leaf: ?__destroy_aux@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B67A0, 84 bytes, `ret 4`.
// Callers: the landed vector<CBlendedEnvironmentTheme> range destroy 0x006B6FF0 (modelled there as an opaque 0x1F4-byte
//   "SThemeBlock").
// Callees: free @ 0x00BFEA14 (the STLport malloc allocator's deallocate, inlined from ~_Vector_base).
// Types (Ego_r.pdb, shrunk to retail 12-byte STLport vectors): CBlendedEnvironmentTheme = TimeOfDay float +0, Colours
//   CRGBFloatColour[19] +4, Scalers float[30] +0x134, TextureSets CIDrawEnvironmentTextureSet[3] +0x1AC (Textures
//   CArray<long> +0, Blends CArray<float> +0xC) = 0x1F4 bytes (PDB 524 with 16-byte Dinkumware vectors).
// Shape: STLport 4.x `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (`__fastcall`: first in ecx, last
//   in edx, the empty tag reference on the stack, `ret 4`) with the implicit destructor inlined: the compiler's
//   array-destructor loop walks TextureSets backwards from +0x1F4, freeing Blends before Textures in each set. Same
//   source as the CThemeChannel twin 0x006B6610; only the 4-byte TimeOfDay head moves the offsets.
// Label correction: the catalog `_Copy_backward<C3DVertex2*,C3DVertex2*>` is propagated.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

struct __false_type { __false_type() { } };

template <class T>
class CArray { public:                                         // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CArray()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};

class CRGBFloatColour { public:
    float R, G, B, A;
};

struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                             // +0x00
    CArray<float> Blends;                               // +0x0C
};

struct CBlendedEnvironmentTheme {
    float                       TimeOfDay;              // +0x000
    CRGBFloatColour             Colours[19];            // +0x004
    float                       Scalers[30];            // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3];         // +0x1AC
};

namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template void __fastcall __destroy_aux<CBlendedEnvironmentTheme*>(CBlendedEnvironmentTheme*, CBlendedEnvironmentTheme*, const __false_type&);
}