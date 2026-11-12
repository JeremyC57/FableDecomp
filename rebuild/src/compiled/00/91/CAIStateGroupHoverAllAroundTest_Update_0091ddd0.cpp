// CAIStateGroup_HoverAllAroundTest::Update()
// Grade: RELOCATION_MATCH (626 bytes). Loop exit is `if (Move == 2) continue; Terminate(); return;` -- the return (not break) changes block weights so the player IP is ranked above the frac spill slot.
// What it does: test state -- while the player creature lives, hover to a random point on a vertical circle (radius
//   GFFloatRandom(10)) around the start position, in the plane spanned by the up axis and the horizontal perpendicular to the
//   creature->player direction; the point is clamped to at least ground height; terminate when a move does not report 2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091DDD0, 626 bytes, vtable 0x0128A2C0 slot 12.
// Callees: CAIStateGroupBase::GetWorld 0x008FCFC0, CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPControlledCreature 0x00487DC0,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CThing::GetPos 0x004C73D0,
//   C3DVector::AccurateNormalise 0x00A14440, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890, _CIfmod 0x00BFEB28,
//   GFFloatRandom 0x004978A0, CAIStateGroupBase::PeekWorldMap 0x008FCFF0, CWorldMap::GetGroundSizeZAt 0x0063A300,
//   CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00, CAIStateGroupBase::Terminate 0x008FCF40. Inline: GFSin/GFCos (turns -> radians).
// RandomFractionInline is a DESCRIPTIVE made-up name (the first random is inlined, the second GFFloatRandom(10) is a call).
// Leaf: ?Update@CAIStateGroup_HoverAllAroundTest@@UAEXXZ
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#include <math.h>
#pragma intrinsic(fmod, sin, cos)
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CWorldMap.h"
long __fastcall GFROR13(unsigned long value);               // 0x00497890
class CThingPlayerCreature;
#include "engine/GFFloatRandom.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
inline float RandomFractionInline(unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return (float)(fmod((double)*seed, 65536.0) / 65536.0);
}
class CAIStateGroup_HoverAllAroundTest : public CAIStateGroupBase { public:
    virtual void Update();
};
void CAIStateGroup_HoverAllAroundTest::Update()
{
    CIntelligentPointer<CThing> player((CThing*)GetWorld().PeekMyPlayer()->GetPControlledCreature());
    C3DVector start = GetCreature().GetPos();
    while (player.Get() != 0 && !player.Get()->IsDead()) {
        const CThing* me = &GetCreature();
        const CThing* them = player.Get();
        const C3DVector& mp = me->GetPos();
        const C3DVector& tp = them->GetPos();
        C3DVector dir(tp.X - mp.X, tp.Y - mp.Y, 0.0f);
        dir.AccurateNormalise();
        C3DVector perp(dir.Y, -dir.X, 0.0f);
        float frac = RandomFractionInline(GetPWorldSeed());
        float dist = GFFloatRandom(10.0f, GetPWorldSeed());
        C3DVector target = start + C3DVector(0.0f, 0.0f, 1.0f) * (float)GFSin(frac) * dist + perp * (float)GFCos(frac) * dist;
        float ground = PeekWorldMap().GetGroundSizeZAt(target);
        if (target.Z < ground)
            target.Z = ground;
        bool moving;
        if (AIAction_MoveToPos((EAIActionRepeat)1, target, moving, (EMovementType)3, true, true, 0, true, 0.0f) == 2)
            continue;
        Terminate();
        return;
    }
}