// CAIStateGroup_BanditKingLunge::IsRunnable() -- runnable when CanLungeAtPlayer(hero) holds and the hero is inside the lunge radius (squared XY distance < 16.0f @ 0x01238164): target the hero
// Leaf: IsRunnable@CAIStateGroup_BanditKingLunge
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009614A0, length 169 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::GetHeroThing @ 0x00487DC0, CAIStateGroup_BanditKingLunge::CanLungeAtPlayer(CThing*) @ 0x009613A0,
// CThing::GetPos @ 0x004C73D0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10.
// Shape: `dist2 < R` gives `fcomp ; test ah,5 ; jp`; PTargetThing = CIntelligentPointer<..>(hero) (vtable 0x01231C4C).
#include "engine/C3DVector.h"

#include "engine/CThing.h"
class CThingPlayerCreature : public CThing { };

class CPlayer
{ public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer    // CIntelligentPointer<CThingCreatureBase>, vtable 0x01231C4C
{
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem((CBaseObject*)item); }
};

class CAIBrain
{ public:
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

// squared XY distance between two positions (Y term evaluated first by VC7.1; arguments evaluate right to left,
// so `b` -- the creature -- is fetched first and kept in esi)
__forceinline float GetDistanceSquaredXY(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}

struct CAIStateGroup_BanditKingLunge
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14
    CPlayerManager* GetPPlayerManager();            // 0x008FCF90
    bool CanLungeAtPlayer(CThing* hero);            // 0x009613A0
    virtual bool IsRunnable();
};

bool CAIStateGroup_BanditKingLunge::IsRunnable()
{
    CThing* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();
    if (CanLungeAtPlayer(hero)
        && GetDistanceSquaredXY(hero->GetPos(), PBrain->Creature->GetPos()) < 16.0f)
    {
        PTargetThing.SetPItem((CBaseObject*)(((CThing*)CIntelligentPointerThing(hero).GetPItem())));
        return true;
    }
    return false;
}