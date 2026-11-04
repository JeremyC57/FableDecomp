// std::vector::~vector() -- (vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry >) out-of-line destructor: destroy every 0x30-byte element (its implicit destructor frees the inner Themes array at +0x8 when it has a block), then free the vector's own block
// Leaf: ??1?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@V?$allocator@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6AF0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry >::~vector -- probable; evidence: only unreferenced code reaches it (the jmp thunks 0x006B6D10 / 0x006B6D80 / 0x006B6F00 and the deleting destructor 0x006B6E70, i.e. the CArray-level destructors of the same vector) in the environment-theme TU (CIDrawEnvironmentParams rows 0x006B6010..0x006B6B30); its 0x30-byte element with a block at +8 is exactly Ego_r.pdb NEnvironmentInternal::CEnvironmentThemePriorityStackEntry with the 12-byte retail CArray (the double at +0x20 rounds it to 48) and FableWin.pdb names ??1?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@...@QAE@XZ.
// Label: the manifest's "Vector_DestroyRange" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `~vector() { _Destroy(_M_start, _M_finish); }` with the inline `~_Vector_base() { if (_M_start) deallocate(_M_start, ...); }`; the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {                   // 48 bytes (Ego_r.pdb 56 with its 16-byte CArray)
public:
    struct CThemeWeight {                                      // 12 bytes (Ego_r.pdb)
    public:
        long ThemeID;
        float Weight;
        bool LockWeight;
    };
    float  BaseWeight;                                        // +0x00
    bool   BaseWeightLocked;                                  // +0x04
    CArray<CThemeWeight> Themes;                              // +0x08
    long   ThemeTransitionTargetID;                           // +0x14
    float  ThemeTransitionInitialWeight;                      // +0x18
    bool   ThemeTransitionActive;                             // +0x1c
    double ThemeTransitionStartTime;                          // +0x20
    float  ThemeTransitionDuration;                           // +0x28
};
}
template class std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry >;