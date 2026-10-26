// CTCActionUseBlowHorn::vfunc_28(CThingCreatureBase*) -- run the base slot, then make a live creature blow this horn
// Leaf: vfunc_28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EB9B0, length 90 bytes. Slot 28 of the CTCActionUseBlowHorn
// vtable 0x0123F564 (rtti-vtables.tsv).
// Callees: CTCActionUseBase's slot 28 @ 0x007EAB90 (an empty `ret 4`, called non-virtually; manifest "$E2"),
// CCreatureAction_BlowHorn::CCreatureAction_BlowHorn(CThingCreatureBase*, CThing*) @ 0x00842EC0,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, CBaseIntelligentPointer::
// ~CBaseIntelligentPointer @ 0x00A01C10 (the action's PObjectToUse), CCreatureActionBase::~CCreatureActionBase
// @ 0x00693EF0 (it stores vtable 0x01259D24 = CCreatureActionBase; the manifest's
// "CPhysicsMeshInfo::~CPhysicsMeshInfo" is a propagated name).
//
// Layout: the retail action is 0xb0 bytes -- the 0xa8-byte CCreatureActionBase, then PObjectToUse
// (CIntelligentPointer<CThing>, PDB +0xb4) at +0xa8. The creature's dead flag is bit 0 of the byte at +0x91.
// Shape: the creature argument is held in esi across the base call; a null or dead creature does nothing;
// otherwise the action is a TEMPORARY in the call expression (address taken, VC extension C4238 -- as the
// landed 0x00920960) built in the 0xb0-byte frame and destroyed after the call through the derived
// class's implicit destructor (member pointer, then the base; no vtable store of its own). The horn
// (ParentThing) is read into a named local: retail loads it into edi, the dead `this` register
// (`mov edi,[edi+4] ; push edi`); read inline in the call it lands in eax.
// Header note: engine/CCreatureActionBase.h, CThingCreatureBase.h and CCreatureAction_BlowHorn.h hold the
// flattened PDB layouts only (the retail action is 0xc bytes narrower), so the action classes and the
// creature are modelled locally with a _Retail suffix.
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};

struct CCreatureActionBase_Retail {
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    ~CCreatureActionBase_Retail();                     // 0x00693EF0
};

struct CThingCreatureBase_Retail : CThing {
    bool SetCurrentAction(CCreatureActionBase_Retail* action);   // 0x006644F0
};

struct CBaseIntelligentPointer {                // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CCreatureAction_BlowHorn_Retail : CCreatureActionBase_Retail {
    CBaseIntelligentPointer PObjectToUse;       // +0xa8
    CCreatureAction_BlowHorn_Retail(CThingCreatureBase_Retail* creature, CThing* horn);   // 0x00842EC0
};

class CTCActionUseBase { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    void vfunc_28(CThingCreatureBase_Retail* creature);  // 0x007EAB90 (empty)
};

struct CTCActionUseBlowHorn : CTCActionUseBase {
    void vfunc_28(CThingCreatureBase_Retail* creature);
};

void CTCActionUseBlowHorn::vfunc_28(CThingCreatureBase_Retail* creature)
{
    CTCActionUseBase::vfunc_28(creature);
    if (creature != 0 && !(creature->m_flags91 & 1)) {
        CThing* horn = ParentThing;
        creature->SetCurrentAction(&CCreatureAction_BlowHorn_Retail(creature, horn));
    }
}