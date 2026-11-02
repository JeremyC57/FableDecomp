// CCreatureAction_SpiritAttack::FinishAction() -- if the spirit attack's particle emitter thing is still alive, kill it; then the base action's FinishAction
// Leaf: ?FinishAction@CCreatureAction_SpiritAttack@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008547D0, 62 bytes (int3 padding 0x0085480E..F; the next function 0x00854810 is
//   CCreatureAction_SpiritHitResponse::Initialise), thiscall, tail `jmp CCreatureActionBase::FinishAction` 0x00694430.
// Identity (proven, two sources): (1) the only reference is slot 4 (the FinishAction slot: CCreatureActionBase 0x01259D24 slot 4
//   = 0x00694430 ?FinishAction@CCreatureActionBase@@UAEXXZ, this row's tail) of vtable 0x01275D74, RTTI
//   .?AVCCreatureAction_SpiritAttack@@ : CCreatureActionBase : CBaseObject : CBaseClass; (2) Ego_r.pdb and FableWin.pdb both
//   spell ?FinishAction@CCreatureAction_SpiritAttack@@EAEXXZ -- a PRIVATE virtual override (`E`), as the landed sibling
//   ?FrameUpdate@CCreatureAction_SpiritAttack@@EAEXXZ 0x00854B90; the stand-in's EAE access is right.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ, both
//   PDBs; three calls through the inline CIntelligentPointer<CThing> accessors), CThing::Kill(bool) 0x004C9B80
//   (?Kill@CThing@@QAEX_N@Z), CCreatureActionBase::FinishAction() 0x00694430.
// Shape: `if (IS_THING_ALIVE(PParticleEmitter)) PParticleEmitter->Kill(true);` -- the PDB template
//   ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z (inline in release; its body
//   is not in either PDB: written over the PDB accessors operator* / operator-> and the inline ?IsDead@CThing@@QBE_NXZ, the
//   test the landed CCombatActionBase::FinishAction 0x00859A20 and ~CModeOutOfActionBase 0x008D7580 expand identically).
// Layout: Ego_r CCreatureAction_SpiritAttack PTarget +180 / PParticleEmitter +188 over a 0xB4 CCreatureActionBase; retail
//   CCreatureActionBase is 0xA8 (12-byte STLport containers), so PTarget +0xA8 / PParticleEmitter +0xB0 -- the `lea esi,[edi+0B0h]`
//   and engine/CCreatureAction_SpiritAttack.h's retail offsets. That header and engine/CCreatureActionBase.h are flattened (vptr as
//   data), so the action classes are restated over engine/CBaseObject.h (FABLE_RETAIL_BASE_OBJECT); CThing : CClass : CBaseObject
//   as the landed CCreatureAction_LeapLong::FinishAction 0x008B74A0 (CClass through the layout-rename include), Dead = bit 0 of +0x91.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
};

class CThing : public CClass { public:                  // RTTI CThing : CClass
    unsigned char _pad14[0x91 - 0x14];
    unsigned char Dead : 1;                             // +0x91 bit 0 (Ego_r CThing::Dead)
    bool IsDead() const { return Dead; }                // FableWin ?IsDead@CThing@@QBE_NXZ (inline in release)
    void Kill(bool immediately);                        // 0x004C9B80
};

#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes; 1-aligned in retail)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // FableWin member set: Get / Set / -> / *
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};
#pragma pack(pop)

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}

class CCreatureActionBase : public CBaseObject {        // vtable 0x01259D24, retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
    virtual void FinishAction();                        // slot 4: 0x00694430
    unsigned char _pad08[0xa8 - 0x08];                  // +0x08 PCreature .. (not touched here)
};

class CCreatureAction_SpiritAttack : public CCreatureActionBase {    // vtable 0x01275D74
    virtual void FinishAction();                        // private (Ego_r / FableWin EAE)
public:
    CIntelligentPointer<CThing> PTarget;                // +0xA8
    CIntelligentPointer<CThing> PParticleEmitter;       // +0xB0
};

void CCreatureAction_SpiritAttack::FinishAction()
{
    if (IS_THING_ALIVE(PParticleEmitter))
        PParticleEmitter->Kill(true);
    CCreatureActionBase::FinishAction();
}