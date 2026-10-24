// GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites,std::less<NUISystem::ETableSprites> >(CMemoryDataInputStream&, std::map<NUISystem::ETableSprites,long>&) -- binary persist read of the UI def's sprite map: clear it, read the count, then per entry read key and value through their persist traits and insert the pair
// Grade: DIFFER(ours v retail) -- 196v196: frame-slot assignment only: retail leaves L+0x10 unused and puts the pair at +0x14/+0x18 (count overflow slot +0x14) and the traits word at +0x1C; ours packs the pair at +0x10/+0x14 and the traits at +0x18 (every displacement 4 lower), all instructions otherwise identical
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00632830, 196 bytes (+12 int3 padding), `__fastcall` (ecx = stream, edx = map), plain `ret`.
// Callees: map::clear @ 0x00630B20 (out of line in this unit), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//   CPersistTraits<NUISystem::ETableSprites>::TransferBinaryIn @ 0x00632900, CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60,
//   map insert @ 0x00632990 (pair<iterator,bool> through a hidden pointer).
// Identity: FableWin.pdb ??$GFSerialiseMapBinaryIn@JW4ETableSprites@NUISystem@@...; the only caller is 0x006327CF, the map
//   transfer of NUISystem::CUIDef::Sprites (+0x64 in retail; Ego_r.pdb +0x6C with 16-byte vectors), from the CUIDef
//   persist (vtable 0x01259F8C slot 18). The catalog's list<COpinionTransientOffset> label is wrong.
// Layout: STLport map = 12 bytes {header node*, node count, empty less<>}; 8-byte pairs on the frame (`and esp,-8`).
// Shape: the count goes through an inlined CPersistTraits<long>::TransferBinaryIn (fast path straight into edi); the
//   loop value-initialises the pair (two zero stores after the first argument pushes) and reads its two words through
//   function-scope traits objects that share one frame word (the key traits' DefaultVal zeroed at entry; the value
//   traits are never written and VC7.1 overlaps them with the key traits, `lea ecx,[esp+24h]` for both calls).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
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

namespace NUISystem { enum ETableSprites { TABLE_SPRITE_NULL = 0, TABLE_SPRITE_FORCE_DWORD = 0x7fffffff }; }

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
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x00632990
    void clear();                                           // 0x00630B20
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T* out) const;
};

__forceinline void ReadCount(CMemoryDataInputStream& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <> void CPersistTraits<NUISystem::ETableSprites>::TransferBinaryIn(CMemoryDataInputStream& stream, NUISystem::ETableSprites* out) const;   // 0x00632900
template <> void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream, long* out) const;   // 0x0040FE60

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& out)
{
    CPersistTraits<K> keyTraits;
    keyTraits.DefaultVal = K();
    CPersistTraits<V> valueTraits;
    out.clear();
    long count;
    ReadCount(stream, &count);
    for (; count > 0; --count) {
        std::pair<K, V> value;
        keyTraits.TransferBinaryIn(stream, &value.first);
        valueTraits.TransferBinaryIn(stream, &value.second);
        out.insert(typename std::map<K, V, C>::value_type(value.first, value.second));
    }
}

template void __fastcall GFSerialiseMapBinaryIn<long, NUISystem::ETableSprites, std::less<NUISystem::ETableSprites> >(CMemoryDataInputStream& stream,
    std::map<NUISystem::ETableSprites, long, std::less<NUISystem::ETableSprites> >& out);
