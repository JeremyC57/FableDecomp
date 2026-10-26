// GFSerialiseMapBinaryIn(CMemoryDataInputStream&, std::map<EOpinionReactionType,float>&) -- [instantiation <float,EOpinionReactionType,std::less<EOpinionReactionType> >] binary persist read of a map: clear it, read the entry count, then per entry read the key and the value through their persist traits and insert the pair
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067C200, 230 bytes (+10 int3 padding), `__fastcall` (ecx = stream, edx = map), plain `ret`.
// Callees: 0x0067A840 (STLport _Rb_tree::_M_erase of the root), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//   CPersistTraits<EOpinionReactionType>::TransferBinaryIn @ 0x0045E011, CPersistTraits<float>::TransferBinaryIn @ 0x0040EFB0,
//   _Rb_tree::insert_unique(const value_type&) @ 0x0067BBB0 (pair<iterator,bool> returned through a hidden pointer).
// Identity: FableWin.pdb ??$GFSerialiseMapBinaryIn@MW4EOpinionReactionType@@U?$less@W4EOpinionReactionType@@@std@@@@YIX...;
//   caller 0x006797DF (COpinionReactionManagerDef::Transfer, member PreReactionDelay). The catalog's
//   `CPersistTraits<std::list<COpinionTransientOffset> >::TransferBinaryIn` label is propagated and wrong.
// Layout: STLport map = 12 bytes {header node*, node count, empty less<> padded}; the header node holds parent +4, left +8,
//   right +0xC. The 8-byte pairs on the frame make VC7.1 align it (`and esp,-8`).
// Shape: the inline clear() (erase the root, relink the header, zero the count); the count goes through an inlined
//   CPersistTraits<long>::TransferBinaryIn (fast path straight into the loop register); the loop builds a value-initialised
//   pair each pass (two zero stores after the first argument pushes) and calls the out-of-line element traits on empty
//   traits temporaries that VC7.1 homes on the pair's own first word (`lea ecx,[esp+10h]`), which needs the stand-ins below.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CMemoryDataInputStream { public:
    void*                m_pVtable;                         // +0x00
    unsigned long        position;                          // +0x04
    long                 sourceLength;                      // +0x08
    const unsigned char* chunkBase;                         // +0x0C
    long                 chunkPosition;                     // +0x10
    long                 chunkLength;                       // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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

enum EOpinionReactionType { OPINION_REACTION_TYPE_NULL = 0, OPINION_REACTION_TYPE_FORCE_DWORD = 0x7fffffff };
enum EOpinionAttitudeType { OPINION_ATTITUDE_TYPE_NULL = 0, OPINION_ATTITUDE_TYPE_FORCE_DWORD = 0x7fffffff };
enum EOpinionTargetingConditionType { OPINION_TARGETING_CONDITION_NULL = 0, OPINION_TARGETING_CONDITION_FORCE_DWORD = 0x7fffffff };

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
    void _M_erase(_Rb_tree_node_base* node);                // 0x0067A840
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x0067BBB0
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

template <> __forceinline void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream, long* out) const
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

// Same-TU stand-ins (auto_inline off, noinline) for the out-of-line element traits 0x0045E011 / 0x0040EFB0: they read only the
// stream and write only through `out`; seeing that lets VC7.1 home the empty traits temporaries on the pair's first word
// and pack the pair over the count's dead slot as retail does (opaque declarations cost 8 more frame bytes).
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<EOpinionReactionType>::TransferBinaryIn(CMemoryDataInputStream& stream, EOpinionReactionType* out) const
{
    *out = (EOpinionReactionType)stream.position;
}
template <> __declspec(noinline) void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream& stream, float* out) const
{
    *out = (float)stream.position;
}
#pragma auto_inline(on)

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& out)
{
    out.clear();
    long count;
    CPersistTraits<long>().TransferBinaryIn(stream, &count);
    for (; count > 0; --count) {
        std::pair<K, V> value;
        CPersistTraits<K>().TransferBinaryIn(stream, &value.first);
        CPersistTraits<V>().TransferBinaryIn(stream, &value.second);
        out.insert(typename std::map<K, V, C>::value_type(value.first, value.second));
    }
}

template void __fastcall GFSerialiseMapBinaryIn<float, EOpinionReactionType, std::less<EOpinionReactionType> >(CMemoryDataInputStream& stream, std::map<EOpinionReactionType, float, std::less<EOpinionReactionType> >& out);