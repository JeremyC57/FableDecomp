// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<CCharString, long>& out) -- clear the map, read the count, then read each string key and long value and insert the pair (CCreatureDef::PhonemeAnim)
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043151D, length 233 bytes (__fastcall stream, out).
// Identity: the catalog label <pair<long,long>, CCharString> is propagated; the map here is <CCharString, long>.
// Callees: map::clear 0x0043027A, map::insert 0x00431606.
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: the string traits calls the stream's slot 6 ReadNullTerminatedString (0x00995D30) DIRECTLY through a vtable view
//   (an inline wrapper re-addresses the string temporary with an extra `lea`); CCharString ctor 0x0099E4B0, dtor 0x0099EAE0,
//   operator= 0x0099EFB0; the long traits 0x0040FE60 is out of line (stand-in).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

// Virtual-slot view of the stream's vptr (the shared header flattens it to a data word): slot 6 is
// CMemoryDataInputStream::ReadNullTerminatedString (0x00995D30), returning the string by hidden pointer.
struct CDataInputStreamVirtuals {
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2();
    virtual void Slot3(); virtual void Slot4(); virtual void Slot5();
    virtual CCharString ReadNullTerminatedString();
};

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
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less { };
struct _Rb_tree_iterator {
    void* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator& other) : _M_node(other._M_node) { }
};
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    void*         _M_header;
    unsigned long _M_node_count;
    C             _M_key_compare;
    void clear();                                           // 0x0043027A
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x00431606
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;
};
template <> __forceinline void CPersistTraits<CCharString>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, CCharString* out) const
{
    *out = reinterpret_cast<CDataInputStreamVirtuals&>(stream).ReadNullTerminatedString();
}
template <> struct CPersistTraits<long> {
    CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, long* out) const;   // 0x0040FE60
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
// Same-TU stand-in (auto_inline off, noinline) for the out-of-line long traits 0x0040FE60: it reads only the stream and
// writes only through `out`, which lets VC7.1 home the empty long-traits object on the string traits' word.
#pragma auto_inline(off)
__declspec(noinline) void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, long* out) const
{
    *out = stream.position;
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
template void __fastcall GFSerialiseMapBinaryIn<long, CCharString, std::less<CCharString> >(
    CMemoryDataInputStreamReader& stream, std::map<CCharString, long, std::less<CCharString> >& out);