// CTCTargetingBase::SetPNearestInterestingThing(CThing* thing) -- store the thing in PNearestInterestingThing through a temporary CIntelligentPointer<CThing> (converting constructor, then the pointer assignment)
// Leaf: ?SetPNearestInterestingThing@CTCTargetingBase@@IAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5810, 71 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?SetPNearestInterestingThing@CTCTargetingBase@@IAEXPAVCThing@@@Z (donor 0x007F0D80) is
//   followed 0x50 bytes later by GetPNearestInterestingThing (0x007F0DD0) and then SetPScriptOverrideInterestingThing
//   (0x007F0E00); retail has the same run: this row, GetPNearestInterestingThing 0x007F5860 (+0x50), then the landed
//   SetPScriptOverrideInterestingThing 0x007F5890. FableWin.pdb has the same spelling (0x02533F80). Callers: the landed
//   CTCTargetingBase::UpdateNearestInterestingThing 0x007F5AE0 (twice) and CTCTargetingPlayer's override 0x008023E0 (twice,
//   landed, already naming this address). Ego_r.pdb CTCTargetingBase: PNearestInterestingThing (CIntelligentPointer<CThing>)
//   at +0x0D, the `lea ecx,[esi+0Dh]` here. The catalog label "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90 (twice), CBaseIntelligentPointer::GetPItem() const 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10.
//   Vtable 0x01238C6C = RTTI .?AV?$CIntelligentPointer@VCThing@@@@ (the temporary's own vtable, stored by its constructor).
// Shape: `PNearestInterestingThing = thing;` -- the PDB's converting constructor CIntelligentPointer(T*) and
//   operator=(const CIntelligentPointer&) (both in Ego_r.pdb) inline: base ctor, vtable store, SetPItem; then
//   member.SetPItem(temp.GetPItem()); CIntelligentPointer<T> declares no destructor, so the temporary dies through the
//   base's out-of-line destructor (no vtable reset).
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
    CThing* GetPNearestInterestingThing() const;            // 0x007F5860
    void SetPScriptOverrideInterestingThing(CThing* thing); // 0x007F5890
protected:
    void SetPNearestInterestingThing(CThing* thing);
public:
    bool                        ScriptOverrideIgnoreEverything;     // +0x0c
    CIntelligentPointer<CThing> PNearestInterestingThing;           // +0x0d
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;    // +0x15
    CIntelligentPointer<CThing> PScriptOverrideRangedTarget;        // +0x1d
    CIntelligentPointer<CThing> PTargetedThing;                     // +0x25
};

void CTCTargetingBase::SetPNearestInterestingThing(CThing* thing)
{
    PNearestInterestingThing = thing;
}