// CCameraManagerDef::Transfer(CPersistContext& context) -- persist the CCameraManagerDef's fields (10 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CCameraManagerDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A0B0, 123 bytes, virtual thiscall `ret 4`; size-optimised def TU (every member address through eax; the entry is 16-aligned by chance).
// Identity (proven): CCameraManagerDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01233BE4, whose RTTI type descriptor is .?AVCCameraManagerDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CCameraManagerDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CCameraManagerDef@@UAEXAAVCPersistContext@@@Z" agrees; family H058 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(std::vector<long >&) 0x00466A47, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061.
// Layout: Ego_r.pdb CCameraManagerDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
class CPersistContext {
public:
    void Transfer(std::vector<long >& value);                    // 0x00466A47
    template <class T> void Transfer(T& value);                                  // 0x00431102
};
class CCameraManagerDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01233BE4 slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    std::vector<long > CameraList;                     // +0x3C (Ego_r +0x40)
    std::vector<long > SpecialityCameraList;           // +0x48 (Ego_r +0x50)
    long InitialMode;                                  // +0x54 (Ego_r +0x60)
    long ZTargetMode;                                  // +0x58 (Ego_r +0x64)
    long ProjectileWeaponMode;                         // +0x5C (Ego_r +0x68)
    float BaseHeightToKeepAboveFlatLandscape;          // +0x60 (Ego_r +0x6C)
    float BaseHeightToKeepAboveWater;                  // +0x64 (Ego_r +0x70)
    float MaxHeightRelativeToGround;                   // +0x68 (Ego_r +0x74)
    float MaxHeightStepForPointAcceptance;             // +0x6C (Ego_r +0x78)
    float AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat; // +0x70 (Ego_r +0x7C)
};

void CCameraManagerDef::Transfer(CPersistContext& context)
{
    context.Transfer(CameraList);
    context.Transfer(SpecialityCameraList);
    context.Transfer(InitialMode);
    context.Transfer(ZTargetMode);
    context.Transfer(ProjectileWeaponMode);
    context.Transfer(BaseHeightToKeepAboveFlatLandscape);
    context.Transfer(BaseHeightToKeepAboveWater);
    context.Transfer(MaxHeightRelativeToGround);
    context.Transfer(MaxHeightStepForPointAcceptance);
    context.Transfer(AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat);
}