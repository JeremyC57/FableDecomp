// GFWriteVectorMapMemDump(CDataOutputStream& stream, const CVectorMap<CRandomAppearanceMorph::CTextureMorph, unsigned long, CKeyPairCompareLess>& map) -- dump the vector map: entry count, byte size, raw entry bytes, dirty flag
// Leaf: GFWriteVectorMapMemDump
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B210, length 134 bytes (__fastcall stream, map).
// Callees: WriteLong 0x00993EB0 (twice), the stream's slot 4 Write(const void*, long), WriteBool 0x00993E30.
// Model: engine/CDataOutputStream.h + writer adapter; the virtual Write goes through a slot view of the vptr (the header flattens it).
// Shape: CVectorMap = {vector<pair<K, V> > Entries; Compare; bool Dirty}; GetData() is `size() ? begin : this` (retail's `cmovz`-free
//   branch on the size), and size() is recomputed for each use as in retail.
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamVirtuals {
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3();
    virtual void Write(const void* data, long size);
};
struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteBool(bool value);                             // 0x00993E30
    void WriteLong(long value);                             // 0x00993EB0
    void Write(const void* data, long size) { reinterpret_cast<CDataOutputStreamVirtuals*>(this)->Write(data, size); }
};
class CRandomAppearanceMorph { public:
    class CTextureMorph { public: long A; long B; };   // one texture-morph entry (8 bytes)
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
}
template <class K, class V> class CKeyPairCompareLess { public: };
template <class V, class K, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Entries;                        // +0x00
    C                       Compare;                        // +0x0c
    bool                    Dirty;                          // +0x0d
    unsigned int size() const { return Entries.size(); }
    const void* GetData() const { return size() ? (const void*)Entries._M_start : (const void*)this; }
};
template <class V, class K, class C>
void __fastcall GFWriteVectorMapMemDump(CDataOutputStreamWriter& stream, const CVectorMap<V, K, C>& map)
{
    stream.WriteLong(map.size());
    stream.WriteLong(map.size() * sizeof(typename CVectorMap<V, K, C>::value_type));
    stream.Write(map.GetData(), map.size() * sizeof(typename CVectorMap<V, K, C>::value_type));
    stream.WriteBool(map.Dirty);
}
template void __fastcall GFWriteVectorMapMemDump<CRandomAppearanceMorph::CTextureMorph, unsigned long, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >(
    CDataOutputStreamWriter& stream, const CVectorMap<CRandomAppearanceMorph::CTextureMorph, unsigned long, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >& map);