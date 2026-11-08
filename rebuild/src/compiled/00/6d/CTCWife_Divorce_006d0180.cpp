// CTCWife::Divorce() -- once married, divorce the hero: unmarry, mark divorced, set the wife's divorced opinion masks and personality, reset the hero's marriage state and morality, sour the opinion of whoever is at the marital home and refresh the player GUI
// Leaf: ?Divorce@CTCWife@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_FrameRate (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0180, 598 bytes (+ int3 padding), plain `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::Unmarry(bool) 0x006CFF10; LowerBound 0x0040F020;
//   CTCCreatureOpinionOfHero::SetOpinionReactionMask 0x00681E30 / SetOpinionDeedMask 0x006822E0 / SetPersonalityOverride
//   0x006820E0 / ClearAllKnownDeeds 0x00681A70 / ApplyOpinionTransientOffset 0x00682140; CTCBase::GetWorld 0x00686A70 (on the opinion
//   TC) / CTCBase::GetPlayerManager 0x00686D40; CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0;
//   CTCHeroStats::SetMarriageState 0x0057BC6E / GetMoralityMax 0x0057A67E / SetMorality 0x0057C2E6; the home's
//   occupant getter 0x007EE540; the player GUI refresh 0x004378C0 on (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790; CCharString 0x0099EBF0 /
//   ~CCharString 0x0099EAE0; frame rate 0x01375550.
// Layout (retail CTCWife = Ego_r.pdb less 4 after the 12-byte GiftsReceived map): ParentThing +0x04, PHeroMarriageDef +0x0c,
//   FrameGotMarriedToThePlayer +0x20, DivorcedHero +0x24, LoveAttitudeValue +0x8c; CHeroMarriageDef
//   HeroStatMoralityInOpinionFormChangePerDivorce +0xac; CTCCreatureOpinionOfHero LastOpinionReactionFrame +0x0c,
//   HeroOpinionEnemy +0x342; CTCHeroStats Morality +0x28.
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    unsigned char _pad92[0x1c9 - 0x92];
    unsigned char Flags1C9;                     // +0x1c9
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCCreatureOpinionOfHero { public:
    unsigned char _pad00[0x0c];
    long LastOpinionReactionFrame;              // +0x0c
    unsigned char _pad10[0x342 - 0x10];
    bool HeroOpinionEnemy;                      // +0x342
    CWorld* GetWorld();                         // 0x00686A70
    void SetOpinionReactionMask(const CCharString& name);   // 0x00681E30
    void SetOpinionDeedMask(const CCharString& name);       // 0x006822E0
    void SetPersonalityOverride(const CCharString& name, bool force);   // 0x006820E0
    void ClearAllKnownDeeds();                  // 0x00681A70
    void ApplyOpinionTransientOffset(long type, float amount, long a, long frames, float decay, bool ignorePersonality);   // 0x00682140 (6th parameter bool: retail tests it as a byte)
};
class CTCHeroStats { public:
    unsigned char _pad00[0x28];
    long Morality;                              // +0x28
    void SetMarriageState(CThing* spouse, long state);   // 0x0057BC6E
    long GetMoralityMax();                      // 0x0057A67E
    void SetMorality(long morality);            // 0x0057C2E6
};
struct CHomeOccupant {
    unsigned char _pad00[4];
    CThing* Thing;                              // +0x04
};
class CTCVillage;
class CTCVillageMember { public:
    CTCVillage* GetPVillage() const;   // 0x007EE540 (Ego_r.pdb ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ)
};
struct CTCHome {
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool a);          // 0x004378C0
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CHeroMarriageDef { public:
    unsigned char _pad00[0xac];
    float HeroStatMoralityInOpinionFormChangePerDivorce;   // +0xac
};
class CTCBase { public:
    virtual ~CTCBase();                         // slot 0 (0x00686830)
protected:
    CPlayerManager& GetPlayerManager();         // 0x00686D40 (Ego_r.pdb CTCBase, protected)
};
class CTCWife : public CTCBase { public:
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CHeroMarriageDef* PHeroMarriageDef;         // +0x0c
    unsigned char _pad10[0x20 - 0x10];
    long FrameGotMarriedToThePlayer;            // +0x20
    bool DivorcedHero;                          // +0x24
    unsigned char _pad25[0x8c - 0x25];
    float LoveAttitudeValue;                    // +0x8c
    void Unmarry(bool notify);                  // 0x006CFF10
    void Divorce();
};

void CTCWife::Divorce()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetFrame())
        return;
    Unmarry(false);
    DivorcedHero = true;
    CTCCreatureOpinionOfHero* opinion;
    if (ParentThing->GetInterface(&opinion, 0x4f)) {
        {
            CCharString mask("OPINION_REACTION_MASK_WIFE_DIVORCED");
            opinion->SetOpinionReactionMask(mask);
        }
        {
            CCharString mask("OPINION_DEED_MASK_WIFE_DIVORCED");
            opinion->SetOpinionDeedMask(mask);
        }
        {
            CCharString personality("OPINION_PERSONALITY_WIFE_DIVORCED");
            opinion->SetPersonalityOverride(personality, true);
        }
        opinion->LastOpinionReactionFrame = opinion->GetWorld()->GetFrame();
        opinion->ClearAllKnownDeeds();
        opinion->HeroOpinionEnemy = false;
    }
    ParentThing->Flags1C9 &= ~8;
    LoveAttitudeValue = 0.0f;
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !(hero->Flags91 & 1)) {
        CTCHeroStats* stats;
        hero->GetInterface(&stats, 4);
        stats->SetMarriageState(ParentThing, 5);
        long morality = stats->Morality;
        const CHeroMarriageDef* def = PHeroMarriageDef;
        stats->SetMorality((long)(stats->GetMoralityMax() * def->HeroStatMoralityInOpinionFormChangePerDivorce + morality));
    }
    CTCHome* home;
    if (ParentThing->GetInterface(&home, 0x23)) {
        CHomeOccupant* occupant = ((CHomeOccupant*)((CTCVillageMember*)home)->GetPVillage());
        if (occupant) {
            CTCCreatureOpinionOfHero* other;
            if (occupant->Thing->GetInterface(&other, 0x4f)) {
                other->ApplyOpinionTransientOffset(0, -1.0f, 0, CMainGameComponent::GetConstantFPS() * 300, 0.0f, 0);
                other->ApplyOpinionTransientOffset(3, -2.0f, 0, CMainGameComponent::GetConstantFPS() * 300, -0.5f, 0);
            }
        }
    }
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
}