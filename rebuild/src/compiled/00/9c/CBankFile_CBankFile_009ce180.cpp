// CBankFile::~CBankFile() -- bank-file destructor: restore the vtable, close the bank (Close devirtualised and expanded: CloseEntries), then destroy the checksum cache, the strings, arrays, symbol trees, counted file pointer and packed arrays in reverse order and ~CBaseClassNonCopyable
// Leaf: ??1CBankFile@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE180, 473 bytes, thiscall, tail jump to ~CBaseClassNonCopyable 0x0099A300.
// Identity (proven): the body stores vtable 0x0129B54C first, whose RTTI COL names .?AVCBankFile@@ (rtti-vtables.tsv: CBankFile
//   0x0129B54C slot 0 = 0x009D47A0); the first caller 0x009D47A3 is that slot-0 scalar deleting destructor (landed
//   CBankFile_scalardeletingdestructor_009d47a0), the others (0x009D668F, 0x00A620FE, 0x00A6288D, 0x00A636B9, 0x00A63B68) are
//   the derived bank destructors chaining to their base. Ego_r.pdb + FableWin.pdb ??1CBankFile@@UAE@XZ. The member walk is the
//   retail CBankFile layout of engine/CBankFile.h (Symbols +0x08 .. ChecksumCache +0xF8). The catalog label
//   "NHeroInformationScreens::CBase::~CBase" is propagated and wrong.
// Callees: CBankFile::CloseEntries 0x009CD850 (private; reached through the same-TU Close(), whose out-of-line copy 0x009CE360
//   right after this body is the bare `jmp` to it), ~CChecksumCacheFile 0x00A60CE0, ~CCharString 0x0099EAE0, free 0x00BFEA14
//   (STLport allocator), _Rb_tree<CCharString, pair<const CCharString, unsigned long>, ...>::_M_erase 0x00579435 (map and
//   multimap share it), ~CWideString 0x0099B510, CCountedPointer<CDiskFileWin32> release inline (operator delete 0x00BFE9BC),
//   ~CPackedUIntArray 0x00A62AC0, ~CBaseClassNonCopyable 0x0099A300.
// Layout: engine/CBankFile.h (retail offsets, a flat layout with a `__vftable` member and byte-array containers), restated
//   locally with the real member types so their destructors run (NOTES.md "Header debt").
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "engine/CBaseClassNonCopyable.h"                   // ~CBaseClassNonCopyable 0x0099A300
#include "engine/CCountedPointer.h"
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    bool operator<(const CCharString&) const;
    CCharStringData* PStringData;
};
class CWideStringData;
class CWideString { public:
    ~CWideString();                                         // 0x0099B510
    CWideStringData* PStringData;
};
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_pair.h, stl/_function_base.h, stl/_tree.h, stl/_vector.h, stl/_map.h
// (the members this row needs; the malloc-backed allocator of this build: deallocate = free 0x00BFEA14)
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <> struct __type_traits<unsigned long> { typedef __true_type has_trivial_destructor; };
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);
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
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class multimap {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray over the STLport vector
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap { public:   // 0x10 bytes
    std::vector<std::pair<K, V> > Data;                     // +0x00
    C    Compare;                                           // +0x0C
    bool Dirty;                                             // +0x0D
};
class CPackedUIntArray { public:                            // 0x10 bytes
    ~CPackedUIntArray();                                    // 0x00A62AC0
    unsigned long* Data;
    unsigned long  Count;
    unsigned long  BitsPerEntry;
    unsigned long  AllocatedSize;
};
class CChecksumCacheFile { public:                          // 0x18 bytes
    ~CChecksumCacheFile();                                  // 0x00A60CE0
    unsigned long Body[6];
};
class CDiskFileWin32;
class CBankFileEntryUpdateData;
struct CProgressCallback;

class CBankFile : public CBaseClassNonCopyable { public:    // vtable 0x0129B54C, 0x110 bytes
    class CRuntimeData { public: unsigned long Data[3]; };
    virtual ~CBankFile();                                                           // slot 0
    virtual bool OpenReadOnly(const CCharString& bankHandle, unsigned long flags);  // slot 1
    virtual void OpenReadWrite(const CCharString& bankHandle, unsigned long flags, unsigned long alignment);   // slot 2
    virtual bool OpenFileReadOnly(const CWideString& filename, unsigned long flags);                          // slot 3
    virtual void OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment); // slot 4
    virtual void Close();                                                           // slot 5
    unsigned long Size;                                     // +0x04
    CArray<CCharString> Symbols;                            // +0x08
    CArray<unsigned long> Checksums;                        // +0x14
    CArray<CRuntimeData> RuntimeData;                       // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;           // +0x2C
    CPackedUIntArray PackedDataOffset;                      // +0x38
    CPackedUIntArray PackedDataSize;                        // +0x48
    CPackedUIntArray PackedDataType;                        // +0x58
    CPackedUIntArray PackedValid;                           // +0x68
    unsigned long OpenFlags;                                // +0x78
    CCountedPointer<CDiskFileWin32> BankFile;               // +0x7C
    CWideString   Filename;                                 // +0x84
    CCharString   BankHandle;                               // +0x88
    bool          RetailMode;                               // +0x8C
    bool          ReadOnly;                                 // +0x8D
    bool          WrittenToFile;                            // +0x8E
    bool          Solid;                                    // +0x8F
    unsigned long ChangeCount;                              // +0x90
    bool          FileValid;                                // +0x94
    bool          UpdatingFlag;                             // +0x95
    unsigned long Alignment;                                // +0x98
    unsigned long BankFormatVersion;                        // +0x9C
    unsigned long HeaderSize;                               // +0xA0
    unsigned long FileInformationSize;                      // +0xA4
    unsigned long FileSize;                                 // +0xA8
    std::map<CCharString, unsigned long> SymbolIndices;     // +0xAC
    std::map<CCharString, unsigned long> OldSymbolIndices;  // +0xB8
    std::multimap<CCharString, unsigned long> FilenameIndices;   // +0xC4
    CVectorMap<unsigned long, unsigned long> SymbolCRCIndices;   // +0xD0
    CArray<unsigned long> BlockOrder;                       // +0xE0
    CProgressCallback* ProgressCallback;                    // +0xEC
    CCharString   HeaderEnumerationType;                    // +0xF0
    bool          WarningFlag;                              // +0xF4
    CChecksumCacheFile ChecksumCache;                       // +0xF8
private:
    void CloseEntries();                                    // 0x009CD850
};

CBankFile::~CBankFile()
{
    Close();
}

void CBankFile::Close()                                     // 0x009CE360 (not graded: the out-of-line virtual)
{
    CloseEntries();
}
FABLE_STATIC_ASSERT(sizeof(CBankFile) == 0x110);