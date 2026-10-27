// GFSerialiseMapBinaryIn<float, EDamageAttribute, less<EDamageAttribute> >(CMemoryDataInputStream& stream, map<EDamageAttribute, float>& out) -- binary persist read of a map: clear it, read the entry count, then per entry read the key and the float and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@MW4EDamageAttribute@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046190A, 176 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`, frame pointer.
// Identity (probable): its insert 0x004619BA is bound ?insert@?$map@W4EDamageAttribute@@MU?$less@W4EDamageAttribute@@@std@@@std@@... (CArmourDef's DamageTypeResponse / ArmourThresholds); caller 0x00461894.
// Callees: map::clear() 0x0045128D (out of line), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path),
//   CPersistTraits<EDamageAttribute>::TransferBinaryIn 0x00459083, CPersistTraits<float>::TransferBinaryIn 0x0040EFB0, map::insert(const value_type&)
//   0x004619BA (pair<iterator, bool> through a hidden pointer).
// Shape (family K096, representative 0x004616D6): the landed GFSerialiseMapBinaryIn 0x0067C110 in a frame/size TU (the 0x00469381
//   recipe): clear() called, the value-initialised pair as `and [k],0 ; fldz ; fstp [v]`, the empty function-scope traits
//   objects homed on the spilled map pointer's slot; CMemoryDataInputStream from engine/CMemoryDataInputStream.h.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
enum EDamageAttribute { EDamageAttribute_Max = 0x7fffffff };
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
    void*         _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    C             _M_key_compare;                           // +0x08
    void clear();                                           // 0x0045128D (out of line in this TU)
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x004619BA
};
}
template <class T> struct CPersistTraits {
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
// Same-TU noinline stand-ins for the out-of-line traits readers (CPersistTraits<EDamageAttribute> 0x00459083, CPersistTraits<float>
// 0x0040EFB0): they only read the stream and write through `out`, which lets VC7.1 home the empty traits objects as retail does.
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<EDamageAttribute>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, EDamageAttribute* out) const
{
    *out = (EDamageAttribute)stream.position;
}
template <> __declspec(noinline) void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, float* out) const
{
    *out = (float)(long)stream.position;
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
template void __fastcall GFSerialiseMapBinaryIn<float, EDamageAttribute, std::less<EDamageAttribute> >(CMemoryDataInputStreamReader& stream, std::map<EDamageAttribute, float, std::less<EDamageAttribute> >& out);