// GFSerialiseMapBinaryIn(CMemoryDataInputStream&, std::map<EObjectAugmentationType,CWeaponTrailGraphicSet>&) -- [instantiation <CWeaponTrailGraphicSet,EObjectAugmentationType,std::less<EObjectAugmentationType> >] binary persist read of the weapon-trail graphic map: clear it, read the count, then per entry read key and value through their persist traits and insert the pair
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9156, 189 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`; ebp frame of a
//   size TU (`#pragma optimize("y", off)` + `("s", on)`: out-of-line clear, `movsd` 4-byte read, `and/or` immediates).
// Callees: map::clear @ 0x004DB9EC, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CPersistTraits<EObjectAugmentationType>::
//   TransferBinaryIn @ 0x00452D8E, CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn @ 0x004E9021, map insert @ 0x004E9213.
// Identity: FableWin.pdb ??$GFSerialiseMapBinaryIn@VCWeaponTrailGraphicSet@@W4EObjectAugmentationType@@...; the only caller is
//   0x004E9106 (a CPersistContext map transfer, load-stream arm). The catalog's list<COpinionTransientOffset> label is wrong.
// Layout: Ego_r.pdb CWeaponTrailGraphicSet {long Attack, long Knockdown} (8 bytes); the loop pair is {key, Attack, Knockdown}.
// Shape: both traits objects live at function scope: the key traits' DefaultVal is value-initialised once (`and [ebp-0Ch],0`
//   first thing), the value traits get a frame word but no stores, so they are modelled as an empty specialization (the
//   PDB gives CPersistTraits<CWeaponTrailGraphicSet> an 8-byte DefaultVal, whose -1/-1 default construction retail lacks).
//   The loop pair is value-initialised each pass (key 0, the set's ctor -1/-1: `and ; or ; or`), the map pointer is
//   spilled for the insert, and the insert result shares the count's dead frame word.
#pragma optimize("y", off)
#pragma optimize("s", on)
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

enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0, OBJECT_AUGMENTATION_FORCE_DWORD = 0x7fffffff };
class CWeaponTrailGraphicSet {
public:
    long Attack;                                            // +0x00
    long Knockdown;                                         // +0x04
    CWeaponTrailGraphicSet() : Attack(-1), Knockdown(-1) { }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less { };
struct _Rb_tree_iterator { void* _M_node; };
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    void*         _M_header;
    unsigned long _M_node_count;
    C             _M_key_compare;
    void clear();                                           // 0x004DB9EC
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x004E9213
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStream& stream, T* out) const;
};
template <> struct CPersistTraits<CWeaponTrailGraphicSet> {
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStream& stream, CWeaponTrailGraphicSet* out) const;   // 0x004E9021
};

template <> __forceinline void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream, long* out) const
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& out)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    out.clear();
    long count;
    CPersistTraits<long>().TransferBinaryIn(stream, &count);
    for (; count > 0; --count) {
        std::pair<K, V> value;
        keyTraits.TransferBinaryIn(stream, &value.first);
        valueTraits.TransferBinaryIn(stream, &value.second);
        out.insert(typename std::map<K, V, C>::value_type(value.first, value.second));
    }
}

template void __fastcall GFSerialiseMapBinaryIn<CWeaponTrailGraphicSet, EObjectAugmentationType, std::less<EObjectAugmentationType> >(
    CMemoryDataInputStream& stream, std::map<EObjectAugmentationType, CWeaponTrailGraphicSet, std::less<EObjectAugmentationType> >& out);