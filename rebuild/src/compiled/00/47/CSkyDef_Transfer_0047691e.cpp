// CSkyDef::Transfer(CPersistContext& context) -- persist the CSkyDef's fields (15 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSkyDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047691E, 178 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSkyDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123353C, whose RTTI type descriptor is .?AVCSkyDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CSkyDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSkyDef@@UAEXAAVCPersistContext@@@Z" agrees; family H054 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(float&) 0x00431061, Transfer(std::vector<CLensFlareElementDef >&) 0x004769D0.
// Layout: Ego_r.pdb CSkyDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {      // STLport, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
class CLensFlareElementDef {                                       // Ego_r.pdb, 80 bytes (opaque here)
public:
    unsigned char _body[80];
};
class CPersistContext {
public:
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(std::vector<CLensFlareElementDef >& value);    // 0x004769D0
};
class CSkyDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123353C slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    long SunRadius;                                    // +0x3C (Ego_r +0x40)
    long SunTexture;                                   // +0x40 (Ego_r +0x44)
    long MoonRadius;                                   // +0x44 (Ego_r +0x48)
    long MoonTexture;                                  // +0x48 (Ego_r +0x4C)
    long SunFlareRadius;                               // +0x4C (Ego_r +0x50)
    long SunFlareTexture;                              // +0x50 (Ego_r +0x54)
    long StarTexture;                                  // +0x54 (Ego_r +0x58)
    long StarChartTextureSize;                         // +0x58 (Ego_r +0x5C)
    float StarSize;                                    // +0x5C (Ego_r +0x60)
    long StarChartFilter;                              // +0x60 (Ego_r +0x64)
    long TwinkleInterval;                              // +0x64 (Ego_r +0x68)
    float TwinkleSpeed;                                // +0x68 (Ego_r +0x6C)
    float TwinkleMin;                                  // +0x6C (Ego_r +0x70)
    float TwinkleMax;                                  // +0x70 (Ego_r +0x74)
    std::vector<CLensFlareElementDef > FlareElements;  // +0x74 (Ego_r +0x78)
};

void CSkyDef::Transfer(CPersistContext& context)
{
    context.Transfer(SunRadius);
    context.Transfer(SunTexture);
    context.Transfer(SunFlareRadius);
    context.Transfer(SunFlareTexture);
    context.Transfer(MoonRadius);
    context.Transfer(MoonTexture);
    context.Transfer(StarTexture);
    context.Transfer(StarChartTextureSize);
    context.Transfer(StarSize);
    context.Transfer(StarChartFilter);
    context.Transfer(TwinkleInterval);
    context.Transfer(TwinkleSpeed);
    context.Transfer(TwinkleMin);
    context.Transfer(TwinkleMax);
    context.Transfer(FlareElements);
}