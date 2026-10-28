// _global::GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<CDefString, long, CCompareDefString<long> >& out) -- clear the map, read the count, then read each CDefString key and long value and insert the pair
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5A76, 171 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Identity: FableWin.pdb ??$GFSerialiseMapBinaryIn@JVCDefString@@V?$CCompareDefString@J@@@@YIX..AAV?$map@VCDefString@@J..;
//   the only caller is the CPersistContext map transfer at 0x004E5A1F (CFlammableDef's serialiser); its writer twin is
//   0x004E5A39. CCompareDefString<long> is an empty comparator class.
// Callees: map::clear @ 0x004DBB20 (labelled CTCCarryable::OnKill, propagated), CDataInputStream::ReadWithSrcChunkOverflow
//   @ 0x00993CA0 (the inlined count read), CDefString::TransferBinaryIn @ 0x0044FC00 (the key, through the inline CDefString
//   traits), CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60 (the value, out of line), map::insert @ 0x004E5B21.
// Shape: the family recipe of the landed 0x004EC66C / 0x006DBA50: `#pragma optimize("y", off)` + `("s", on)` (frame-pointer
//   size TU, out-of-line clear); the count read is an inlined 4-byte read; the loop pair is value-initialised (CDefString's
//   TablePos -1 = `or [ebp-8],-1`, the long 0 = `and [ebp-4],0`, sharing the dead count word); the out-of-line long traits get
//   a same-TU noinline stand-in under auto_inline(off) (it reads only the stream and writes only through `out`), so VC7.1
//   homes the empty traits object on the map spill word (`lea ecx,[ebp-0Ch]`) exactly as retail.
// Model: engine/CMemoryDataInputStream.h + a methods-only reader adapter (the inline Read: 64-bit range check, memcpy from the
//   chunk, else ReadWithSrcChunkOverflow).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CDefString {
public:
    long TablePos;                                          // +0x00
    CDefString() : TablePos(-1) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream);  // 0x0044FC00
};
template <class T> class CCompareDefString { };
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_iterator { void* _M_node; };
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    void*         _M_header;
    unsigned long _M_node_count;
    C             _M_key_compare;
    void clear();                                           // 0x004DBB20
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x004E5B21
};
}
template <class T> struct CPersistTraits {
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;
};
template <> struct CPersistTraits<CDefString> {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, CDefString* out) const { out->TransferBinaryIn(stream); }
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
// Same-TU stand-in for the out-of-line long traits 0x0040FE60 (reads only the stream, writes only through `out`).
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, long* out) const
{
    *out = (long)stream.position;
}
#pragma auto_inline(on)
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStreamReader& stream, std::map<K, V, C>& out)
{
    CPersistTraits<K> keyTraits;
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
template void __fastcall GFSerialiseMapBinaryIn<long, CDefString, CCompareDefString<long> >(
    CMemoryDataInputStreamReader& stream, std::map<CDefString, long, CCompareDefString<long> >& out);