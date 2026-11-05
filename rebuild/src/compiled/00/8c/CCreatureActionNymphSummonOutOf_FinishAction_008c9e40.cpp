// Byte-exact reconstruction of
// Leaf: ?CCreatureAction_NymphSummonOutOf_FinishAction@@YIXPAVCCreatureAction_NymphSummonOutOf@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// ?FinishAction@CCreatureAction_NymphSummonOutOf@@UAEXXZ  @ 0x008c9e40

// The invoked object at [Inner+0x60]. Its used method is the 37th virtual (byte
// offset 0x90 = index 36). Declaring 36 leading virtuals + the real one makes VC71
// emit a proper __fastcall virtual call (this in ecx, one arg pushed).
#include "engine/CCreatureAction_NymphSummonOutOf.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Target {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void slot36(int a);   // vtable byte offset 0x90
};

struct Inner {
    char pad[0x60];
    Target* pField60;   // [Inner + 0x60]
};

// callee at 0xa01b50: __fastcall(ecx = self+8) -> Inner*


// base method tail-jumped to: 0x859a20, __fastcall(self)
extern void __fastcall base_FinishAction_859a20(CCreatureAction_NymphSummonOutOf* action);

void __fastcall CCreatureAction_NymphSummonOutOf_FinishAction(CCreatureAction_NymphSummonOutOf* action)
{
    Inner* p = ((Inner*)(((CBaseIntelligentPointer*)((char*)action + 8))->GetPItem()));
    Target* h = p->pField60;      // ecx = [eax+0x60]; slot fetched from [ecx]
    h->slot36(0);                 // push 0; call [eax+0x90]
    base_FinishAction_859a20(action);
}