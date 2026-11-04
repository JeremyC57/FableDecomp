// NThingShot::CTCShotDefinitionDrainLife::SetSoulEffect(long effectType) -- create the soul effect thing at the shot's position; when it is alive keep it in PSoulEffect and attach its particle emitter to the shot's "weapon_pos_a" helper
// Leaf: ?SetSoulEffect@CTCShotDefinitionDrainLife@NThingShot@@QAEXJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BE05C, 157 bytes (`ret 4`; ebp frame); Ego_r.pdb ?SetSoulEffect@CTCShotDefinitionDrainLife@NThingShot@@QAEXJ@Z.
// Pragmas: optimize("y", off) + ("s", on), the size TU mode of the landed shot-definition rows.
// Callees: CThing::GetPos() const 0x004C73D0, static __fastcall CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880,
//   IS_THING_ALIVE<CThing*> 0x0040DCE0 (out of line, __fastcall), CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760,
//   ~CBaseIntelligentPointer 0x00A01C10, CThing::GetTCDriver<CTCDParticleEmitter> 0x004ACB50, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CTCDParticleEmitter::AttachToThing(CThing&, long, const CCharString&, long, float) 0x006E0BE0.
// Layout: PSoulEffect (CIntelligentPointer<CThing>) at retail +0xAC (Ego_r.pdb +0xB0).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/CIntelligentPointer.h"

class CTCDParticleEmitter : public CTCDriverBase {                  // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    static CThing* __fastcall Create(long type, const C3DVector& position, bool flag);    // 0x006E0880
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}

namespace NThingShot {
class CTCShotDefinitionDrainLife : public CTCBase {
public:
    void SetSoulEffect(long effectType);
private:
    unsigned char _shot0c[0xac - 0x0c];                             // CTCShotDefinition members, MaxCosAngleToTurn .. Speed
    CIntelligentPointer<CThing> PSoulEffect;                        // +0xac
};
}

void NThingShot::CTCShotDefinitionDrainLife::SetSoulEffect(long effectType)
{
    if (effectType) {
        CThing* effect = CTCDParticleEmitter::Create(effectType, GetParentThing().GetPos(), false);
        if (IS_THING_ALIVE(effect)) {
            {
                CIntelligentPointer<CThing> soulEffect(effect);
                PSoulEffect = soulEffect;
            }
            CTCDParticleEmitter* emitter = 0;
            effect->GetTCDriver(&emitter);
            CCharString helperName("weapon_pos_a");
            emitter->AttachToThing(GetParentThing(), 0x3c, helperName, 0, 0.0f);
        }
    }
}