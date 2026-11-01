// NBankFileManager::CRetailBank::~CRetailBank() -- compiler-generated destructor: free the file path, clear and free the contained-bank map (STLport map<CCharString, CContainedBank>), release the threaded and disk file counted pointers, then ~CBaseClassNonCopyable
// Leaf: ??1CRetailBank@NBankFileManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AC7C0, length 161 bytes, tail jump to ~CBaseClassNonCopyable.
// Identity (proven): the only caller is the scalar deleting destructor 0x009AC7A0 = slot 0 of vtable 0x0129B010 (RTTI
//   .?AVCRetailBank@NBankFileManager@@; the landed constructor 0x009AC4D0 stores the same vtable); Ego_r.pdb / FableWin.pdb
//   ??1CRetailBank@NBankFileManager@@UAE@XZ. Label correction: the catalog's "C3DMeshStripifier::~C3DMeshStripifier" is propagated.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (FilePath), std::_Rb_tree<CCharString, pair<const CCharString,
//   NBankFileManager::CContainedBank>, ...>::_M_erase @ 0x009AA780 (its erase frees 0x28-byte nodes destroying a CCharString
//   at value +0: the 0x18-byte pair of this map; landed under CUnproven placeholders -> NAME_FIXES.tsv), free @ 0x00BFEA14 (the
//   tree header node), operator delete @ 0x00BFE9BC (each count block), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300.
// Layout (Ego_r.pdb CRetailBank re-laid at STLport sizes, as the landed ctor 0x009AC4D0): CBaseClassNonCopyable vptr +0x00,
//   DiskFile CCountedPointer<CDiskFileWin32> +0x04, ThreadedFile CCountedPointer<CThreadedFile> +0x0C, UseDMA +0x14,
//   ContainedBanks map<CCharString, CContainedBank> +0x18 (12 bytes), FilePath CCharString +0x24.
// Shape: no vtable store = a compiler-generated destructor; members in reverse order. The real STLport 4.6 tree text
//   (`~_Rb_tree() { clear(); }`, `~_Rb_tree_base()` deallocating the header) gives the shared-zero (`xor ebx,ebx`) clear and the
//   null-tested free; the two counted pointers are engine/CCountedPointer.h. The implicit ??1 is emitted the way retail's TU emits
//   it: the real out-of-line constructor (retail 0x009AC4D0, PDB ??0CRetailBank@NBankFileManager@@QAE@XZ) is defined here
//   (its map construction is not modelled; only the destructor is graded). rebuild/include/fable_retail_banks.h is a flat,
//   non-polymorphic layout: local model (header debt).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                // +0x00
    _Base_ptr   _M_parent;                               // +0x04
    _Base_ptr   _M_left;                                 // +0x08
    _Base_ptr   _M_right;                                // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                             // +0x04
    _Compare  _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                       // out of line: 0x009AA780 for this map
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
} // namespace std

#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)
#include "engine/CCountedPointer.h"                    // retail model (8 bytes: Data, Info)

class CCharStringData;
class CCharString {
public:
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;
};
class CDiskFileWin32;
class CThreadedFile;

namespace NBankFileManager {
class CContainedBank {
public:
    unsigned long DataVersion;                          // +0x00
    unsigned long EntryCount;                           // +0x04
    unsigned long InfoOffset;                           // +0x08
    unsigned long InfoSize;                             // +0x0C
    unsigned long Alignment;                            // +0x10
};
class CRetailBank : public CBaseClassNonCopyable {      // vtable 0x0129B010
public:
    CRetailBank();
    CCountedPointer<CDiskFileWin32>                 DiskFile;           // +0x04
    CCountedPointer<CThreadedFile>                  ThreadedFile;       // +0x0C
    bool                                            UseDMA;             // +0x14
    std::map<CCharString, CContainedBank>           ContainedBanks;     // +0x18
    CCharString                                     FilePath;           // +0x24
};

CRetailBank::CRetailBank()
    : UseDMA(false)
{
}
} // namespace NBankFileManager