// CDisplayEngine::RenderHeroDrunkenness(const CInterpolationInfo&) const -- lets the hero's stats draw the drunkenness screen effect
// Leaf: ?RenderHeroDrunkenness@CDisplayEngine@@QBEXABVCInterpolationInfo@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x00435070, 94 bytes, `ret 4`. Catalog label CGameScriptInterface::DisplayTutorial is wrong. Evidence: the only
// caller is CDisplayEngine::DoRender 0x004357D8 (this = the display engine); it pushes this->Engine3D (+0x2c) into
// CTCHeroStats::RenderHeroDrunkenness 0x0057B43F; Ego_r.pdb ?RenderHeroDrunkenness@CDisplayEngine@@IBEXABVCInterpolationInfo@@@Z.
// Calls: the main game component singleton 0x013B86A0 (+0x1c PPlayerManager), CPlayerManager::GetMainPlayer 0x00449970,
// CPlayer::GetPControlledCreature 0x00487DC0, CThing::GetTC<CTCHeroStats> (Components lower_bound 0x0040F020).
// Layout: CDisplayEngine per Ego_r.pdb (Engine3D +0x2c); CMainGameComponent PPlayerManager +0x1c.
// CTCHeroStats retail = Ego_r.pdb minus 8 through +0x118 (12-byte STLport map and vector), minus 0x18 after the four vectors
// (Age +0x30, Stamina +0x58, RenownLevel +0x70, Attractiveness +0x9c, BestTimePairs +0x178, Marriages +0x1d8); engine/CTCHeroStats.h
// carries the PDB offsets.
// ScriptOverridePreventStaminaRefill +0x68 (PDB +0x70), TrainingStatus +0x114 (PDB +0x11c).
// Shape: GetTC's end-or-mismatch choice is one ternary whose ->component is stored (`*out = ((it == last ||
// type < it->type) ? last : it)->component`); the if/assign form tests the TC in eax and copies it to ecx (2 bytes longer).
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;      // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CIEngine;
class CInterpolationInfo;
class CCharStringData;

struct CMarriageNode { CMarriageNode* _M_next; CMarriageNode* _M_prev; };
class CMarriageList {                                    // std::list<CHeroMarriageInfo> (STLport: one node pointer)
public:
    CMarriageNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
enum EMarriageState { MARRIAGE_STATE_1 = 1, MARRIAGE_STATE_2 = 2, MARRIAGE_STATE_HAS_CHILDREN = 4, MARRIAGE_STATE_DIVORCED = 5,
                      MARRIAGE_STATE_WIFE_MURDERED = 7, MARRIAGE_STATE_FORCE_DWORD = 0x7fffffff };
enum EMorality { MORALITY_NEUTRAL = 3, MORALITY_FORCE_DWORD = 0x7fffffff };
enum EHeroTrainingStatus { HERO_TRAINING_STATUS_NONE = 0, HERO_TRAINING_STATUS_APPRENTICE = 1, HERO_TRAINING_STATUS_FORCE_DWORD = 0x7fffffff };
class CTCHeroStats {
public:
    unsigned char _pad_0x000[0x30];
    float Age;                                  // +0x030
    float SunTan;                               // +0x034
    float Fatness;                              // +0x038
    unsigned char _pad_0x03c[0x1c];
    long Stamina;                               // +0x058
    long MaxStamina;                            // +0x05c
    unsigned char _pad_0x060[0x8];
    bool ScriptOverridePreventStaminaRefill;    // +0x068
    unsigned char _pad_0x069[0x7];
    long RenownLevel;                           // +0x070
    long RenownMaxLevel;                        // +0x074
    unsigned char _pad_0x078[0x24];
    float Attractiveness;                       // +0x09c
    float Scariness;                            // +0x0a0
    unsigned char _pad_0x0a4[0x70];
    EHeroTrainingStatus TrainingStatus;         // +0x114
    unsigned char _pad_0x118[0x58];
    CCharString DeathRecoveryMarkerName;        // +0x170
    unsigned char _pad_0x174[0x4];
    float BestTimePairs;                        // +0x178
    float BestTimeSorting;                      // +0x17c
    unsigned char _pad_0x180[0x10];
    float BestTimeGuessTheAddition;             // +0x190
    unsigned char _pad_0x194[0x44];
    CMarriageList Marriages;                    // +0x1d8
    float GetAge() const { return Age; }
    float GetFatness() const { return Fatness; }
    long  GetStamina() const { return Stamina; }
    long  GetMaxStamina() const { return MaxStamina; }
    float GetStaminaPercentage() const { return (float)Stamina / MaxStamina; }
    long  GetRenownLevel() const { return RenownLevel; }
    float GetAttractiveness() const { return Attractiveness; }
    float GetScariness() const { return Scariness; }
    float GetBestTimePairs() const { return BestTimePairs; }
    float GetBestTimeSorting() const { return BestTimeSorting; }
    float GetBestTimeGuessTheAddition() const { return BestTimeGuessTheAddition; }
    const CCharString& GetDeathRecoveryMarkerName() const { return DeathRecoveryMarkerName; }
    bool  HasMarriageInState(EMarriageState state) const;          // 0x0057AC29
    bool  HasCurrentMarriage() const;                              // 0x0057AC4B
    float GetPercentageThroughRenownLevel() const;                 // 0x0057AAD9
    void  UpgradeRenownLevel();                                    // 0x0057CAE9
    void  AddRenown(long renown);                                  // 0x0057D8BE
    void  SetAge(float age);                                       // 0x0057B05F
    void  ForceStaminaTo(float stamina);                           // 0x0057B26F
    void  ResetTrainingStatus();                                   // 0x0057ADB8
    float GetMoralityPercentage() const;                           // 0x0057A690
    EMorality GetMoralityCategory() const;                         // 0x0057A6B8
    void RenderHeroDrunkenness(CIEngine* engine);                    // 0x0057B43F
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c
    CPlayerManager* GetPlayerManager() const { return PPlayerManager; }
};
class CDisplayEngine {
public:
    unsigned char _pad00[0x2c];
    CIEngine*     Engine3D;                              // +0x2c
    void RenderHeroDrunkenness(const CInterpolationInfo& info) const;
};

void CDisplayEngine::RenderHeroDrunkenness(const CInterpolationInfo& info) const
{
    CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager()->GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* stats;
    if (hero && hero->GetTC(TCI_HERO_STATS, &stats))
    {
        if (stats)
            stats->RenderHeroDrunkenness(Engine3D);
    }
}