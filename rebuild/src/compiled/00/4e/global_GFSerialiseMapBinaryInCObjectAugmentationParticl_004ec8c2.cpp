// GFSerialiseMapBinaryIn<CObjectAugmentationParticleSet,EObjectAugmentationType,less<EObjectAugmentationType> >(CMemoryDataInputStream& stream, map<EObjectAugmentationType,CObjectAugmentationParticleSet>& out) -- binary persist read of the augmentation particle-set map: clear it, read the entry count, then per entry read the type key and the particle set into a default pair and insert a copy
// Leaf: ??$GFSerialiseMapBinaryIn@VCObjectAugmentationParticleSet@@W4EObjectAugmentationType@@U?$less@W4EObjectAugmentationType@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@U?$less@W4EObjectAugmentationType@@@std@@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EC8C2, 243 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`; frame-pointer
//   size TU (`lea ebp,[esp-78h]`).
// Identity (proven): (1) Ego_r.pdb and FableWin.pdb both hold exactly this mangled name (GFSerialiseMapBinaryIn<V, K, C>,
//   __fastcall, CMemoryDataInputStream&, map&); (2) its only caller 0x004EC851 is the load arm of the CPersistContext map
//   transfer whose save arm is the landed GFSerialiseMapBinaryOut<CObjectAugmentationParticleSet, EObjectAugmentationType>
//   0x004EC85A; (3) its callees are the EObjectAugmentationType key reader 0x00452D8E, the CObjectAugmentationParticleSet
//   value reader 0x004E9642, the pair default constructor 0x004EC9B5 (float 0 + two empty CArray<CDefString>) and the
//   CObjectAugmentationParticleSet copy constructor / destructor 0x004EC46D / 0x004E25F1 (Ego_r names).
// Label: the catalog's `GFSerialiseMapBinaryIn<CCharString,long,std::less<long> >` is propagated (wrong K and V).
// Callees: _Rb_tree::clear 0x004E8212, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the count's slow path),
//   pair<EObjectAugmentationType, CObjectAugmentationParticleSet>::pair() 0x004EC9B5,
//   CPersistTraits<EObjectAugmentationType>::TransferBinaryIn 0x00452D8E, CPersistTraits<CObjectAugmentationParticleSet>::
//   TransferBinaryIn 0x004E9642, CObjectAugmentationParticleSet copy ctor 0x004EC46D, _Rb_tree::insert_unique 0x004EC9D3,
//   ~CObjectAugmentationParticleSet 0x004E25F1.
// Shape: the value traits hold a DefaultVal CObjectAugmentationParticleSet built inline (PeriodSecs 0.0 by `fldz ; fstp`, the
//   two empty CArray<CDefString> as six zero stores; the four effect indices are left alone) and destroyed at the end; the
//   key traits object shares the spilled map pointer's frame word: the real 0x00452D8E body is a same-TU noinline callee
//   (it never reads `this`), which lets VC7.1 home it there as retail does.
// Frame: the biased `lea ebp,[esp-78h]` size frame comes from `optimize("s", on)` alone (no `y off`); the pair default
//   constructor is out of line in retail (declared here, not defined).
// Model: CDataInputStream per Ego_r.pdb with its inline Read (the engine/ headers are flat layouts without methods).
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CBaseClass { public: virtual ~CBaseClass(); };                  // 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };
class CDataStream : public CBaseClassNonCopyable { };                 // vptr +0x00
class CDataInputStream : public CDataStream {            // Ego_r.pdb 0x18
public:
    unsigned long  StreamPos;                            // +0x04
    long           StreamSize;                           // +0x08
    unsigned char* CurrentSourcePtr;                     // +0x0C
    unsigned long  SourceChunkStreamPos;                 // +0x10
    long           SourceChunkBytesRemaining;            // +0x14
    static __forceinline bool IsValidPosition(__int64 pos) { return pos >= 0 && pos <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 new_pos = (__int64)(StreamPos + size);
        if (!IsValidPosition(new_pos))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    template <class T> __forceinline T ReadType() { T value; Read(&value, sizeof(T)); return value; }
    __forceinline long ReadSLONG() { return ReadType<long>(); }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream { };

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    vector() { _M_start = 0; _M_finish = 0; _M_end_of_storage._M_data = 0; }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair();                                              // out of line in retail (0x004EC9B5 for this pair)
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct _Nonconst_traits { };
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;    // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare     _M_key_compare;                                        // +0x08
    pair<iterator, bool> insert_unique(const value_type& __x);          // 0x004EC9D3
    void clear();                                                       // 0x004E8212
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
    void clear() { _M_t.clear(); }
};
}   // namespace std

enum EObjectAugmentationType { };                        // Ego_r.pdb enum (values not needed here)
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
class CObjectAugmentationParticleSet {                   // Ego_r.pdb (retail 0x2C: STLport CArray)
public:
    float              PeriodSecs;                       // +0x00
    long               ParticleEffect;                   // +0x04
    long               ParticleEffectToBlendTo;          // +0x08
    long               AttackParticleEffect;             // +0x0C
    long               AttackParticleEffectToBlendTo;    // +0x10
    CArray<CDefString> StartPoint;                       // +0x14
    CArray<CDefString> EndPoint;                         // +0x20
    CObjectAugmentationParticleSet() : PeriodSecs(0.0f) { }
    CObjectAugmentationParticleSet(const CObjectAugmentationParticleSet& other);   // 0x004EC46D (implicit in retail)
    ~CObjectAugmentationParticleSet();                   // 0x004E25F1 (implicit in retail)
};

template <class T> class CPersistTraits {
public:
    T DefaultVal;                                        // +0x00
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);
};

// CPersistTraits<EObjectAugmentationType>::TransferBinaryIn 0x00452D8E: the real body as a same-TU noinline callee.
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<EObjectAugmentationType>::TransferBinaryIn(CMemoryDataInputStream& stream,
                                                                                               EObjectAugmentationType& value)
{
    value = stream.ReadType<EObjectAugmentationType>();
}
#pragma auto_inline(on)

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& out)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    out.clear();
    long count = stream.ReadSLONG();
    for (; count > 0; --count) {
        std::pair<K, V> value;
        keyTraits.TransferBinaryIn(stream, value.first);
        valueTraits.TransferBinaryIn(stream, value.second);
        out.insert(typename std::map<K, V, C>::value_type(value.first, value.second));
    }
}

template void __fastcall GFSerialiseMapBinaryIn<CObjectAugmentationParticleSet, EObjectAugmentationType, std::less<EObjectAugmentationType> >(
    CMemoryDataInputStream& stream,
    std::map<EObjectAugmentationType, CObjectAugmentationParticleSet, std::less<EObjectAugmentationType> >& out);