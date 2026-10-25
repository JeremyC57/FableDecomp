// GFSerialiseMapBinaryIn(CMemoryDataInputStream&, map<CCharString, bool>&) -- clear the map, read the entry count, then for each entry read the key string (stream slot 6) and the bool value (CPersistTraits<bool>) and insert the pair
// Leaf: GFSerialiseMapBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ADBF0, 286 bytes (padded to 288), `__fastcall` (ecx stream, edx map), plain `ret`;
//   one caller 0x007AE456.
// Label: the catalog name GFSerialiseMapBinaryIn<std::pair<long,long>,CCharString,std::less<CCharString>> is propagated from
//   the byte-similar 0x007565E0 (landed, map<CCharString, pair<long,long> >): this row's value is ONE byte read by
//   CPersistTraits<bool>::TransferBinaryIn @ 0x00403EB0, so the instance is the map<CCharString, bool, less<CCharString> > one.
// Callees: CCharString() @ 0x0099E4B0, the map's _M_erase @ 0x007ACC50 (manifest "LTextTreeWalkThrough::BuildTreeArray" is
//   propagated), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, the stream's virtual slot 6 ReadString (hidden
//   CCharString result), CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, CPersistTraits<bool>::TransferBinaryIn
//   @ 0x00403EB0, CCharString(const CCharString&) @ 0x0099EC30, the map's insert(const value_type&) @ 0x007AD060.
// Layout: CMemoryDataInputStream (engine header): position +4, chunkBase +0xc, chunkLength +0x14. STLport map: header node
//   pointer +0, node count +4; header node Parent +4 / Left +8 / Right +0xc.
// Shape: the landed 0x007565E0 sibling with a bool value: the pair's value is zeroed after the key's default construction, the
//   value reader has a same-TU read-only stand-in (with a declared-only reader VC keeps the count and the traits object in
//   frame words of their own: 0x28 frame), the inserted pair is a copy built in the insert call.
#include "engine/CMemoryDataInputStream.h"
namespace X_007adbf0 {
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
// CDataInputStream::ReadWithSrcChunkOverflow at 0x00993CA0 is inherited through this method-only bridge.
struct CMemoryDataInputStreamAdapter : ::CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char*, int);   // 0x00993CA0
    __forceinline void Read(void* out, int size)
    {
        __int64 next = (__int64)(unsigned)(position + size);
        if (next < 0 || next > 0x7fffffff) return;
        if (chunkLength >= size) { memcpy(out, (const void*)chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)out, size);
    }
    __forceinline unsigned int ReadWord() { unsigned int v; Read(&v, 4); return v; }
};

struct CCharString {
    void* PStringData;
    CCharString();                                // 0x0099E4B0
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                               // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
// Method-only ABI view of the stream's virtual slots (slot 6 returns CCharString through a hidden result pointer).
struct CMemoryDataInputStreamVirtualSlots {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCharString ReadString();             // slot 6 (+0x18)
};
namespace std {
template <class First, class Second> struct pair {
    First first;
    Second second;
    pair() { }
    pair(const First& f, const Second& s) : first(f), second(s) { }
};
}
typedef std::pair<CCharString, bool> Pair;
struct _Rb_tree_node_base { unsigned int State; _Rb_tree_node_base* Parent; _Rb_tree_node_base* Left; _Rb_tree_node_base* Right; };
struct _Rb_tree_insert_result {
    _Rb_tree_node_base* Position;
    bool Added;
    _Rb_tree_insert_result(_Rb_tree_node_base* p, bool a) : Position(p), Added(a) { }
    _Rb_tree_insert_result(const _Rb_tree_insert_result& x) : Position(x.Position), Added(x.Added) { }
};
struct map {                                      // map<CCharString, bool, less<CCharString> >
    _Rb_tree_node_base* Head;
    unsigned int Size;
    void _M_erase(_Rb_tree_node_base*);           // 0x007ACC50
    _Rb_tree_insert_result insert(const Pair&);   // 0x007AD060
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamAdapter& in, T* p);   // 0x00403EB0 for bool
};
// Same-TU stand-in body for the out-of-line reader (the real one is 0x00403EB0, another TU): it reads one byte and writes *p
// without letting either address escape, so VC7.1 may share the frame words as retail does (the landed 0x007565E0 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStreamAdapter& in, bool* p)
{
    unsigned char value;
    in.Read(&value, 1);
    *p = value != 0;
}
#pragma auto_inline(on)

void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStreamAdapter& in, map& out)
{
    CCharString temporary;
    if (out.Size != 0) {
        out._M_erase(out.Head->Parent);
        out.Head->Left = out.Head;
        out.Head->Parent = 0;
        out.Head->Right = out.Head;
        out.Size = 0;
    }
    int count = (int)in.ReadWord();
    for (; count > 0; --count) {
        Pair pair;
        pair.second = false;
        pair.first = reinterpret_cast<CMemoryDataInputStreamVirtualSlots&>(in).ReadString();
        CPersistTraits<bool> traits;
        traits.TransferBinaryIn(in, &pair.second);
        out.insert(Pair(pair.first, pair.second));
    }
}

}