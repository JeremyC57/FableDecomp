// GFSerialiseMapBinaryIn(CMemoryDataInputStream&, std::map<long,CDefString>&) -- [instantiation <CDefString,long,std::less<long> >] binary persist read of a long-to-def-string map: clear it, read the count, then per entry read the key and the def string and insert the pair
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DBA50, 228 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`.
// Callees: _Rb_tree::_M_erase @ 0x004DA27F, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CPersistTraits<long>::
//   TransferBinaryIn @ 0x0040FE60, CDefString::TransferBinaryIn(CDataInputStream&) @ 0x0044FC00, map insert @ 0x006DB450.
// Identity: FableWin.pdb ??$GFSerialiseMapBinaryIn@VCDefString@@JU?$less@J@std@@@@...; the callers are CHeroExperienceDef::Transfer
//   (0x006D90C3 ..) for its five map<long,CDefString> members (SoundIntensityThresholds, ComboMultiplierIncrease, ..).
//   The catalog's list<COpinionTransientOffset> label is wrong.
// Layout: STLport map = 12 bytes {header node*, node count, empty less<>}; header parent +4, left +8, right +0xC; the
//   8-byte pairs on the frame make VC7.1 align it (`and esp,-8`).
// Shape: inline clear(); the count through an inlined CPersistTraits<long>::TransferBinaryIn (fast path into ebx); the
//   loop value-initialises the pair (key 0, CDefString's default TablePos -1), reads the key through an empty long-traits
//   temporary homed on the pair's first word and the def string through its own member TransferBinaryIn.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void*                m_pVtable;                         // +0x00
    unsigned long        position;                          // +0x04
    long                 sourceLength;                      // +0x08
    const unsigned char* chunkBase;                         // +0x0C
    long                 chunkPosition;                     // +0x10
    long                 chunkLength;                       // +0x14
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CDefString {                                          // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                          // +0x00
    CDefString() : TablePos(-1) { }
    void TransferBinaryIn(CMemoryDataInputStream& stream);  // 0x0044FC00 (CDataInputStream&)
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    void _M_erase(_Rb_tree_node_base* node);                // 0x004DA27F
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x006DB450
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStream& stream, T* out) const;
};

__forceinline void ReadCount(CMemoryDataInputStream& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

// Same-TU stand-in (auto_inline off, noinline) for the out-of-line long traits 0x0040FE60: it reads only the stream and
// writes only through `out`, which lets VC7.1 home the empty traits temporary on the pair's first word.
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream, long* out) const
{
    *out = (long)stream.position;
}
#pragma auto_inline(on)

template <> struct CPersistTraits<CDefString> {
    void TransferBinaryIn(CMemoryDataInputStream& stream, CDefString* out) const { out->TransferBinaryIn(stream); }
};

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& out)
{
    out.clear();
    long count;
    ReadCount(stream, &count);
    for (; count > 0; --count) {
        std::pair<K, V> value;
        CPersistTraits<K>().TransferBinaryIn(stream, &value.first);
        CPersistTraits<V>().TransferBinaryIn(stream, &value.second);
        out.insert(typename std::map<K, V, C>::value_type(value.first, value.second));
    }
}

template void __fastcall GFSerialiseMapBinaryIn<CDefString, long, std::less<long> >(CMemoryDataInputStream& stream, std::map<long, CDefString, std::less<long> >& out);