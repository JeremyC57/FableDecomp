// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<EObjectAugmentationType, CDefString>& out) -- clear the map, read the count, then read each key (enum traits) and CDefString value and insert the pair
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC66C, length 171 bytes (__fastcall stream, out).
// Callees: map::clear 0x004DB99F, map::insert 0x004EC717, CDefString::TransferBinaryIn 0x0044FC00 (via the CDefString traits), the enum key traits (out of line).
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: the count read is the inline long traits (`CPersistTraits<long>().TransferBinaryIn`); CDefString default-constructs TablePos = -1.
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

enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0, OBJECT_AUGMENTATION_FORCE_DWORD = 0x7fffffff };
class CDefString {
public:
    long TablePos;
    CDefString() : TablePos(-1) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream);  // 0x0044FC00
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
    void clear();                                           // 0x004DB99F
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x004EC717
};
}
template <class T> struct CPersistTraits {
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;
};
template <> struct CPersistTraits<CDefString> {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, CDefString* out) const { out->TransferBinaryIn(stream); }
};
template <> __forceinline void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, long* out) const
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<EObjectAugmentationType>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, EObjectAugmentationType* out) const
{
    *out = (EObjectAugmentationType)stream.position;
}
#pragma auto_inline(on)
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStreamReader& stream, std::map<K, V, C>& out)
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
template void __fastcall GFSerialiseMapBinaryIn<CDefString, EObjectAugmentationType, std::less<EObjectAugmentationType> >(
    CMemoryDataInputStreamReader& stream, std::map<EObjectAugmentationType, CDefString, std::less<EObjectAugmentationType> >& out);