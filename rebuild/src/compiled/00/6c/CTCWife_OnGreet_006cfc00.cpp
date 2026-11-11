// CTCWife::OnGreet() -- once married, a greeting from the living hero posts opinion deed 0x50 at the wife's position
// Leaf: ?OnGreet@CTCWife@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CFC00, 140 bytes (+4 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCBase::GetPlayerManager() 0x00686D40; CPlayerManager::GetMainPlayer()
//          0x00449970; CPlayer::GetPControlledCreature() 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020;
//          CThing::GetPos() 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType, const C3DVector*, CThing*,
//          CThingAICreature*, EOpinionPostDeedType, bool, bool, CFaction*, bool, long) 0x006C6990.
// Shape: FrameGotMarriedToThePlayer (+0x20) must not be in the future; the hero's interface 0x51 (CTCHeroOpinionDeedLog,
//        bitset word 2 bit 17: `test dword ptr [eax+28h],20000h`) is fetched into a named local (esi) and, when the wife's
//        parent has bit 4 of its +0x6C byte set, PostDeed(0x50, &pos, parent, parent, 1, false, false, 0, false, 0).
// Layout: CTCWife ParentThing +0x04, FrameGotMarriedToThePlayer +0x20; retail CThing bitset +0x20, interface map +0x44,
//         Dead +0x91.
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
class CFaction;
class CThingAICreature;
#include "engine/EOpinionDeedType.h"
enum EOpinionPostDeedType { };                   // Ego_r.pdb enum (values used here by number)
class CTCHeroOpinionDeedLog { public:
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1);   // 0x006C6990 Ego_r.pdb / FableWin.pdb (engine/CTCHeroOpinionDeedLog.h)
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCWife : public CTCBase { public:
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[0x20 - 0x08];
    long FrameGotMarriedToThePlayer;             // +0x20
    void OnGreet();
};
void CTCWife::OnGreet()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetFrame())
        return;
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    CTCHeroOpinionDeedLog* deedLog;
    if (hero->GetTC(TCI_HERO_OPINION_DEED_LOG, &deedLog)) {
        if (ParentThing->GetTypeFlags() & 0x10)
            deedLog->PostDeed(OPINION_DEED_TYPE_WIFE_GREETED, ParentThing->GetPos(), ParentThing, (CThingAICreature*)ParentThing, (EOpinionPostDeedType)1, false, false, 0, false, 0);
    }
}