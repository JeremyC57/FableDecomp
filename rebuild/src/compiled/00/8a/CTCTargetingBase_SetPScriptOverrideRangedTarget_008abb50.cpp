// CTCTargetingBase::SetPScriptOverrideRangedTarget(CThing* thing) -- store the thing in PScriptOverrideRangedTarget through a temporary CIntelligentPointer<CThing> (converting constructor, then the pointer assignment)
// Leaf: ?SetPScriptOverrideRangedTarget@CTCTargetingBase@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ABB50, 71 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?SetPScriptOverrideRangedTarget@CTCTargetingBase@@QAEXPAVCThing@@@Z (donor 0x008E9A40) is
//   followed 0x50 bytes later by ?SetScriptTargetOverride@CTCTargetingAI@@QAEXPAVCThingCreatureBase@@@Z (0x008E9A90);
//   retail has the same pair: this row, then 0x008ABBA0 (+0x50, the same shape over CIntelligentPointer<CThingCreatureBase>
//   vtable 0x0123E7D8 at +0x40 = Ego_r CTCTargetingAI::PScriptTargetOverride). FableWin.pdb places the same spelling
//   (0x02AA97B0) right after CGameScriptInterface::EntitySetRangedTarget (0x02AA95F0); retail's callers are exactly
//   EntitySetRangedTarget 0x008A04C0 (twice) and EntityClearRangedTarget 0x008A0540 (tail jump with 0; landed, already
//   naming this address). Ego_r.pdb CTCTargetingBase: PScriptOverrideRangedTarget (CIntelligentPointer<CThing>) at +0x1D,
//   the `lea ecx,[esi+1Dh]` here. The catalog label "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90 (twice), CBaseIntelligentPointer::GetPItem() const 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10.
//   Vtable 0x01238C6C = RTTI .?AV?$CIntelligentPointer@VCThing@@@@ (the temporary's own vtable, stored by its constructor).
// Shape: the 0x007F5810 SetPNearestInterestingThing body on the next member: `PScriptOverrideRangedTarget = thing;` -- the
//   PDB's converting constructor CIntelligentPointer(T*) and operator=(const CIntelligentPointer&) inline; the temporary
//   dies through the base's out-of-line destructor (CIntelligentPointer<T> declares none).
#include "engine/CTCBase.h"                                 // CTCBase (flattened vptr, 0xc bytes)

class CBaseObject;
class CBaseObjectPointer;
#pragma pack(push, 1)                                       // the pointer classes are byte-aligned (PTarget-style +0x0d / +0x7d members)
class CBaseIntelligentPointer {                             // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
    T* Get() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
#pragma pack(pop)

class CTCTargetingBase : public CTCBase {                   // Ego_r.pdb, 0x38 bytes (byte-aligned pointer members)
public:
    void SetPScriptOverrideRangedTarget(CThing* thing);

    bool                        ScriptOverrideIgnoreEverything;     // +0x0c
    CIntelligentPointer<CThing> PNearestInterestingThing;           // +0x0d
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;    // +0x15
    CIntelligentPointer<CThing> PScriptOverrideRangedTarget;        // +0x1d
    CIntelligentPointer<CThing> PTargetedThing;                     // +0x25
};

void CTCTargetingBase::SetPScriptOverrideRangedTarget(CThing* thing)
{
    PScriptOverrideRangedTarget = thing;
}