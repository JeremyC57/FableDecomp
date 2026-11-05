// CTCWife::Died() -- a married wife's death unmarries her and sets the living hero's marriage state to 6 (widowed)
// Leaf: ?Died@CTCWife@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0020, 120 bytes (+8 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::Unmarry(bool) 0x006CFF10; CPlayerManager::GetMainPlayer()
//          0x00449970; CPlayer::GetPControlledCreature() 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020;
//          CTCHeroStats::SetMarriageState(CThingAICreature*, EMarriageState) 0x0057BC6E. Global CMainGameComponent::Get() 0x013B86A0.
// Shape: same guard as OnGreet; the player manager is read from the main game component's +0x1C (not through CTCBase);
//        interface 4 is CTCHeroStats (bitset byte +0x20 bit 4), called through the out-parameter lookup form.
// Layout: CTCWife ParentThing +0x04, FrameGotMarriedToThePlayer +0x20; CMainGameComponent PlayerManager +0x1C.
#include "engine/C3DVector.h"
class CTCBase;
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
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;               // +0x1c
};
class CTCHeroStats { public:
    void SetMarriageState(CThing* spouse, long state);   // 0x0057BC6E
};
class CTCWife { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[0x20 - 0x08];
    long FrameGotMarriedToThePlayer;             // +0x20
    void Unmarry(bool notify);                   // 0x006CFF10
    void Died();
};
void CTCWife::Died()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetFrame())
        return;
    Unmarry(true);
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    CTCHeroStats* stats;
    if (hero->GetTC(TCI_HERO_STATS, &stats))
        stats->SetMarriageState(ParentThing, 6);
}