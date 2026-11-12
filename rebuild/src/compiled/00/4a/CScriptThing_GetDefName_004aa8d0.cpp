// CScriptThing::GetDefName() -- the pointee's DefName string, or a copy of the static empty string when there is no pointee
// Leaf: GetDefName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA8D0, length 42 bytes (`?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ`, a const virtual).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30; virtual slot 2 of the pointee.
// CScriptThing is a pimpl handle {vptr, CCountedPointer<CScriptThing> PImp}: every query forwards to the
// implementation object at PImp_Data (+0x04) through its vtable. engine/CScriptThing.h gives the
// handle layout; the pointee's vtable is reproduced through a local slot view cast from PImp_Data
// (the landed GetPos 0x004AA980 / GetCurrentMapName 0x004AA9E0 siblings use the same model).
// Shape: `PImp_Data` loaded straight into ecx and null-tested; the hidden result pointer
// read into esi once for both arms. Null arm: the out-of-line CCharString copy constructor
// @ 0x0099EC30 builds the result from the global; else the virtual (slot 2, `call [eax+8h]`)
// writes the result through the forwarded hidden pointer.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09), reached through the hdr opt-in
//   FABLE_RETAIL_CSCRIPTTHING_NULLSTRING on the flattened engine/CScriptThing.h; was g_EmptyString.
#define FABLE_RETAIL_CSCRIPTTHING_NULLSTRING   // hdr overlay (agentNF26)
#include "engine/CScriptThing.h"

class CCharString { public:
    char* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30 (out of line in retail; fable_string.h would inline it)
};

struct CScriptThingVirtuals {                       // vtable view of the implementation object
    virtual void v0();
    virtual void v1();
    virtual CCharString GetDefName() const;                 // slot 2 (+0x08)
};

struct CScriptThing_Methods : CScriptThing {
    CCharString GetDefName() const;
};

CCharString CScriptThing_Methods::GetDefName() const
{
    const CScriptThingVirtuals* imp = (const CScriptThingVirtuals*)PImp_Data;
    if (imp == 0)
        return NullString;
    return imp->GetDefName();
}