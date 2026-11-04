// _$E2() -- `dynamic atexit destructor for 'CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D::empty_scene'': the function-local static's inlined STLport ~vector (trivially destroyed elements: just free the storage)
// Leaf: _$E2
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122AD60, 17 bytes, in `.text$yd`: `mov eax,[0x014377F0] ; test ; je ; push eax ; call free ; pop ecx ; ret`.
// Identity (proven): the static at 0x014377F0 is initialised and registers this thunk (0x00B4AC6C) inside the CEnginePrimitiveRenderer2D constructor 0x00B4AC10, which passes it to CEnginePrimitiveManager2DAnimatedMeshes::CEnginePrimitiveManager2DAnimatedMeshes 0x00BA1AC0 (Ego_r.pdb ??0CEnginePrimitiveManager2DAnimatedMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z, so CArray<CEngineScene*>); Ego_r.pdb: `CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D'::`2'::`dynamic atexit destructor for 'empty_scene'' (scope 2 = the function's top level, ?1 in the mangling).
// Static: `static CArray<CEngineScene*> empty_scene;` in CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D (0x00B4AC10).
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate behind the _Vector_base null test).
// Shape (family M004, representative 0x01229370): VC7.1 emits a function-local static's atexit destructor as the TU's `_$E2`
//   in .text$yd; ~vector's _Destroy loop over elements with trivial destructors vanishes, leaving ~_Vector_base's
//   `if (_M_start) free(_M_start)`.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
#include "engine/CArray.h"
class CEngineScene;
class CEnginePrimitiveRenderer2D {
public:
    CEnginePrimitiveRenderer2D();                        // 0x00B4AC10
    long SceneCount;
};
// Only the local static's atexit destructor is graded; this body just owns the static (the retail one does more).
CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D()
{
    static CArray<CEngineScene*> empty_scene;            // 0x014377F0
    SceneCount = empty_scene.size();
}