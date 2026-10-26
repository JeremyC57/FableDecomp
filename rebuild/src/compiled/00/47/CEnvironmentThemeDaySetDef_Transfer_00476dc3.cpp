// CEnvironmentThemeDaySetDef::Transfer(CPersistContext& context) -- persist the CEnvironmentThemeDaySetDef's fields (10 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CEnvironmentThemeDaySetDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00476DC3, 123 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CEnvironmentThemeDaySetDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123360C, whose RTTI type descriptor is .?AVCEnvironmentThemeDaySetDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CEnvironmentThemeDaySetDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CEnvironmentThemeDaySetDef@@UAEXAAVCPersistContext@@@Z" agrees; family H058 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(CArray<CEnvironmentThemeDef >&) 0x00476E3E, Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(CRGBColour&) 0x004595A3.
// Layout: Ego_r.pdb CEnvironmentThemeDaySetDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
//   engine/CEnvironmentThemeDaySetDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
template <class T> class CArray {                                       // 12 bytes in retail (16 in Ego_r.pdb)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
class CEnvironmentThemeDef {                                       // Ego_r.pdb, 200 bytes (opaque here)
public:
    unsigned char _body[200];
};
class CRGBColour {                                                 // Ego_r.pdb, 4 bytes (opaque here)
public:
    unsigned char _body[4];
};
class CPersistContext {
public:
    void Transfer(CArray<CEnvironmentThemeDef >& value);         // 0x00476E3E
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(CRGBColour& value);                            // 0x004595A3
};
class CEnvironmentThemeDaySetDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123360C slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    CArray<CEnvironmentThemeDef > TimeTheme;           // +0x3C (Ego_r +0x40)
    float SunTilt;                                     // +0x48 (Ego_r +0x50)
    float SunRotate;                                   // +0x4C (Ego_r +0x54)
    float SunHeight;                                   // +0x50 (Ego_r +0x58)
    float MoonTilt;                                    // +0x54 (Ego_r +0x5C)
    float MoonRotate;                                  // +0x58 (Ego_r +0x60)
    float MoonHeight;                                  // +0x5C (Ego_r +0x64)
    long ColourLookupColumn;                           // +0x60 (Ego_r +0x68)
    CRGBColour EditorColour;                           // +0x64 (Ego_r +0x6C)
    float FishWeightMult;                              // +0x68 (Ego_r +0x70)
};

void CEnvironmentThemeDaySetDef::Transfer(CPersistContext& context)
{
    context.Transfer(TimeTheme);
    context.Transfer(SunTilt);
    context.Transfer(SunRotate);
    context.Transfer(SunHeight);
    context.Transfer(MoonTilt);
    context.Transfer(MoonRotate);
    context.Transfer(MoonHeight);
    context.Transfer(ColourLookupColumn);
    context.Transfer(EditorColour);
    context.Transfer(FishWeightMult);
}