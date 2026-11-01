// CEngineSceneGrid::~CEngineSceneGrid() -- scene grid destructor: restore the CEngineSceneGrid vtable, delete every grid cell in GridCells (by index, re-reading the size each pass), then the GridCells array's own destructor (free the buffer) and the CBaseClassNonCopyable base
// Leaf: ??1CEngineSceneGrid@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B93E40, 98 bytes, thiscall, tail `jmp ??1CBaseClassNonCopyable@@MAE@XZ` (0x0099A300).
// Identity (proven): stores vtable 0x012A3E58 first (RTTI .?AVCEngineSceneGrid@@ : CBaseClassNonCopyable : CBaseClass); sole
//   caller is that vtable's slot-0 scalar deleting destructor 0x00B97A60 (call at 0x00B97A63); Ego_r.pdb ??1CEngineSceneGrid@@UAE@XZ
//   and ??1CEngineSceneGridCell@@QAE@XZ (the per-cell callee 0x00B93660, non-virtual). The catalog label
//   CLocalDetailGeneratorTheme::~CLocalDetailGeneratorTheme is propagated from a masked twin.
// Callees: ~CEngineSceneGridCell 0x00B93660, operator delete 0x00BFE9BC, free 0x00BFEA14 (STLport allocator), ~CBaseClassNonCopyable 0x0099A300.
// Layout: Ego_r CEngineSceneGrid (no engine header): Level +0x04, LevelUp +0x08, LevelDown +0x0C, Size +0x10, GridCells +0x14
//   (CArray<CEngineSceneGridCell*>, Fable's CArray over the 12-byte STLport 4.6 vector: _M_start +0x14, _M_finish +0x18).
// Shape: `for (unsigned long i = 0; i < GridCells.size(); ++i) delete GridCells[i];` -- unsigned size (`jbe` / `jb`), the
//   size re-read every pass, operator[] re-loads _M_start; `delete` of the non-virtual cell is the null test + ??1 + operator
//   delete; then the STLport ~vector (pointer elements: no destroy loop) is `if (_M_start) free`.
#include "engine/CBaseClassNonCopyable.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };   // STLport: pointers are trivial
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Tp& reference;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }   // pointer elements: the trivial __destroy_aux
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector
class CEngineSceneGridCell {
public:
    ~CEngineSceneGridCell();                               // 0x00B93660 (Ego_r ??1CEngineSceneGridCell@@QAE@XZ)
};
class CEngineSceneGrid : public CBaseClassNonCopyable {
public:
    unsigned long                 Level;                   // +0x04
    CEngineSceneGrid*             LevelUp;                 // +0x08
    CEngineSceneGrid*             LevelDown;               // +0x0c
    unsigned long                 Size;                    // +0x10
    CArray<CEngineSceneGridCell*> GridCells;               // +0x14
    virtual ~CEngineSceneGrid();
};
CEngineSceneGrid::~CEngineSceneGrid()
{
    for (unsigned long i = 0; i < GridCells.size(); ++i)
        delete GridCells[i];
}