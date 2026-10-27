// CCountedPointer<C3DGroup2>::DeleteData(void* data) -- the counted pointer's deleter: destroy and free the C3DGroup2
// Leaf: ?DeleteData@?$CCountedPointer@VC3DGroup2@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A982D0, 34 bytes, `__fastcall` (the pointer in ecx), plain `ret`.
// Identity (proven): its address is stored as the count block's deleter by CCountedPointer<C3DGroup2>'s constructor 0x00A984F0 and inline in 0x00A91640 right after `new C3DGroup2(chunk)` (0x00AC4150, bound ??0C3DGroup2@@QAE@ABVC3DMeshFileGroupChunk@@@Z).
// Callees: free 0x00BFEA14 (the vector's storage), operator delete 0x00BFE9BC.
// Shape (family M079, representative 0x004ADD90): `delete p;` (null-tested) with the implicit C3DGroup2 destructor inline --
//   only the POD vector at +0x4 has work: STLport ~_Vector_base `if (_M_start) deallocate` (free).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }
};
}
class C3DVertexBlend2 {                                   // Ego_r.pdb, 8 bytes
public:
    unsigned long VertexIndex;
    float BlendValue;
};
class C3DGroup2 {                                         // Ego_r.pdb, no vtable
public:
    long BoneIndex;                                       // +0x0
    std::vector<C3DVertexBlend2> VertexBlends;            // +0x4
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data);
    T* Data;
    CCPPointerInfo* Info;
};
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* data)
{
    delete (T*)data;
}
template void __fastcall CCountedPointer<C3DGroup2>::DeleteData(void* data);