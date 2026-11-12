// CTCTargeted::GetTargetPoint(long index, float radius) const -- the indexed targeting helper point of the owner's graphic appearance when there is one, else the generic fire point at the owner
// Leaf: GetTargetPoint
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EA490, length 116 bytes (`ret 0Ch`, C3DVector through the hidden pointer).
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560 (the global helper
// name @ 0x013BAC50, flags 0x20, 1.0f), CTCBase::PeekWorld @ 0x00686A80, NTargeting::GetTargetingPositionOnThing(const CWorld&,
// const CThing&, float) @ 0x007EA2F0 (`__fastcall`, hidden C3DVector result in ecx, world in edx, `ret 8`; manifest "Creature::GetTargetFirePoint").
// Layout: CThing::PTCGraphicAppearance at retail +0x64 (PDB +0x74). CTCTargeted per engine/CTCTargeted.h (members unused here).
// Shape: the appearance is a named local read before the pushes; the helper point is a local returned through the user copy ctor (`fld X ; fstp [eax]`, Y/Z dword moves); the fallback passes the
// hidden pointer straight to the helper (args right to left: radius, thing, then the GetWorld call).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAC50 TARGETING_CURSOR_POSITION_NAME (global, const; initializer 0x0121E700), was g_TargetHelperName.
#include "engine/C3DVector.h"
class CCharString { public: void* m_pData; };
extern const CCharString TARGETING_CURSOR_POSITION_NAME;          // 0x013BAC50
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& out, float scale) const;   // 0x004C0560 (Ego_r/FableWin ?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z)
};
}
class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; // +0x64 (PDB +0x74)
};
class CWorld;
namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCTargeted : public CTCBase { public:
    unsigned long TargetingFlags;               // +0x0c
    bool Targetable;                            // +0x10
    bool UseTargetGlow;                         // +0x11
    bool ValidTargetWithoutLOS;                 // +0x12
    void* PTargetingDef;                        // +0x14
    C3DVector GetTargetPoint(long index, float radius) const;
};

C3DVector CTCTargeted::GetTargetPoint(long index, float radius) const
{
    if (index >= 0) {
        NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
        C3DVector point;
        if (appearance->GetHelperPoint(TARGETING_CURSOR_POSITION_NAME, index, 0x20, point, 1.0f))
            return point;
    }
    const CThing& thing = *ParentThing;
    return NTargeting::GetTargetingPositionOnThing(PeekWorld(), thing, radius);
}