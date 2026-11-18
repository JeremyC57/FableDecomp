// NCreatureMode::CModeBriarRoseConcentrate::OnDeactivate -- resume the AI (an AI creature owner), hit every surviving creature imposter
//   (remove its TCI_ABSORB_HITS component, then OnHit with 10 melee damage from the owner), kill the body / sword effect things, deactivate.
// Leaf: ?OnDeactivate@CModeBriarRoseConcentrate@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7E70, 269 bytes. Callees: CThingAICreature::SuspendAI 0x00833A10, CHitParameters ctor 0x007A3BC0 /
//   AddDamageWithAttribute 0x00868CA0 / SetThingHitting 0x008685F0 / dtor 0x00789358, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CThing::RemoveTC 0x004C9840, CThingCreatureBase::OnHit (vtable +0x104, slot 65), CThing::Kill 0x004C9B80, CModeStandBase::OnDeactivate 0x00861D00.
// Names: Ego_r.pdb 0x00830FE0 / FableWin.pdb 0x0280C140 ?OnDeactivate@CModeBriarRoseConcentrate@NCreatureMode@@UAEXXZ; members from
//   engine/NCreatureMode_CModeBriarRoseConcentrate.h (Imposters +0x80, PBodyEffect +0x94, PSwordEffect +0x9C); the base call is Ego_r / FableWin
//   ?OnDeactivate@CModeStandBase@@UAEXXZ (global CModeStandBase).
// Port (NF86, q11099 binder; was the all-local ModeBriarConcentrate / ThingNative / HitParameters model under a #define CThing CThingDonor
//   quarantine include, leaf ?OnDeactivate@ModeBriarConcentrate@@QAEXXZ): the shared headers, as the sibling rows 0x008E7F80 / 0x008E7850 and the
//   OnHit callers 0x007A1B20 spell them (FABLE_RETAIL_CTHING_ORIGINAL_VTABLE for the OnHit slot, FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE for the base).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHitParameters.h"
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
#include "engine/ThingAlive.h"

void NCreatureMode::CModeBriarRoseConcentrate::OnDeactivate()
{
    if (ParentThing.GetType() == 1)
        ((CThingAICreature*)&ParentThing)->SuspendAI(false, this);
    CHitParameters hit_params;
    hit_params.AddDamageWithAttribute(DAMAGE_MELEE, 10.0f);
    hit_params.SetThingHitting(ParentThing);
    for (std::vector<CIntelligentPointer<CThing> >::iterator it = Imposters.begin(); it != Imposters.end(); ++it) {
        CThing* imposter = it->Get();
        if (imposter && !imposter->IsDead() && (imposter->GetTypeFlags() & 8)) {
            imposter->RemoveTC(TCI_ABSORB_HITS);
            static_cast<CThingCreatureBase*>(imposter)->OnHit(hit_params);
        }
    }
    if (IS_THING_ALIVE(PBodyEffect))
        PBodyEffect->Kill(true);
    if (IS_THING_ALIVE(PSwordEffect))
        PSwordEffect->Kill(true);
    CModeStandBase::OnDeactivate();
}
