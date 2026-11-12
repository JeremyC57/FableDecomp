// CTCFishingSpot::FrameUpdate() -- while enabled, (re)create the two ripple particles and the underwater fish particle and attach them to the spot; otherwise remove them
// Leaf: ?FrameUpdate@CTCFishingSpot@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B0360, 773 bytes (plain `ret`). CTCFishingSpot vtable 0x0126D51C slot 9; Ego_r.pdb / FableWin.pdb
//   ?FrameUpdate@CTCFishingSpot@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThing::GetPos 0x004C73D0, CTCDParticleEmitter::Create(const CCharString&, const C3DVector&,
//   bool) 0x006E0970 (__fastcall), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetTC, interface 0x4b),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, ::SetAlwaysUpdate 0x006E0590, CTCFishingSpot::RemoveParticles 0x007B01C0.
// Layout: Ego_r.pdb CTCFishingSpot (0x28): PParticle1 +0x0c, PParticle2 +0x14, PParticleFish +0x1c, Enabled +0x24 (landed ctor 0x007B0300 /
//   RemoveParticles 0x007B01C0). No shared CTCFishingSpot header: the class is modelled here on the shared CTCBase / CThing /
//   CIntelligentPointer / CTCDParticleEmitter headers.
// Shape: one FABLE_FORCEINLINE per-particle member helper expanded three times; the member address lives in esi, the created thing / emitter in ebx.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCDParticleEmitter.h"

class CTCFishingSpot : public CTCBase {                     // vtable 0x0126D51C
public:
    virtual void FrameUpdate();                             // slot 9
protected:
    void RemoveParticles();                                 // 0x007B01C0
private:
    FABLE_FORCEINLINE void CreateParticle(CIntelligentPointer<CThing>& particle, const char* name, long attachType, float offset)
    {
        if (!particle.GetPItem()) {
            particle = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(name, GetParentThing().GetPos(), false));
            if (particle.GetPItem()) {
                CTCDParticleEmitter* emitter = 0;
                CThing* thing = particle.Get();
                if (thing->HasTC(TCI_PARTICLE_EMITTER))
                    emitter = (CTCDParticleEmitter*)&thing->GetTC(TCI_PARTICLE_EMITTER);
                emitter->AttachToThing(GetParentThing(), attachType, "", 0, offset);
                emitter->SetAlwaysUpdate(true);
            }
        }
    }
    CIntelligentPointer<CThing> PParticle1;                 // +0x0c
    CIntelligentPointer<CThing> PParticle2;                 // +0x14
    CIntelligentPointer<CThing> PParticleFish;              // +0x1c
    bool                        Enabled;                    // +0x24
};

void CTCFishingSpot::FrameUpdate()
{
    if (Enabled) {
        CreateParticle(PParticle1, "FISHING_SPOT_01", 0x40, 0.0f);
        CreateParticle(PParticle2, "FISHING_SPOT_02", 0x40, 0.0f);
        CreateParticle(PParticleFish, "UNDERWATER_FISH_01", 0x80, -1.0f);
    } else {
        RemoveParticles();
    }
}