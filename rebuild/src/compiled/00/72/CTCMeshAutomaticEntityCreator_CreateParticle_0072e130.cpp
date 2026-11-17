// CTCMeshAutomaticEntityCreator::CreateParticle(const CCharString& dummy_name, const CCharString& particle_name, NGraphicAppearance::CTCGraphicAppearance* appearance) -- for every dummy object of that name on the appearance, create the particle at the owner, attach it to that dummy, mark it created by its owner, record the creator and register it with the owner
// Leaf: ?CreateParticle@CTCMeshAutomaticEntityCreator@@AAEXABVCCharString@@0PAVCTCGraphicAppearance@NGraphicAppearance@@@Z
// Name: Ego_r.pdb / FableWin.pdb ?CreateParticle@CTCMeshAutomaticEntityCreator@@AAEXABVCCharString@@0PAVCTCGraphicAppearance@NGraphicAppearance@@@Z (private).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072E130, 352 bytes (+ int3 padding), `ret 0Ch`. Speed TU (esp frame,
//   `lea ecx,[ecx]` loop alignment); the 8-byte intelligent-pointer temporary built inside the loop gives the
//   `and esp,-8` aligned frame (GOTCHAS chunk 232).
// Callees: ?GetNoDummyObjects@CTCGraphicAppearance@NGraphicAppearance@@QBEJABVCCharString@@M@Z @ 0x004BE000 (re-evaluated per pass),
//   CThing::GetPos @ 0x004C73D0, ?Create@CTCDParticleEmitter@@SIPAVCThing@@ABVCCharString@@ABVC3DVector@@_N@Z @ 0x006E0970,
//   CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, CCharString(const char*, long = -1) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//   CThing::AddTC @ 0x004C9D60, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 /
//   dtor @ 0x00A01C10 (the inline CTCCreatedEntity::SetPCreator), CTCOwnedEntity::SetPOwner @ 0x007E8590 (through the inlined
//   same-TU ?AddThingToOwnerTC@CTCMeshAutomaticEntityCreator@@AAEXAAVCThing@@@Z, landed 0x0072DD60).
// Data: "CTCCreatedEntity" @ 0x0123AA60, "CTCOwnedEntity" @ 0x0123AA74; CIntelligentPointer<const CThing> vtable @ 0x01237F1C.
// Layout: CThing (shared header): interface 0x4B = TCI_PARTICLE_EMITTER read through the cached driver +0x5C (GetTCDriver),
//   Dead / CreatedByOwnerThingCreation bits of +0x91; CTCCreatedEntity PCreator +0x0C; CTCBase ParentThing +0x04.
//   CTCMeshAutomaticEntityCreator has no shared header: the local class carries only the two private members this row needs.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_CREATEDBYOWNER
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_NODUMMYOBJECTS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCCreatedEntity.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

class CTCMeshAutomaticEntityCreator : public CTCBase {
private:
    void AddThingToOwnerTC(CThing& thing)                                // 0x0072DD60 (same TU, inlined here)
    {
        CTCOwnedEntity* owned = static_cast<CTCOwnedEntity*>(thing.AddTC("CTCOwnedEntity", false, 0));
        owned->SetPOwner(ParentThing);
    }
    void CreateParticle(const CCharString& dummy_name, const CCharString& particle_name,
                        NGraphicAppearance::CTCGraphicAppearance* appearance);
};

void CTCMeshAutomaticEntityCreator::CreateParticle(const CCharString& dummy_name, const CCharString& particle_name,
                                                   NGraphicAppearance::CTCGraphicAppearance* appearance)
{
    for (long i = 0; i < appearance->GetNoDummyObjects(dummy_name, 1.0f); ++i) {
        CThing* particle = CTCDParticleEmitter::Create(particle_name, ParentThing->GetPos(), false);
        if (particle && !particle->IsDead()) {
            CTCDParticleEmitter* emitter = 0;
            particle->GetTCDriver(&emitter);
            emitter->AttachToThing(*ParentThing, 10, dummy_name, i, 0.0f);
            particle->SetAsCreatedByOwnerThingCreation(true);
            CTCCreatedEntity* created = static_cast<CTCCreatedEntity*>(particle->AddTC("CTCCreatedEntity", false, 0));
            created->SetPCreator(*ParentThing);
            AddThingToOwnerTC(*particle);
        }
    }
}
