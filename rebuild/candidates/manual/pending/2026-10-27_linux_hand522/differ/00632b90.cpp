// GFSerialiseMapBinaryIn<CCharString, unsigned long, std::less<unsigned long> >(CMemoryDataInputStream& stream, std::map<unsigned long, CCharString>& data) -- clear the map, read the entry count, then read each key (CPersistTraits<unsigned long>) and value (a null-terminated string) and insert it
// Grade: DIFFER(253v253) -- stack slots only: the empty key traits gets its own 4-byte slot (+0x14) so the value traits and the count temp sit 4 bytes lower; retail aliases the empty CPersistTraits<unsigned long> onto the value traits' address. Tried: swapped declaration order (same), key traits as a temporary (268 bytes, worse).
// Leaf: ??$GFSerialiseMapBinaryIn@VCCharString@@KU?$less@K@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@KVCCharString@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@3@@std@@@Z
// Flags: /O2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632B90, 253 bytes, __fastcall (ecx = stream, edx = map), `and esp, -8` frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb carry this exact template spelling; its only caller CPersistContext::Transfer
//   for map<unsigned long, CCharString> at 0x00632AF0 (load mode 2, PLoadStream at context +0x24) passes CUIDef::ActionMap
//   (+0x1A8) from 0x006320C2.
// Callees: CCharString() 0x0099E4B0 (the value traits' DefaultVal), _Rb_tree<...>::clear() 0x00630FE0,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline ReadSLONG), CPersistTraits<unsigned long>::TransferBinaryIn
//   (CMemoryDataInputStream&, unsigned long&) 0x0040F8A0 (empty key traits, `this` shares the value traits' address),
//   virtual CMemoryDataInputStream::ReadNullTerminatedString (slot 6, +0x18; inline CPersistTraits<CCharString>
//   ::TransferBinaryIn), CCharString::operator= 0x0099EFB0, CCharString(const CCharString&) 0x0099EC30,
//   _Rb_tree<...>::insert_unique(const value_type&) 0x00632C90, ~CCharString 0x0099EAE0.
// Label correction: catalogued with template arguments <pair<long,long>, CCharString, less<CCharString> >.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

class CDataInputStream {                                    // retail 0x18 bytes
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual CCharString ReadNullTerminatedString();         // slot 6 (+0x18)
    __forceinline long ReadSLONG()
    {
        long value;
        Read(&value, sizeof(value));
        return value;
    }
    __forceinline void Read(void* data, long size)
    {
        __int64 newPos = (__int64)(unsigned long)(StreamPos + size);
        if (newPos < 0 || newPos > 0x7fffffff)
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(data, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)data, size);
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char* data, long size);   // 0x00993CA0
public:
    unsigned long        StreamPos;                         // +0x04
    long                 StreamSize;                        // +0x08
    const unsigned char* CurrentSourcePtr;                  // +0x0C
    unsigned long        SourceChunkStreamPos;              // +0x10
    long                 SourceChunkBytesRemaining;         // +0x14
};
class CMemoryDataInputStream : public CDataInputStream {    // retail 0x1C bytes
public:
    virtual CCharString ReadNullTerminatedString();
    unsigned char* StreamSource;                            // +0x18
};

template <class T> class CPersistTraits;
template <> class CPersistTraits<unsigned long> {
public:
    void TransferBinaryIn(CMemoryDataInputStream& stream, unsigned long& value);   // 0x0040F8A0
};
template <> class CPersistTraits<CCharString> {
public:
    CCharString DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, CCharString& value) { value = stream.ReadNullTerminatedString(); }
};

namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Nonconst_traits { };
template <class _Pair> struct _Select1st { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    pair<iterator, bool> insert_unique(const _Value& __x);  // 0x00632C90
    void clear();                                           // 0x00630FE0
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    _Compare            _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
    void clear() { _M_t.clear(); }
    _Rep_type _M_t;
};
}   // namespace std

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& data)
{
    CPersistTraits<V> valueTraits;
    CPersistTraits<K> keyTraits;
    data.clear();
    long count = stream.ReadSLONG();
    for (; count > 0; --count) {
        std::pair<K, V> entry;
        keyTraits.TransferBinaryIn(stream, entry.first);
        valueTraits.TransferBinaryIn(stream, entry.second);
        data.insert(std::map<K, V, C>::value_type(entry.first, entry.second));
    }
}

template void __fastcall GFSerialiseMapBinaryIn<CCharString, unsigned long, std::less<unsigned long> >(
    CMemoryDataInputStream& stream, std::map<unsigned long, CCharString, std::less<unsigned long> >& data);
