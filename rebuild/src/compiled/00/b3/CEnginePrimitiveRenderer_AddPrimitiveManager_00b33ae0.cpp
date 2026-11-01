// CEnginePrimitiveRenderer::AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager) -- append the counted manager pointer to PrimitiveManagers (STLport push_back: copy-construct in place with the reference-count bump, or _M_insert_overflow when full), then register the raw manager with the global primitive type info
// Leaf: ?AddPrimitiveManager@CEnginePrimitiveRenderer@@AAEXABV?$CCountedPointer@VCEnginePrimitiveManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B33AE0, 103 bytes, __fastcall `ret 4` (through the tail jump: the manager's Data is
//   written over the dead argument slot and RegisterPrimitiveManager pops it).
// Identity: Ego_r.pdb ?AddPrimitiveManager@CEnginePrimitiveRenderer@@AAEXABV?$CCountedPointer@VCEnginePrimitiveManager@@@@@Z
//   at donor 0x009D44D0, immediately before ??0CEnginePrimitiveRenderer@@QAE@XZ (0x009D4550); retail has the same pair: this
//   row, then the 21 call sites all inside the next function 0x00B33B50 (the renderer ctor: each passes a copy of one of
//   the CCountedPointer manager members +0x280.. and releases it after). PrimitiveManagers = CArray<CCountedPointer<
//   CEnginePrimitiveManager> > at Ego_r +0x274 (the 12-byte STLport vector here; struct_layouts_egor.tsv).
// Callees: std::vector<CCountedPointer<CEnginePrimitiveManager> >::_M_insert_overflow 0x00B38BB0 (out of line, the
//   __false_type overload: CCountedPointer has a user copy ctor); CEnginePrimitiveTypeInfo::RegisterPrimitiveManager(
//   CEnginePrimitiveManager*) 0x00B8FAD0 (FableWin.pdb ?RegisterPrimitiveManager@CEnginePrimitiveTypeInfo@@QAEXPAVCEngine
//   PrimitiveManager@@@Z) on the global 0x01436E84 (the established landed spelling g_pEnginePrimitiveTypeInfo; no PDB data
//   symbol).
// Label: manifest "_global::CEnginePrimitiveTypeInfo_Manager_List_Insert" was descriptive.
// Shape: STLport 4.6 inline push_back (placement new with its null test, the copy ctor's `if (Info) ++Info->RefCount`),
//   CArray<T> = the engine's empty wrapper over std::vector<T>.
// Layout: engine/CEnginePrimitiveRenderer.h is flattened (PrimitiveManagers as bytes, no member functions), so `this` is a
//   local model that keeps only the member this row touches (Header debt in NOTES.md).
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

class CEnginePrimitiveRenderer {
public:
    unsigned char _pad_0x000[0x274];                                             // base + members before (not touched)
    CArray<CCountedPointer<CEnginePrimitiveManager> > PrimitiveManagers;         // +0x274
private:
    void AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager);
};

void CEnginePrimitiveRenderer::AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager)
{
    PrimitiveManagers.push_back(manager);
    g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(manager.Get());
}