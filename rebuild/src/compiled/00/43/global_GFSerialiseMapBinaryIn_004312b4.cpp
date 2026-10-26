// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<unsigned long, unsigned long>& out) -- clear the map, read the count, then read each key and value through their persist traits and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@KKU?$less@K@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004312B4, 175 bytes, `__fastcall` (stream in ecx, the map in edx, homed at [ebp-0Ch]), plain `ret`; ebp frame (a size-optimised /Os module: unaligned entry).
// Identity (proven): GFSerialiseMapBinaryIn<unsigned long, unsigned long, less<unsigned long> >(CMemoryDataInputStream&, map<unsigned long, unsigned long>&) (FableWin.pdb name set); evidence: its callers are CPersistContext::TransferBinary<map<unsigned long, unsigned long> > 0x00431242 (0x0043126E), called by NUISystem::CUIMiscThingsDef::Transfer 0x00430900 on this+0x188 (Ego_r.pdb CUIMiscThingsDef::MapPaths, map<unsigned long, unsigned long>) and NUISystem::CUIDef::Transfer 0x00631C60 on this+0x1B4 (CUIDef::ActionMapAliases, map<unsigned long, unsigned long>) (vtables by RTTI); both element reads call CPersistTraits<unsigned long>::TransferBinaryIn 0x0040F8A0
// Label: the manifest's "GFSerialiseMapBinaryIn<long,long,std::less<long>_>" is a propagated label: this row is GFSerialiseMapBinaryIn<unsigned long, unsigned long, less<unsigned long> > (family J047, each row its own instantiation).
// Callees: _Rb_tree::clear() 0x00430046, CPersistTraits<unsigned long>::TransferBinaryIn 0x0040F8A0, CPersistTraits<unsigned long>::TransferBinaryIn 0x0040F8A0,
//   _Rb_tree::insert_unique(const value_type&) 0x00431363, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (FableWin.pdb, private).
// Shape: FableWin.pdb ??$GFSerialiseMapBinaryIn@<V><K><C>: one key-traits and one value-traits object for the whole call (FableWin.pdb: CPersistTraits<T>
//   has a constructor and a destructor; the constructor value-initialises DefaultVal), clear, the count through the inline ReadSLONG (CDataInputStream::Read:
//   64-bit range check, memcpy from the source chunk, else ReadWithSrcChunkOverflow), then per element a zeroed pair<K, V>, the two traits reads and
//   map::insert of the converted value_type; the counted loop runs down a register.
// Model: both traits bodies (0x0040F8A0, 0x0040F8A0) were compiled earlier in the retail TU, so they are visible here (explicit member specializations as the device): neither traits object needs a store, and both share the homed map pointer's slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CDataInputStream CDataInputStreamPdb
#include "engine/CDataInputStream.h"
#undef CDataInputStream
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
class CDataInputStream : public CDataInputStreamPdb {      // engine/CDataInputStream.h is data-only: the reader is added here
public:
    __forceinline void Read(void* buffer, long size)       // FableWin.pdb ?Read@CDataInputStream@@QAEXPAXJ@Z (inline here)
    {
        __int64 newPosition = (__int64)(unsigned long)(StreamPos + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadSLONG() { long value; Read(&value, sizeof(value)); return value; }                          // FableWin.pdb ?ReadSLONG@CDataInputStream@@QAEJXZ
    unsigned long ReadULONG() { unsigned long value; Read(&value, sizeof(value)); return value; }        // FableWin.pdb ?ReadULONG@CDataInputStream@@QAEKXZ
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream {
public:
    unsigned char* StreamSource;                           // +0x18
};
template <class T> class CPersistTraits {
public:
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    ~CPersistTraits() { }
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);
};
#pragma auto_inline(off)
template <> void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream& stream, unsigned long& value)
{
    unsigned long temp;
    stream.Read(&temp, sizeof(temp));
    value = temp;
}
#pragma auto_inline(on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};
struct _Rb_tree_node_base;
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    _Compare            _M_key_compare;
    void clear();                                          // 0x00430046
    pair<iterator, bool> insert_unique(const _Value& __x);   // 0x00431363
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    void clear() { _M_t.clear(); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
};
}
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& out)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    out.clear();
    long count = stream.ReadSLONG();
    for (long i = 0; i < count; ++i) {
        std::pair<K, V> value;
        keyTraits.TransferBinaryIn(stream, value.first);
        valueTraits.TransferBinaryIn(stream, value.second);
        out.insert(value);
    }
}
template void __fastcall GFSerialiseMapBinaryIn<unsigned long, unsigned long, std::less<unsigned long > >(CMemoryDataInputStream&, std::map<unsigned long, unsigned long, std::less<unsigned long > >&);