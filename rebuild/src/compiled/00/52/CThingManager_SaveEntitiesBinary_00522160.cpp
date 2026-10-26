// CThingManager::SaveEntitiesBinary(CMemoryDataOutputStream& stream, long version, const vector& things, vector& names) -- writes the things as binary: the set of their serialisation section names (copied into names), then per section its name and a length-prefixed section stream of (type name + SaveBinary) records for the things in it, closed by a 0
// Leaf: ?SaveEntitiesBinary@CThingManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00522160, 653 bytes (0x28D + int3 padding to 656), thiscall `ret 10h`, /O2 esp
//   frame (sub esp,44h). Identity: Ego_r.pdb ?SaveEntitiesBinary@CThingManager@@QAEXAAVCMemoryDataOutputStream@@JABV?$vector@
//   PAVCThing@@...@std@@AAV?$vector@VCCharString@@...@Z (the catalog's "SavePersistentEntitiesInRegions" is descriptive).
//   Callers: CThingManager::RuntimeSaveGameHero 0x00523B40 and the level/global savers.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the set header node, 0x14 bytes), set<CCharString> insert_unique
//   0x004AC380 / _Rb_tree::_M_erase 0x004ABA70, vector<CCharString>::erase(first, last) 0x0043336A (clear),
//   vector<CCharString>::_M_range_insert(pos, first, last, const bidirectional_iterator_tag&) 0x0052B6A0,
//   CDataOutputStream::WriteULONG 0x00993EA0 / WriteNullTerminatedString 0x00995AE0, CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, Compare 0x00411570 (__fastcall, the inline CCharString equality),
//   CMemoryDataOutputStream() 0x00996AF0 / ~ 0x00996B30 / GetLength 0x00994B40 / PeekData 0x00995E70 / Write 0x00996CC0,
//   CThing::GetTypeName 0x004C7E10 / SaveBinary 0x004C84B0; the outer stream's virtual Write (slot 4, +0x10).
// Layout: retail CThingManager::SerialiseVersionNumber +0xd8 (my landed constructor 0x00523540); CThing::
//   SerialisationSectionName +0x58; CMemoryDataOutputStream 0x18 bytes; CCharString data block {chars, length}.
// Shape: `if (things.empty()) return;`, the version stored before the set is built and again after the section count;
//   STLport set<CCharString> written out (header node allocate + empty-initialise, clear + deallocate) and walked with the
//   inline static `_Rb_global::_M_increment`; names.clear() then the range insert, whose bidirectional tag temp sits in the
//   live `things` slot (the range insert has a same-TU stand-in that never reads the tag); each section is a by-value
//   copy (in the dead `version` slot). The inner record write is spelled as WriteULONG + the direct (devirtualised) Write
//   on the local section stream; the outer is CDataOutputStream::Write(const CMemoryDataOutputStream&) inline (virtual).
//   Compare has a same-TU stand-in body that only reads: retail VC7.1 knew it, so the section's string block stays cached
//   in esi across the no-match path (with only a declaration VC reloads it after the call and reallocates every register).
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

class CCharStringData { public:
    const char*   m_pData;                                                             // +0x00
    unsigned long m_length;                                                            // +0x04
};
__declspec(noinline) long __fastcall Compare(const char* a, const char* b)
{                                                   // 0x00411570 (same-TU stand-in: reads only)
    for (;;) {
        char c = *a;
        if (c == 0 && *b == 0)
            return 0;
        if (c < *b)
            return -1;
        if (c > *b)
            return 1;
        ++a;
        ++b;
    }
}
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                                             // 0x0099EC30
    ~CCharString();                                                                    // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

void RangeInsertNames(void* names, void* position, void* first, void* last);           // stand-in helper

namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                                                // +0x00
    _Base_ptr _M_parent;                                                               // +0x04
    _Base_ptr _M_left;                                                                 // +0x08
    _Base_ptr _M_right;                                                                // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                                             // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr _M_node);
};

template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}

template <class _Value> struct _Rb_tree_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    _Rb_tree_iterator() : _M_node(0) { }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef allocator<_Rb_tree_node<_Value> > _Node_alloc;
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, _Node_alloc> _M_header;   // +0x00
    _Rb_tree_base(const _Alloc&) : _M_header(_Node_alloc(), (_Rb_tree_node<_Value>*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;                                                        // +0x04
    _Compare     _M_key_compare;                                                       // +0x08
    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    iterator begin() const { return iterator(_M_leftmost()); }
    iterator end() const { return iterator(this->_M_header._M_data); }
    unsigned int size() const { return _M_node_count; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    pair<iterator, bool> insert_unique(const _Value& __v);                             // 0x004AC380
private:
    void _M_erase(_Link_type __x);                                                     // 0x004ABA70
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(_Compare(), _Alloc()) { }
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
    unsigned int size() const { return _M_t.size(); }
    pair<iterator, bool> insert(const _Key& __x) { return _M_t.insert_unique(__x); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    iterator erase(iterator __first, iterator __last);                                 // 0x0043336A
    void clear() { erase(begin(), end()); }
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_range_insert(__pos, __first, __last, bidirectional_iterator_tag());
    }
    template <class _ForwardIter>
    __declspec(noinline) void _M_range_insert(iterator __pos, _ForwardIter __first, _ForwardIter __last,
                                              const bidirectional_iterator_tag&)
    {                                               // 0x0052B6A0 (same-TU stand-in: never reads the tag)
        RangeInsertNames(this, __pos, __first._M_node, __last._M_node);
    }
};

}

class CMemoryDataOutputStream;
class CDataOutputStream {
public:
    virtual ~CDataOutputStream();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long length);                       // slot 4 (+0x10)
    void WriteULONG(unsigned long value);                                              // 0x00993EA0
    void WriteNullTerminatedString(const CCharString& text);                           // 0x00995AE0
    void Write(const CMemoryDataOutputStream& stream);
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    unsigned char _pad_0x04[0x04];
    unsigned char Data[0x0c];                                                          // +0x08 CArray<char>
    unsigned long CurrentPosition;                                                     // +0x14
    CMemoryDataOutputStream();                                                         // 0x00996AF0
    ~CMemoryDataOutputStream();                                                        // 0x00996B30
    using CDataOutputStream::Write;
    virtual void Write(const void* data, unsigned long length);                       // 0x00996CC0
    unsigned long GetLength() const;                                                   // 0x00994B40
    const void* PeekData() const;                                                      // 0x00995E70
};
inline void CDataOutputStream::Write(const CMemoryDataOutputStream& stream)
{
    WriteULONG(stream.GetLength());
    Write(stream.PeekData(), stream.GetLength());
}

class CThing {
public:
    unsigned char _pad_0x00[0x58];
    CCharString   SerialisationSectionName;                                            // +0x58
    const CCharString& GetTypeName() const;                                            // 0x004C7E10
    void SaveBinary(CDataOutputStream& stream);                                        // 0x004C84B0
};

class CThingManager {
public:
    unsigned char _pad_0x00[0xd8];
    long          SerialiseVersionNumber;                                              // +0xd8
    void SaveEntitiesBinary(CMemoryDataOutputStream& stream, long version, const std::vector<CThing*>& things,
                            std::vector<CCharString>& names);
};

void CThingManager::SaveEntitiesBinary(CMemoryDataOutputStream& stream, long version,
                                       const std::vector<CThing*>& things, std::vector<CCharString>& names)
{
    if (things.empty())
        return;
    SerialiseVersionNumber = version;
    std::set<CCharString> sections;
    for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it)
        sections.insert((*it)->SerialisationSectionName);
    names.clear();
    names.insert(names.end(), sections.begin(), sections.end());
    stream.WriteULONG(sections.size());
    SerialiseVersionNumber = version;
    for (std::set<CCharString>::iterator s = sections.begin(); s != sections.end(); ++s) {
        CCharString section(*s);
        stream.WriteNullTerminatedString(section);
        CMemoryDataOutputStream sectionStream;
        for (std::vector<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            if (thing->SerialisationSectionName == section) {
                CMemoryDataOutputStream thingStream;
                thingStream.WriteNullTerminatedString(thing->GetTypeName());
                thing->SaveBinary(thingStream);
                sectionStream.WriteULONG(thingStream.GetLength());
                sectionStream.Write(thingStream.PeekData(), thingStream.GetLength());
            }
        }
        sectionStream.WriteULONG(0);
        stream.Write(sectionStream);
    }
}