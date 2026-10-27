// CEngineTextureMorph::CEngineTextureMorph(const CEngineTextureMorph& other) -- member-wise copy constructor
// Leaf: ??0CEngineTextureMorph@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BB9FA, 35 bytes, thiscall, `ret 4`, returns this (size-optimised module).
// Identity (proven): its callers are the landed __uninitialized_copy<const CEngineTextureMorph*, CEngineTextureMorph*> 0x005BBC93 and the map insert 0x005BBB7D of that TU; the third member's copy is the bound vector<CCompositeBlendDesc> copy constructor 0x005BB346; Ego_r.pdb CEngineTextureMorph {long OriginalTextureIndex, long MorphID, vector<CCompositeBlendDesc> Blends}.
// Callees: the third member's out-of-line copy 0x005BB346.
// Shape (family M096, representative 0x004D6C66): the two leading dwords copied through ecx, then the third member's copy
//   with `lea ecx,[esi+8]` and the source + 8 pushed; `optimize("s")` keeps the third member's copy out of line.
#pragma optimize("s", on)
class CCompositeBlendDesc {                               // Ego_r.pdb, 16 bytes
public:
    unsigned long Members[4];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    vector(const vector& __x);                            // out of line
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CEngineTextureMorph {                               // Ego_r.pdb, 0x14 bytes in retail
public:
    CEngineTextureMorph(const CEngineTextureMorph& other);
    long OriginalTextureIndex;                            // +0x0
    long MorphID;                                         // +0x4
    std::vector<CCompositeBlendDesc> Blends;              // +0x8
};
CEngineTextureMorph::CEngineTextureMorph(const CEngineTextureMorph& other)
    : OriginalTextureIndex(other.OriginalTextureIndex), MorphID(other.MorphID), Blends(other.Blends)
{
}