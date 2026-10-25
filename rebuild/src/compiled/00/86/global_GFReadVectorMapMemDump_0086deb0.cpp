// GFReadVectorMapMemDump(CMemoryDataInputStream&, CVectorMap&) -- read a vector map's memory dump: the entry count, the byte size (skipped), resize the entries to the count, read the raw entry bytes straight into them, then the Dirty flag
// Leaf: ??$GFReadVectorMapMemDump@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086DEB0, 433 bytes (padded to 448), `__fastcall` (ecx stream, edx map), plain `ret`.
// Name: Ego_r.pdb `??$GFReadVectorMapMemDump@VCTextureMorph@CRandomAppearanceMorph@@KV?$CKeyPairCompareLess@K...@@@@YIXAAV
//   CMemoryDataInputStream@@AAV?$CVectorMap@KVCTextureMorph@...@@@Z` -- the reader twin of the landed GFWriteVectorMapMemDump
//   0x0086B210 (CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<...> >).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined CMemoryDataInputStream::Read's slow path, four
//   sites), vector<pair<unsigned long, CTextureMorph> >::resize(size_type, const value_type&) @ 0x0086D900 (manifest
//   "vector<CBrainBehaviour>::resize" is propagated).
// Layout: CMemoryDataInputStream (engine header): position +4, chunkBase +0xc, chunkLength +0x14. CVectorMap (retail 16 bytes):
//   STLport vector of 12-byte pairs {unsigned long key; CTextureMorph {NewTexture, GroupID}} +0, empty Compare +0xc, Dirty +0xd.
// Shape: the landed GFWriteVectorMapMemDump model (`GetData()` is `size() ? begin : this`, size() recomputed per use); the
//   stream's Read is the landed inline (64-bit position check, memcpy fast path) with a `size > 0` guard that folds away for the
//   fixed-size reads and its chunk test is `size <= chunkLength` (operand order shows only for the variable size); the default
//   element is a pair temporary whose CTextureMorph has a user zeroing ctor (VC then keeps one zero register, esi, for the
//   temporary's stores and every 64-bit range compare; a POD value-init zeroes through eax and the compares turn into `test`);
//   ReadBool returns `byte != 0`. 3 grades.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void Read(void* out, long size)
    {
        if (size > 0) {
            __int64 next = (__int64)(unsigned)(position + size);
            if (next < 0 || next > 0x7fffffff) return;
            if (size <= chunkLength) { memcpy(out, (const void*)chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
            else ReadWithSrcChunkOverflow((unsigned char*)out, size);
        }
    }
    __forceinline long ReadLong() { long v; Read(&v, 4); return v; }
    __forceinline bool ReadBool() { unsigned char v; Read(&v, 1); return v != 0; }
};

class CRandomAppearanceMorph {
public:
    class CTextureMorph {                   // Ego_r.pdb (8 bytes); PDB def types are classes (mangled V)
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
        CTextureMorph() : NewTexture(0), GroupID(0) { }
    };
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    void resize(unsigned int newSize, const T& x);   // 0x0086D900
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Entries;       // +0x00
    C                        Compare;       // +0x0c (empty)
    bool                     Dirty;         // +0x0d
    unsigned int size() const { return Entries.size(); }
    void* GetData() { return size() ? (void*)Entries._M_start : (void*)this; }
};

template <class V, class K, class C>
void __fastcall GFReadVectorMapMemDump(CMemoryDataInputStreamReader& stream, CVectorMap<K, V, C>& map)
{
    long count = stream.ReadLong();
    long bytes = stream.ReadLong();
    map.Entries.resize(count, typename CVectorMap<K, V, C>::value_type());
    stream.Read(map.GetData(), map.size() * sizeof(typename CVectorMap<K, V, C>::value_type));
    map.Dirty = stream.ReadBool();
}
template void __fastcall GFReadVectorMapMemDump<CRandomAppearanceMorph::CTextureMorph, unsigned long,
    CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >(CMemoryDataInputStreamReader& stream,
    CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >& map);