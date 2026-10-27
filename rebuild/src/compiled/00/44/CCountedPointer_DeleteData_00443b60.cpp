// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NPlayerGui::CDrawNPCName>) the per-type deleter: destroy the draw object and free it
// Leaf: ?DeleteData@?$CCountedPointer@VCDrawNPCName@NPlayerGui@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00443B60, 76 bytes, static `__fastcall` (data in ecx), plain `ret`.
// Identity (proven): stored as the count block's deleter by CCountedPointer<NPlayerGui::CDrawNPCName>::Reset 0x00444D00, which the
//   CPlayerGui constructor 0x0043B570 calls for its member at +0x248 (Ego_r.pdb CPlayerGui::PDrawNPCName, 8 bytes further on);
//   the +0x28 member destroyed first is Ego_r.pdb CDrawNPCName::Name (CWideString; destructor 0x0099B510); Ego_r.pdb names this deleter
//   ?DeleteData@?$CCountedPointer@V<T>@@@@SIXPAX@Z for the sibling NPlayerGui draws.
// Callees: the +0x28 member's destructor 0x0099B510, free 0x00BFEA14 (the vector block), operator delete 0x00BFE9BC; the
//   elements' virtual destructors (`push 0 ; call [eax]`).
// Shape (family M028, representative 0x00443840): `if (p) { p->T::~T(); operator delete(p); }` with the whole destructor
//   chain `__forceinline` (else VC7.1 keeps ~T or ??_G out of line) and the draw classes `__declspec(novtable)` (the
//   inlined destructors store no vtable in retail).
// Header debt: CDrawBase / the draw class are modelled here (no engine/ header carries their destructor chain); the
//   novtable is a model of the missing destructor vptr stores, not of the classes' constructors.
extern "C" void __cdecl free(void*);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                    // 0x00BFE9BC
class CEnginePrimitiveHandle {                            // 8 bytes: vptr, handle
public:
    virtual ~CEnginePrimitiveHandle();
    long Handle;
};
class CWideString {
public:
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } void deallocate(_Tp* p, unsigned int) { free(p); } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp> __forceinline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> __forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    __forceinline ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    ~_List_base();                                        // out of line (0x0043FCF0 for SDamageValue)
    void* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    __forceinline ~list() { }
};
}
class CPlayerGui;
class CPlayerManager;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;
namespace NPlayerGui {
class __declspec(novtable) CDrawBase {
public:
    virtual void VirtualSlot00();
    __forceinline ~CDrawBase() { }
    CPlayerGui* PlayerGui;                                // +0x04
    CPlayerManager* PlayerManager;                        // +0x08
    CWorld* World;                                        // +0x0c
    CMainGameComponent* Component;                        // +0x10
    CGraphicDataBank* Bank;                               // +0x14
    bool Drawing;                                         // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles; // +0x1c
};
class __declspec(novtable) CDrawNPCName : public CDrawBase {
public:
    __forceinline ~CDrawNPCName() { }
    CWideString Name;                                     // +0x28 (Ego_r.pdb)
};
}
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data);
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
};
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* data)
{
    T* p = (T*)data;
    if (p) {
        p->T::~T();
        operator delete(p);
    }
}
template void __fastcall CCountedPointer<NPlayerGui::CDrawNPCName>::DeleteData(void* data);