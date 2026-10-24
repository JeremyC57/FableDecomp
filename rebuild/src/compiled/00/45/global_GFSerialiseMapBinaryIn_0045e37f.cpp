// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<EOpinionDeedType, bool>& out) -- clear the map, read the count, then read each opinion-deed key and bool flag and insert the pair (COpinionDeedMaskDef::DeedEnabled)
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E37F, length 175 bytes (__fastcall stream, out).
// Identity: the catalog label <bool, EOpinionAttitudeType> is propagated; the key here is EOpinionDeedType.
// Callees: map::clear 0x00451159, map::insert 0x0045E42E, key traits 0x00456BBE (stand-in), bool traits 0x00403EB0 (stand-in).
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: this key traits is EMPTY (no DefaultVal) and both traits calls need stand-ins.
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

enum EOpinionDeedType { EOPINIONDEEDTYPE_FORCE_DWORD = 0x7fffffff };
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
    void clear();                                           // 0x00451159
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x0045E42E
};
}
template <class T> struct CPersistTraits {
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x00456BBE for the key
};
template <> struct CPersistTraits<bool> {
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, bool* out) const;   // 0x00403EB0
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<EOpinionDeedType>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, EOpinionDeedType* out) const
{
    *out = (EOpinionDeedType)stream.position;
}
__declspec(noinline) void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, bool* out) const
{
    *out = stream.position != 0;
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
template void __fastcall GFSerialiseMapBinaryIn<bool, EOpinionDeedType, std::less<EOpinionDeedType> >(
    CMemoryDataInputStreamReader& stream, std::map<EOpinionDeedType, bool, std::less<EOpinionDeedType> >& out);