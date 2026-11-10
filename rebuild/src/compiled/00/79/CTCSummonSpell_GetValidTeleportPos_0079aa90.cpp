// CTCSummonSpell::GetValidTeleportPos(const CThing* thing) -- find where to put the summoned thing: its own position if valid, else points on rings around the caster (the caster-to-thing direction rotated in 1/36-turn steps, the ring radius growing by the original distance each full turn); after ten more failures settle for the rotated direction at the original distance
// Leaf: ?GetValidTeleportPos@CTCSummonSpell@@AAE?AVC3DVector@@PBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079AA90, 557 bytes, thiscall `ret 8` (hidden result pointer, thing). Caller: CTCSummonSpell::CreateMinion
//   (0x0079ACC0).
// Identity: Ego_r.pdb ?GetValidTeleportPos@CTCSummonSpell@@AAE?AVC3DVector@@PBVCThing@@@Z (Ego_r 0x007A05C0, after IsValidTeleportPos
//   0x007A0360 as retail 0x0079A890 / 0x0079AA90); the catalog's "CTCSummonSpell_GetValidSummonPos" is descriptive.
// Callees: CThing::GetPos 0x004C73D0, CTCBase::GetWorld 0x00686A70, CWorld::PeekWorldMap (vtable slot 13, `call [edx+34h]`),
//   CTCSummonSpell::IsValidTeleportPos(const C3DVector&, const CWorldMap&, const CThing*) 0x0079A890, C3DVector::RotateXY(float) 0x00A14040.
// Globals (pool): 1.0f 0x0122DED8, 1/36 0x0126C324.
// Shape: the accurate length sqrt(Z*Z + Y*Y + X*X) is homed in the dead `thing` slot ([esp+5Ch]) as the growing radius, its copy (the
//   fixed distance) in the reciprocal's slot [esp+14h]. `angle` is declared after the length: declared before it, its zero store
//   (0x0079AAB7) moves the x87 scheduling region so that dir.Z is spilled with `fstp ; fld` instead of retail's `fst [esp+2Ch] ; fmul
//   [esp+2Ch]` (0x0079AAC8).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CTCSUMMONSPELL
#define FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CTCSummonSpell.h"

C3DVector CTCSummonSpell::GetValidTeleportPos(const CThing* thing)
{
    C3DVector direction = thing->GetPos() - GetParentThing().GetPos();
    float radius = direction.GetMagnitude();
    float angle = 0.0f;
    direction /= radius;
    float distance = radius;
    const CWorldMap& world_map = GetWorld().PeekWorldMap();
    C3DVector pos = thing->GetPos();
    long tries = 0;
    while (!IsValidTeleportPos(pos, world_map, thing)) {
        angle += 1.0f / 36.0f;
        if (angle >= 1.0f) {
            angle = 0.0f;
            radius += distance;
        }
        C3DVector rotated = direction;
        rotated.RotateXY(angle);
        pos = GetParentThing().GetPos() + rotated * radius;
        if (++tries > 10) {
            pos = GetParentThing().GetPos() + rotated * distance;
            break;
        }
    }
    return pos;
}