// CEnginePrimitiveRenderer2D::AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager) -- append the counted manager pointer to PrimitiveManagers (STLport push_back), then register the raw manager with the global primitive type info
// Leaf: ?AddPrimitiveManager@CEnginePrimitiveRenderer2D@@AAEXABV?$CCountedPointer@VCEnginePrimitiveManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4ABB0, 94 bytes, thiscall, `ret 4` through the tail jump (the manager's Data
//   is written over the dead argument slot and RegisterPrimitiveManager pops it).
// Identity (proven): Ego_r.pdb (0x009EB610) and FableWin.pdb (0x02D9D980) ?AddPrimitiveManager@CEnginePrimitiveRenderer2D@@
//   AAEXABV?$CCountedPointer@VCEnginePrimitiveManager@@@@@Z, in Ego_r between ~CEnginePrimitiveRenderer2D and the ctor; all 5
//   callers (0x00B4AD7D..0x00B4AE5F) sit in the next function 0x00B4AC10, the renderer ctor (stores vtable 0x012A2308 and
//   zeroes PrimitiveManagers +0x0C and the five manager CCountedPointers +0x18..+0x44). The twin of the landed 3D
//   CEnginePrimitiveRenderer::AddPrimitiveManager 0x00B33AE0 (same callees). The catalog label is right.
// Callees: std::vector<CCountedPointer<CEnginePrimitiveManager> >::_M_insert_overflow 0x00B38BB0 (bound spelling, the
//   __false_type overload), CEnginePrimitiveTypeInfo::RegisterPrimitiveManager 0x00B8FAD0 on the global 0x01436E84 (the
//   established landed spelling g_pEnginePrimitiveTypeInfo of 0x00B33AE0; no PDB data symbol).
// Layout: Ego_r.pdb PrimitiveManagers (CArray<CCountedPointer<CEnginePrimitiveManager> >) at +0x0C after CEngineComponent
//   (vptr, GlobalPointerAddress, RenderEnabled, KeepAlive); engine/CEnginePrimitiveRenderer2D.h / CEngineComponent.h flatten
//   the vptr (header debt), so a local model keeps the member this row touches.
// Shape: the 0x00B33AE0 source verbatim with the 2D class (STLport 4.6 inline push_back, engine/CCountedPointer.h copy ctor).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
inline void operator delete(void*, void*) { }

#include "engine/CCountedPointer.h"

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                // 0x00B38BB0
};

} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CEnginePrimitiveManager;
class CEnginePrimitiveTypeInfo {
public:
    void RegisterPrimitiveManager(CEnginePrimitiveManager* manager);             // 0x00B8FAD0
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                     // 0x01436E84

class CEngineComponent {                                                         // vptr, GlobalPointerAddress +4, RenderEnabled +8, KeepAlive +9
public:
    virtual ~CEngineComponent();
    void** GlobalPointerAddress;                                                 // +0x04
    bool RenderEnabled;                                                          // +0x08
    unsigned char KeepAlive;                                                     // +0x09
};

class CEnginePrimitiveRenderer2D : public CEngineComponent {
public:
    CArray<CCountedPointer<CEnginePrimitiveManager> > PrimitiveManagers;         // +0x0c
private:
    void AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager);
};

void CEnginePrimitiveRenderer2D::AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager)
{
    PrimitiveManagers.push_back(manager);
    g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(manager.Get());
}