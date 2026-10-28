// C3DMeshStats::PackNames(CArray<char>& packed_names, const set<CCharString>& helper_point_names, const set<CCharString>& dummy_object_names) -- pack the helper-point and dummy-object names into one byte block: a 16-bit offset of the dummy names, the non-empty helper names (null terminated) and an empty terminator, then the dummy names and their terminator
// Leaf: ?PackNames@C3DMeshStats@@AAEXAAV?$CArray@D@@ABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2EEF0, 502 bytes, thiscall `ret 0Ch` (`this` unused).
// Identity (proven): Ego_r.pdb ?PackNames@C3DMeshStats@@AAEXAAV?$CArray@D@@ABV?$set@VCCharString@@...@std@@1@Z (private,
//   void, CArray<char>& + two const set<CCharString>&) -- exactly this frame; its caller 0x00A2F740 is the C3DMeshStats
//   file-data initialiser (Ego_r InitialiseFromFileData), and the block is what the landed UnpackNames 0x00A2ED10 reads
//   back (PackedNames starts with the ushort offset of the dummy-object names, helper names follow the word).
// Callees: CMemoryDataOutputStream ctor 0x00996AF0 / SetPosition 0x00994B10 / GetPosition 0x009945B0 / GetLength
//   0x00994B40 / PeekData 0x00995E70 / dtor 0x00996B30 (direct calls: the stream is a local of known type),
//   CDataOutputStream::WriteUWORD 0x00993E80, CDataOutputStream::WriteNullTerminatedString 0x00995AE0,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, memmove 0x00BFEAE6 (inline erase),
//   vector<char>::_M_fill_insert 0x004110D0 (inline resize's insert); memcpy intrinsic (rep movsd / movsb).
// Label: catalog "DoublyLinkedList_MergeToString" is descriptive and wrong (two red-black tree walks into a byte block).
// Shape: both set walks inline STLport's static _Rb_global<bool>::_M_increment on a copy of the node and re-read end()
//   through the const reference; an empty name is skipped (GetLength() != 0); each terminator is the implicit
//   CCharString("") conversion (the temporary's address is re-formed for the call); resize(n) is STLport's inline
//   erase / _M_fill_insert with a `char()` temporary; GetData() is CArray's `size() > 0 ? &[0] : (T*)this`. `this` is
//   not touched, so C3DMeshStats is only declared here.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);  // 0x00BFEAE6
#pragma intrinsic(memcpy)

class CCharStringData {                                    // Ego_r.pdb: CBasicString<char> Data first
public:
    char*         PData;                                   // +0x00
    unsigned long StringLength;                            // +0x04
};

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
    long GetLength() const { return PStringData == 0 ? 0 : (long)PStringData->StringLength; }
};

namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
                                     ((const char*)__last - (const char*)__first);
}

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = (_Tp*)__copy_trivial(__last, _M_finish, __first);
        _M_finish = __i;
        return __first;
    }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);                          // 0x004110D0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

struct _Rb_tree_node_base {
    bool                _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;                         // +0x04
    _Rb_tree_node_base* _M_left;                           // +0x08
    _Rb_tree_node_base* _M_right;                          // +0x0c
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x)
    {
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        return __x;
    }
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    typedef _Rb_tree_iterator<_Key> const_iterator;
    _Rb_tree_node_base* _M_header;                         // +0x00
    unsigned long       _M_node_count;                     // +0x04
    _Compare            _M_key_compare;                    // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    bool empty() const { return _M_node_count == 0; }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    T* GetData() { return this->size() > 0 ? &(*this)[0] : (T*)this; }
};

class CDataOutputStream {
public:
    virtual ~CDataOutputStream();
    CCharString NewLine;                                   // +0x04
    void WriteUWORD(unsigned short value);                 // 0x00993E80
    void WriteNullTerminatedString(const CCharString& text);   // 0x00995AE0
};
class CMemoryDataOutputStream : public CDataOutputStream { // retail 0x18 bytes
public:
    CMemoryDataOutputStream();                             // 0x00996AF0
    virtual ~CMemoryDataOutputStream();                    // 0x00996B30
    virtual void SetPosition(unsigned long position);      // 0x00994B10
    virtual unsigned long GetPosition() const;             // 0x009945B0
    unsigned long GetLength() const;                       // 0x00994B40
    const void* PeekData() const;                          // 0x00995E70
    CArray<char>  Data;                                    // +0x08
    unsigned long CurrentPosition;                         // +0x14
};

class C3DMeshStats {
    void PackNames(CArray<char>& packed_names, const std::set<CCharString>& helper_point_names,
                   const std::set<CCharString>& dummy_object_names);
};

void C3DMeshStats::PackNames(CArray<char>& packed_names, const std::set<CCharString>& helper_point_names,
                             const std::set<CCharString>& dummy_object_names)
{
    if (helper_point_names.empty() && dummy_object_names.empty())
        return;
    CMemoryDataOutputStream stream;
    stream.WriteUWORD(0);
    for (std::set<CCharString>::const_iterator it = helper_point_names.begin(); it != helper_point_names.end(); ++it)
        if (it->GetLength() != 0)
            stream.WriteNullTerminatedString(*it);
    stream.WriteNullTerminatedString("");
    unsigned long dummy_offset = stream.GetPosition();
    for (std::set<CCharString>::const_iterator it2 = dummy_object_names.begin(); it2 != dummy_object_names.end(); ++it2)
        if (it2->GetLength() != 0)
            stream.WriteNullTerminatedString(*it2);
    stream.WriteNullTerminatedString("");
    stream.SetPosition(0);
    stream.WriteUWORD((unsigned short)dummy_offset);
    unsigned long length = stream.GetLength();
    packed_names.resize(length);
    memcpy(packed_names.GetData(), stream.PeekData(), length);
}