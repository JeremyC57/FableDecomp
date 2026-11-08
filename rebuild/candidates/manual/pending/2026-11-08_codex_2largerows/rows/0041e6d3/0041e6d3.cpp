// 0041e6d3 NUISystem::CManager::Action
// Grade: DIFFER(35702v35651), ordinary MSVC 7.1 RTM; exact 0x800 frame. Partial checkpoint, no behavior test or complete symbol audit.
// Frame 0x800 matches retail. Incoming v23 fixes retained; real counted-pointer constructor bodies remain visible and out of line.
// Additional local fixes: bulk count object loads, unsigned augmentation guard, per-branch trade receiver caching.
// Entry action/game register roles, several shared tails, frame-slot order and photo/clan homes still differ.
// Full provenance and alternatives: WORKLOG_CURRENT.md, incoming_action_progress/, history/0041e6d3/.
// Leaf: ?Action@CManager@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Flags: /GS
// Inline-budget: scoped `#pragma inline_depth(0)` regions (one per statement retail calls out of line: list::begin 0x00429A85 x16 in
// cases 2005..2014 / 2019..2023 / CHOOSE_CLAN, vector::operator[] 0x0040F220 x3 (2004) and 0x00429A79 (2014), the CTCInventory /
// CTCInventoryAbilities setters 0x0041C965.. x9, postfix -- 0x0042899B x2) plus per-TU FABLE_RETAIL_*_FORCEINLINE_* / _NOINLINE_ opt-ins
// for the callees retail treats the same way at every site of this function (CWideString text ==, C2DVector / C2DExtentsI
// ctors, ~CSurface, map::begin expanded; list iterator != 0x0041C7C0 called) -- the function is past c2's 10000-tuple limit and its
// caller-size penalty ((sz/2)*N/9000, N = tuples so far) gives no single N that splits retail's set (probe/inl traces, WORKLOG S4).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_ACTION
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMFLOAT
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_ISOPEN
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS
#define FABLE_RETAIL_CGAMEEVENT_CTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CSPELLCONTAINERLIST_VIRTUALS
#define FABLE_RETAIL_CACTIONPARAMEBOOL
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ_HELPER
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_DATA_TEXT_EQ
#define FABLE_RETAIL_STLP_LIST_NOINLINE_NE
#define FABLE_RETAIL_STLP_MAP_FORCEINLINE_BEGIN
#define FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR
#define FABLE_RETAIL_C2DEXTENTSI_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR
#define FABLE_RETAIL_CMENUENTRY_ACTION
#define FABLE_RETAIL_CSPRITE_PUBLIC_TEXTURE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CASOUNDMANAGER_SLOT9
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CACTION_ISVALID
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCHero.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CGameEvent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamFloat.h"
#include "engine/NUISystem_CActionParamString.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CSpellContainerList.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/CCountedPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CUserProfileManager.h"
#include "engine/CASoundManager.h"
#include "engine/NUISystem_CActionParamEBOOL.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CMesh.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CEditBox.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"

class CObjectAugmentationsDef : public CDefClassBase { };

// Ego_r.pdb CEngineGraphic is 14 bytes (BankIndex +0, AnimStep +4, RenderSizeX +8, AdditiveAlpha +0xC, Type +0xD): packed in
// retail (natural alignment would make it 16), and the by-value temporaries GetEngineGraphic returns sit at 2-mod-4 frame
// offsets (e.g. [ebp-6E6h] at 0x004251E9, [ebp-702h] at 0x004252B7).
#pragma pack(push, 1)
class CEngineGraphic {
public:
    // Both ctors are user-declared in the PDB (FableWin ??0CEngineGraphic@@QAE@XZ, ??0CEngineGraphic@@QAE@W4EEngineGraphicType@@KMM_N@Z):
    // the class is not a POD, so GetEngineGraphic's hidden return pointer goes in ecx (id in edx) and callers read the
    // returned temporary in place (0x00425E52 `lea ecx,[ebp-756h]; call 0x005D96A0; cmp [eax],esi`).
    CEngineGraphic();
    CEngineGraphic(EEngineGraphicType type, unsigned long bankIndex, float animStep, float renderSizeX, bool additiveAlpha);
    unsigned long BankIndex;                                         // +0x00
    float AnimStep;                                                  // +0x04
    float RenderSizeX;                                               // +0x08
    bool AdditiveAlpha;                                              // +0x0c
    unsigned char Type;                                              // +0x0d
};
#pragma pack(pop)
// The def classes the description loaders read (Ego_r.pdb names; offsets from the reads in this function).
class CThingBaseDef : public CDefClassBase { };
class CThingObjectDef : public CThingBaseDef {
public:
    unsigned char _pad_0x25[0x70 - sizeof(CThingBaseDef)];
    CEngineGraphic Graphic;                                          // +0x70 (ADD_MESH_CHILD reads +0x70 / +0x7D)
};
class CAnimationEntry {
public:
    long AnimIndex;                                                  // +0x00
};
class CAnimFilterBase {                                              // vtable 0x0122FFCC
public:
    virtual bool Accept(const CAnimationEntry* entry) const;         // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() { }
};
class CAnimFilterNoGroup : public CAnimFilterBase {                  // vtable 0x0122FFDC
public:
    virtual bool Accept(const CAnimationEntry* entry) const;         // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                          // 0x00429A2F
};
class CAnimationSet {
public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
class CAppearanceDef : public CDefClassBase {
public:
    unsigned char _pad_0x25[0x34 - sizeof(CDefClassBase)];
    CAnimationSet Animation;                                         // +0x34
};
enum EWeaponClass { };
class CWeaponDef : public CDefClassBase {
public:
    unsigned char _pad_0x25[0x2c - sizeof(CDefClassBase)];
    EWeaponClass Class;                                              // +0x2c
};
class CAugmentationDef : public CDefClassBase {
public:
    unsigned char _pad_0x25[0x30 - sizeof(CDefClassBase)];
    unsigned long InventoryGraphic;                                  // +0x30
};

namespace std {
// STLport 4.6 _algo.h find() for input iterators (__fastcall in the retail build: Ego_r ??$find@..@std@@YI..).
template <class _InputIter, class _Tp> _InputIter __fastcall find(_InputIter first, _InputIter last, const _Tp& val)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
}

// Same-TU bodies of the def lookups retail emits in this TU's COMDATs and calls out of line (GetDef 0x0042AF3C, GetSubDef
// 0x0042AF9E ...): with them visible VC7.1 knows the def handles' addresses do not escape.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.IsNull())
        return false;
    out->SetDynamicCast(def);
    return true;
}
template <> bool CDefClassBase::GetSubDef<CObjectAugmentationsDef>(CDefPointer<const CObjectAugmentationsDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CObjectAugmentationsDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <> bool CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CAppearanceDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <> bool CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CWeaponDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <> bool CDefClassBase::GetSubDef<CAugmentationDef>(CDefPointer<const CAugmentationDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CAugmentationDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

enum EHeroAbility { };
enum EHeroTrainableStatType { };

class CThingPlayerCreature : public CThing { };
// FableWin.pdb ?GetPPlayerCharacter@CPlayer@@QBEPAVCThingPlayerCreature@@XZ, inline in retail (engine/CThingPlayerCreature.h body).
inline CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return PlayerCharacter.Get(); }

class CTCHeroStats {
public:
    long GetMoralityMax() const;                                     // 0x0057A67E
    void SetMorality(long morality);                                 // 0x0057C2E6
    void ForceRenownLevelTo(long level);                             // 0x0057CB00
    unsigned char _pad_0x00[0x74];
    long RenownMaxLevel;                                             // +0x74
    unsigned char _pad_0x78[0x10c - 0x78];
    long NumTeleports;                                               // +0x10c
};

class CTCInventoryWeapons : public NInventory::CTCInventoryBase {
public:
    virtual const std::vector<long>& GetInitialItems() const;        // slot 86 0x005C2F07
    virtual NUISystem::CComponent* ConstructScreenPC(NUISystem::CComponent*);   // slot 87 0x005C9A95
    void SetThingAsActiveWeapon(long, NUISystem::CComponent*);       // 0x005CB2B0
    void RefreshWeaponDescriptions();                                // 0x005CACB9
    void AugmentWeapon(long, long, bool&);                           // 0x005C44FA
    long GetFreeAugmentationSlotForWeapon(long);                     // 0x005C4671
    static const CCharString& __fastcall GetWeaponDescriptionString();   // 0x005C2DE1 ("Weapon_Descriptions" 0x013B8BAC)
    static const CCharString& __fastcall GetWeaponTitleString();     // 0x005C2DE7 ("Weapon_Title" 0x013B8BB4)
    float GetActiveMeleeWeaponDamage() const;                        // 0x005C4769
    float GetActiveProjectileWeaponDamage() const;                   // 0x005C3180
    void ConstructWeaponDescriptionContainer(NUISystem::CComponent*, long, float) const;   // 0x005C90FC
    void SetWeaponMappingToMenuEntry(NUISystem::CComponent*, long);  // 0x0042D405 (this TU, out of line)
    void AddComponentToAvailableWeaponsEntries(NUISystem::CComponent*);   // 0x0042BEAD (this TU, out of line)
    void RefreshAugmentationsLeftDescriptions();                     // 0x005C69AA
    unsigned char _pad_0x130[0x198 - 0x130];
    std::list<NUISystem::CComponent*> AvailableWeaponsEntries;      // +0x198
};
class CTCInventoryMap : public NInventory::CTCInventoryBase {
public:
    bool IsTeleportValid() const;                                    // 0x005FAA60
    void TeleportHeroToRegionTeleporter(long) const;                 // 0x005FC840
    void TeleportHeroToGuildSealRecallLocation() const;              // 0x005FAD40
    NUISystem::CComponent* ConstructQuestsScreen(NUISystem::CComponent*);   // 0x005FE090
    void DropQuest(long);                                            // 0x005FA900
    NUISystem::CComponent* ConstructLogbookList(NUISystem::CComponent*);       // 0x00607330
    NUISystem::CComponent* ConstructLogbookListPC(NUISystem::CComponent*);     // 0x006096E0
    NUISystem::CComponent* ConstructQuestsScreenPC(NUISystem::CComponent*);    // 0x00601310
};
class CTCInventoryQuests : public NInventory::CTCInventoryBase {
public:
    void TakeQuest(long);                                            // 0x00621F70
    void TeleportToBoastingPodium();                                 // 0x0061A360
    long GetQuestPositionAndRemoveIt(long);                          // 0x0061A2C0
};
class CTCInventoryExperience : public NInventory::CTCInventoryBase {
public:
    void BuyAbility(EHeroAbility, long);                             // 0x005785B3
    void BuyStat(EHeroTrainableStatType, long);                      // 0x005786B5
    void UndoBuyAbility(EHeroAbility, bool);                         // 0x00576C4F
    void UndoBuyStat(EHeroTrainableStatType, bool);                  // 0x00576ED1
    bool IsTutorialPlayed(const CCharString&);                       // 0x0057598A
    unsigned char _pad_0x130[0x134 - 0x130];
    std::map<CCharString, unsigned long> TutorialSoundHandles;       // +0x134
};
class CTCInventory : public NInventory::CTCInventoryBase {
public:
    virtual const std::vector<long>& GetInitialItems() const;        // slot 86 0x00581CC9
    virtual NUISystem::CComponent* ConstructScreenPC(NUISystem::CComponent*);   // slot 87 0x00591E5E
    void SetThingAsQuickAccessItem(long, long, bool);                // 0x00584157
    void AdjustGameplayOptionsSliders();                             // 0x005839A5
    void AdjustAudioOptionsSliders();                                // 0x00583815
    void AdjustVideoOptionsSliders();                                // 0x005838DD
    void ConstructStyleCardsScreen(long, long, long, NUISystem::CComponent*);     // 0x0058A724
    void ConstructStyleCardsScreenPC(long, long, long, NUISystem::CComponent*);   // 0x0058A729
    void AssignThingPC(long, unsigned long, NUISystem::CComponent*);              // 0x00584B86
    void UnAssignThingPC(unsigned long, NUISystem::CComponent*);                  // 0x00584F15
    void AssignThingSwappingPC(long, unsigned long, long, NUISystem::CComponent*, NUISystem::CComponent*);   // 0x00584EDC
    static const CCharString& __fastcall GetItemDescriptionString(); // 0x005819A1 ("Item_Descriptions" 0x013B8B50)
    static const CCharString& __fastcall GetItemNameTableString();   // 0x00581995 ("Item_Name_Table" 0x013B8B54)
    static const CCharString& __fastcall GetItemIconString();        // 0x0058199B ("Item_Icon" 0x013B8B58)
    void ConstructItemDescription(CWideString&, long);               // 0x005819DD
    void SetEditBoxValues();                                         // 0x0058216E
    void DeactivateAllEditBoxes(bool);                               // 0x00582414
    void SetThingToAssign(long thing) { ThingToAssign = thing; }                  // 0x0041CA31 when out of line
    void SetThingSlotToSwap(long slot) { ThingSlotToSwap = slot; }                // 0x0041CA3E
    void SetComponentSlotToSwap(NUISystem::CComponent* component) { PComponentSlotToSwap = component; }   // 0x0041CA52
    unsigned char _pad_0x130[0x134 - 0x130];
    long ThingToAssign;                                              // +0x134
    long ThingSlotToSwap;                                            // +0x138
    NUISystem::CComponent* PComponentSlotToSwap;                     // +0x13c
    unsigned char _pad_0x140[0x21c - 0x140];
    long CurrentOpenMenu;                                            // +0x21c (CURRENT_OPEN_MENU)
};
class CTCInventoryClothing : public NInventory::CTCInventoryBase {
public:
    void AddClothing(long);                                          // 0x005B4753
    void RefreshDescriptions();                                      // 0x005B690B
    void AddClothingToMannequin(long);                               // 0x005B4EA8
    void ResetMannequin();                                           // 0x005B425B
    NUISystem::CComponent* ConstructScreenPC(NUISystem::CComponent*);   // 0x005B8743
    static const CCharString& __fastcall GetClothingDescriptionString();   // 0x005B34C1 ("Clothing_Description" 0x013B8B9C)
    static const CCharString& __fastcall GetClothingIconString();    // 0x005B34BB ("Clothing_Icon" 0x013B8BA0)
    static const CCharString& __fastcall GetClothingTitleString();   // 0x005B34C7 ("Clothing_Title" 0x013B8B98)
    void SetClothingMappingToMenuEntry(NUISystem::CComponent*, long);   // 0x0042D41E (this TU, out of line)
};
class CTCInventoryStats : public NInventory::CTCInventoryBase {
public:
    virtual NUISystem::CComponent* ConstructPersonalityList(NUISystem::CComponent*);   // slot 86 0x005CE0E6
    virtual NUISystem::CComponent* ConstructExperienceList(NUISystem::CComponent*);    // slot 87 0x005D6748
    virtual NUISystem::CComponent* ConstructExperienceListPC(NUISystem::CComponent*);  // slot 88 0x005D7081
    virtual NUISystem::CComponent* ConstructPersonalityListPC(NUISystem::CComponent*); // slot 89 0x005D1F0C
    static CWideString __fastcall GetXString();                      // 0x005CC772
};
class CHeroAbilityDef : public CDefClassBase {
public:
    unsigned char _pad_0x0c[0x98 - sizeof(CDefClassBase)];
    long ABXYPriority;                                               // +0x98 (Ego_r.pdb +0x9C, after a 16-byte vector)
};
class CTCInventoryAbilities : public NInventory::CTCInventoryBase {
public:
    void AssignAbility(long, unsigned long);                         // 0x005F0C5F
    void UnAssignAbility(unsigned long);                             // 0x005F11A9
    void AssignAbilityPC(EHeroAbility, unsigned long, NUISystem::CComponent*);    // 0x005F0DC5
    void UnAssignAbilityPC(unsigned long, NUISystem::CComponent*);                // 0x005F1320
    void ResetAssignedAbilities();                                                // 0x005F694D
    void ResetAssignedAbilitiesPC();                                              // 0x005F6A6F
    NUISystem::CComponent* ConstructPCExpressionsList(NUISystem::CComponent*);    // 0x005F59C7
    NUISystem::CComponent* ConstructPCMagicList(NUISystem::CComponent*);          // 0x005F1B10
    void SetSpellToAssign(EHeroAbility spell) { SpellToAssign = spell; }          // 0x0041C965
    void SetSpellSlotToSwap(long slot) { SpellSlotToSwap = slot; }                // 0x0041C972
    void SetComponentSlotToSwap(NUISystem::CComponent* component) { PComponentSlotToSwap = component; }   // 0x0041C994
    void AssignAbilitySwappingPC(EHeroAbility, unsigned long, long, NUISystem::CComponent*, NUISystem::CComponent*);   // 0x005F4519
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility) const;        // 0x005EDFD9
    unsigned char _pad_0x130[0x134 - 0x130];
    EHeroAbility SpellToAssign;                                      // +0x134
    long SpellSlotToSwap;                                            // +0x138
    NUISystem::CComponent* PComponentSlotToSwap;                     // +0x13c
};
class CTCQuickAccessMenu : public CTCBase {
public:
    bool IsActive() const;                                           // 0x00625930
    bool IsQuitting() const;                                         // 0x00625940
    void Deactivate();                                               // 0x00625920
};
class CTCInventoryTrade : public NInventory::CTCInventoryBase {
public:
    virtual void OpenInventoryPC();                                  // slot 86 0x005AE86B
    virtual void CloseInventoryPC();                                 // slot 87 0x0059E2C7
    void BuyItem(NUISystem::CComponent*, long);                      // 0x005AF64E
    void BuyItemPC(NUISystem::CComponent*, long);                    // 0x005B0AAC
    void SellItem(NUISystem::CComponent*, long);                     // 0x005AECC0
    void SellItemPC(NUISystem::CComponent*, long);                   // 0x005B01FE
    static const CCharString& __fastcall GetSellItemIconString();    // 0x0059B6BC ("Sell_Item_Icon" 0x013B8B88)
    static const CCharString& __fastcall GetSellItemDescriptionString();   // 0x0059B6C2 ("Sell_Item_Desc" 0x013B8B7C)
    static const CCharString& __fastcall GetBuyItemIconString();     // 0x0059B6C8 ("Buy_Item_Icon" 0x013B8B8C)
    static const CCharString& __fastcall GetBuyItemDescriptionString();    // 0x0059B6CE ("Buy_Item_Desc" 0x013B8B80)
    static const CCharString& __fastcall GetWantedItemIconString();  // 0x0059B6D4 ("Wanted_Item_Icon" 0x013B8B90)
    static const CCharString& __fastcall GetWantedItemDescriptionString(); // 0x0059B6DA ("Wanted_Item_Desc" 0x013B8B84)
    long GetNumberOfSellItems(NUISystem::CComponent*) const;         // 0x0042CD3E (this TU, out of line)
    long GetNumberOfBuyItems(NUISystem::CComponent*) const;          // 0x0042CD61 (this TU, out of line)
    void RemoveFromItemSellList(NUISystem::CComponent*);             // 0x0042D437 (this TU, out of line)
    void RemoveFromItemBuyList(NUISystem::CComponent*);              // 0x0042D44A (this TU, out of line)
    bool IsSpecialCase(long);                                        // 0x0059C983
    NUISystem::CComponent* ConstructTradeDescription(long, unsigned long, bool, bool);   // 0x005AD571
    bool ConstructSellActionsList(NUISystem::CComponent*, long, long, bool);     // 0x005A45F6
    bool ConstructSellActionsListPC(NUISystem::CComponent*, long, long, bool);   // 0x005A731F
    bool ConstructBuyActionsList(NUISystem::CComponent*, long, long, bool);      // 0x005A3E47
    bool ConstructBuyActionsListPC(NUISystem::CComponent*, long, long, bool);    // 0x005A79E6
    unsigned char _pad_0x130[0x218 - 0x130];
    bool InventoryTradeOpen;                                         // +0x218
    unsigned char _pad_0x219[0x224 - 0x219];
    NUISystem::CComponent* PCurrentOpenTradeMenu;                    // +0x224
};
class CTCInventoryItem {
public:
    static bool __fastcall IsAugmentation(long);                     // 0x005D9870
    static CWideString __fastcall GetDescription(long);              // 0x005D95C0
    static CEngineGraphic __fastcall GetEngineGraphic(long);         // 0x005D96A0
    static CRightHandedSet __fastcall GetStartingRightHandedSet(long);   // 0x005D9730
};
template <class T> class CTBaseSingletonNoDefaultAlloc {
public:
    static T& __fastcall Get() { return *PClass; }
private:
    static CCountedPointer<T> PClass;
};
class CQuestManager : public CTBaseSingletonNoDefaultAlloc<CQuestManager> {
public:
    class CBoast;
    CBoast* GetBoast(long, CCharString);                             // 0x004AF7A0
    void SetBoastAsMade(long, CCharString);                          // 0x004B1A80
};
class CTCBoastUI {
public:
    void BoastTaken(const CQuestManager::CBoast*);                   // 0x0056A980
    void Deactivate();                                               // 0x0056A880
};
class CTCTavernGame {
public:
    void ChangeBet(bool);                                            // 0x005EB810
};
class CTCGamePlayer : public CTCBase {
public:
    CTCTavernGame* GetGame() const;                                  // 0x005EDC90
};
class CTCQuestCompletionUI : public CTCBase {
public:
    bool IsActive() const { return Active; }
    bool IsQuitting() const { return Quitting; }
    void Deactivate();                                               // 0x005E3DF0
    bool Active;                                                     // +0x0c (CTCBase is 0xC bytes)
    unsigned char _pad_0x0d[0x1c - 0x0d];
    bool Quitting;                                                   // +0x1c
};
class CTCInGameMenu {
public:
    void PCSetToLoad(long);                                          // 0x0047D008
    void SetToSave(long);                                            // 0x0047ED76
    void CloseInGameMenuViaGameEvent();                              // 0x0047D21C
    void PCSetQuitToFrontEnd();                                      // 0x0047D296
    void ConstructSaveGameFileDescription(CWideString&, long);       // 0x0047EEE4
    unsigned char _pad_0x00[0x84];
    std::vector<CCharString> FileRegionMiniMapNames;                 // +0x84
};
class CPlayerGuiDef {
public:
    unsigned char _pad_0x000[0x94c];
    std::map<CCharString, long> MiniMapGraphics;                     // +0x94c
};
class CPlayerGui : public CTBaseSingletonNoDefaultAlloc<CPlayerGui> {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();             // 0x00438570
    bool IsLiveGUIMode() const { return LiveGuiMode; }
    void ToggleLiveGui();                                            // 0x00438310
    void FlashQuickAccessDPadIcon(long button, bool flash);          // 0x00437A30 (W4EX.. button enum, spelled long)
    unsigned char _pad_0x000[0x2be];
    bool LiveGuiMode;                                                // +0x2be
};
class CWeapon;
class CTCWeapon {
public:
    static bool __fastcall IsMeleeWeaponClass(EWeaponClass);         // 0x005DA510
};
class CTCPhotojournalCapture {
public:
    void SetDiscardScreenshot(bool discard) { DiscardScreenShot = discard; }
    void OnCaptionEntered(const CWideString&);                       // 0x0055D810
    void ClosePhotojournalCapture();                                 // 0x0055D770
    unsigned char _pad_0x00[0x10];
    bool DiscardScreenShot;                                          // +0x10
};
class CTCHeroOnlineScoreboard {
public:
    void OnPlayerSelectedClan(CWideString, NUISystem::CComponent*);  // 0x005625F0
    void OnOkClicked();                                              // 0x0055EE20
};
// The object CSoundManagerLH's slot 9 returns (its +0x44); its slot 9 stops a playing sample (descriptive, as the parked 0x005E78F0).
class CASoundSampleOwner_Unproven {
public:
    virtual void CUnprovenSlot0(); virtual void CUnprovenSlot1(); virtual void CUnprovenSlot2(); virtual void CUnprovenSlot3();
    virtual void CUnprovenSlot4(); virtual void CUnprovenSlot5(); virtual void CUnprovenSlot6(); virtual void CUnprovenSlot7();
    virtual void CUnprovenSlot8();
    virtual void StopSample(unsigned long handle);                   // slot 9 (+0x24)
};
CWideString __fastcall GFIntToWideString(long);                      // 0x00405F00
class CWorld {
public:
    void ResetToFrontEnd();                                          // 0x0049DCC0
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->IsDead();
}
bool __fastcall GFPlayFullScreenXBoxMovie(const CCharString&, unsigned long, unsigned long = 0, unsigned long = 0, unsigned long = 0,
                                          const class NGameText_CDataBank* = 0, class CIEngine* = 0, bool = true, float = -1.0f,
                                          bool (__fastcall*)() = 0, class CXMVPlayerBuffers* = 0, CWideString* = 0);   // 0x006286F0
extern CASoundManager* PSoundManager;                                // 0x013B8394
extern bool GTeleportOpenedFromMap;                                  // 0x0137D76C

namespace NUISystem {

class CFrontEndManager : public CObserver {
public:
    static CFrontEndManager* __fastcall GetInstance();               // 0x00595582
    virtual void Action(CCountedPointer<CAction>&);                  // slot 8
    void GetScoreboardOptionsToProfile();                            // 0x00596550
    void GotoPreviousScreen(bool);                                   // 0x00597BF2
    void ResetSlidersToProfileValues();                              // 0x00595356
    void DeactivateAllEditBoxes(CComponent*);                        // 0x005952D8
};
// Ego_r.pdb NUISystem::CActionParamCCountedPointer<NUISystem::CComponent> (16 bytes: the CActionParam base, the pointer +0x08).
template <class T> class CActionParamCCountedPointer : public CActionParam {
public:
    CCountedPointer<T> Value;                                        // +0x08
};
// Ego_r.pdb NUISystem::CActionParam2DVector (16 bytes: the CActionParam base, C2DVector Value +0x08); MOVE_COMPONENT passes &Value.
class CActionParam2DVector : public CActionParam {
public:
    C2DVector Value;                                                 // +0x08
};
class CRedefinerList {
public:
    static bool __fastcall CanAcceptChanges();                       // 0x00556540
    static void __fastcall ResetKeysToDefaults(bool wasd);           // 0x005565C0
};

// After an option change: silence the effect / music sound groups (0x10, 0x80, 0x40) until the menus reapply the volumes.
#define SILENCE_SOUND_GROUPS() \
    if (PSoundManager) { \
        PSoundManager->SetGroupVolume(0x10, 0.0f, 0.0f); \
        PSoundManager->SetGroupVolume(0x80, 0.0f, 0.0f); \
        PSoundManager->SetGroupVolume(0x40, 0.0f, 0.0f); \
    }
// Open / close one of the hero's inventory screens through its thing component.
#define OPEN_INVENTORY(type) { \
        NInventory::CTCInventoryBase* inventory; \
        if (PPlayer->GetTC(type, &inventory) && inventory) \
            inventory->OpenInventory(); \
    }
#define CLOSE_INVENTORY(type) { \
        NInventory::CTCInventoryBase* inventory; \
        if (PPlayer->GetTC(type, &inventory) && inventory) \
            inventory->CloseInventory(); \
    }
#define CLOSE_INVENTORY_EVENT(type) { \
        NInventory::CTCInventoryBase* inventory; \
        if (PPlayer->GetTC(type, &inventory) && inventory) \
            inventory->AddCloseInventoryEvent(); \
    }

void CManager::Action(CCountedPointer<CAction>& action)
{
    if (!action->IsValid())
        return;
    CMainGameComponent* game = CMainGameComponent::Get();
    if (!game) {
        CFrontEndManager::GetInstance()->Action(action);
        game = CMainGameComponent::Get();
    }

    switch (action->ActionType) {
    case UI_ACTION_TYPE_WIELD:
        if (ThingActionParams.size() > 0 && UIActionParams.size() > 0) {
            PWeaponsInventory->SetThingAsActiveWeapon(ThingActionParams.back(), static_cast<CComponent*>(UIActionParams.back()));
            PWeaponsInventory->RefreshWeaponDescriptions();
            SetEvent(UI_EVENT_LEFT);
            CMenuEntry::GSetForceProcessing(true);
            for (long i = 0; i < 10; ++i)
                CList::GPreviousSelected.back()->Update(1.0f);
            SetEvent(UI_EVENT_LEFT);
            for (long j = 0; j < 10; ++j)
                CList::GPreviousSelected.back()->Update(1.0f);
            CMenuEntry::GSetForceProcessing(false);
        }
        break;
    case UI_ACTION_TYPE_AUGMENT:
        if (action->ThingActionParams.size() > 0 && ThingActionParams.size() > 0 &&
            action->UIActionParams.size() > 0 && UIActionParams.size() > 0) {
            bool augmented = false;
            if (CTCInventoryItem::IsAugmentation(action->ThingActionParams.back())) {
                CDefPointer<const CDefClassBase> weaponDef;
                CDefPointer<const CObjectAugmentationsDef> augmentationsDef;
                if (CGameDefinitionManager::Get().GetDef(ThingActionParams.back(), &weaponDef) &&
                    weaponDef->GetSubDef(&augmentationsDef)) {
                    PWeaponsInventory->AugmentWeapon(ThingActionParams.back(), action->ThingActionParams.back(), augmented);
                    --NumberAugmentations;
                }
            }
            bool keepAugmentation = true;
            if (!augmented) {
                CComponent* augmentation = GetCorrectComponentInterface(static_cast<CComponent*>(action->UIActionParams.back()));
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                augmentation->SetDeletion(deletion);
                keepAugmentation = false;
            }
            if (NumberAugmentations <= 0) {
                CComponent* augmentation = GetCorrectComponentInterface(static_cast<CComponent*>(action->UIActionParams.back()));
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_ALL_INSTANCES;
                augmentation->SetDeletion(deletion);
                keepAugmentation = false;
                CTCHero* hero;
                if (PPlayer->GetTC(TCI_HERO, &hero) && !hero->WeaponsUsable) {
                    std::list<CComponent*>& entries = PWeaponsInventory->AvailableWeaponsEntries;
                    for (std::list<CComponent*>::iterator it = entries.begin(); it != entries.end(); ++it) {
                        CComponent* arrow = (*it)->FindChildForward("PC_MENU_ENTRY_ARROW");
                        if (arrow) {
                            CDeletion arrowDeletion;
                            arrowDeletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                            arrow->SetDeletion(arrowDeletion);
                        }
                        (*it)->MakeUnavailableDynamically();
                    }
                    entries.clear();
                }
            }
            if (NumberAugmentations > 0 && !keepAugmentation) {
                CTCHero* hero;
                if (PPlayer->GetTC(TCI_HERO, &hero) && !hero->WeaponsUsable) {
                    CComponent* entry = static_cast<CComponent*>(UIActionParams.back());
                    CComponent* arrow = entry->FindChildForward("PC_MENU_ENTRY_ARROW");
                    if (arrow) {
                        CDeletion arrowDeletion;
                        arrowDeletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                        arrow->SetDeletion(arrowDeletion);
                    }
                    entry->MakeUnavailableDynamically();
                    std::list<CComponent*>& entries = PWeaponsInventory->AvailableWeaponsEntries;
                    std::list<CComponent*>::iterator found = std::find(entries.begin(), entries.end(), entry);
                    if (found != entries.end())
                        entries.erase(found);
                }
            }
            PWeaponsInventory->RefreshWeaponDescriptions();
            PWeaponsInventory->RefreshAugmentationsLeftDescriptions();
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_ADD_CHILD_AUGMENTATION:
        if (action->UIActionParams.size() > 1 && ThingActionParams.size() > 0 && UIActionParams.size() > 0) {
            CComponent* weaponEntry = GetCorrectComponentInterface(static_cast<CComponent*>(UIActionParams.back()));
            CActionParamULONG* graphic = static_cast<CActionParamULONG*>(action->UIActionParams.front());
            CComponent* augmentationEntry = static_cast<CComponent*>(action->UIActionParams.back());
            CCountedPointer<CComponent> augmentation;
            switch (PWeaponsInventory->GetFreeAugmentationSlotForWeapon(ThingActionParams.back())) {
            case 3:
                augmentation.Reset(CreateComponent("PC_AUGMENTATION_ON_WEAPON_FOURTH_SLOT_TEMPLATE", false));
                break;
            case 2:
                augmentation.Reset(CreateComponent("PC_AUGMENTATION_ON_WEAPON_THIRD_SLOT_TEMPLATE", false));
                break;
            case 1:
                augmentation.Reset(CreateComponent("PC_AUGMENTATION_ON_WEAPON_SECOND_SLOT_TEMPLATE", false));
                break;
            case 0:
                augmentation.Reset(CreateComponent("PC_AUGMENTATION_ON_WEAPON_FIRST_SLOT_TEMPLATE", false));
                break;
            }
            if (augmentation->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(augmentation.Get())->SetGraphicIndex(graphic->Value);
            if (augmentation.Get()) {
                weaponEntry->AddChildDynamically(augmentation);
                if (augmentationEntry)
                    augmentationEntry->AddPositionChild(augmentation.Get());
            }
        }
        break;
    case UI_ACTION_TYPE_TELEPORT:
        if (action->ThingActionParams.size() > 0 && PMapInventory->IsTeleportValid()) {
            long teleporter = action->ThingActionParams.back();
            CTCHeroStats* stats;
            CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_HERO_STATS, &stats);
            ++stats->NumTeleports;
            PMapInventory->TeleportHeroToRegionTeleporter(teleporter);
            GTeleportOpenedFromMap = false;
            PMapInventory->AddCloseInventoryEvent();
        }
        break;
    case UI_ACTION_TYPE_TAKE_QUEST:
        if (action->ThingActionParams.size() > 0) {
            PQuestInventory->TakeQuest(action->ThingActionParams.back());
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_TAKE_BOAST:
        if (action->UIActionParams.size() > 0) {
            CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CActionParamULONG* questCard = static_cast<CActionParamULONG*>(action->UIActionParams.front());
            CActionParamString* boastName = static_cast<CActionParamString*>(action->UIActionParams.back());
            const CQuestManager::CBoast* boast = CQuestManager::Get().GetBoast(questCard->Value, boastName->Value.ToCharString());
            CTCBoastUI* boastUI;
            if (IS_THING_ALIVE(hero) && hero->GetTC(TCI_BOAST_UI, &boastUI))
                boastUI->BoastTaken(boast);
            CQuestManager::Get().SetBoastAsMade(questCard->Value, boastName->Value.ToCharString());
        }
        break;
    case UI_ACTION_TYPE_BUY_ABILITY:
        if (action->UIActionParams.size() >= 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.end();
            long cost = static_cast<CActionParamSLONG*>(*--it)->Value;
            long ability = static_cast<CActionParamSLONG*>(*--it)->Value;
            PExperienceInventory->BuyAbility((EHeroAbility)ability, cost);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_BUY_STAT:
        if (action->UIActionParams.size() >= 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.end();
            long cost = static_cast<CActionParamSLONG*>(*--it)->Value;
            long stat = static_cast<CActionParamSLONG*>(*--it)->Value;
            PExperienceInventory->BuyStat((EHeroTrainableStatType)stat, cost);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_USE_ITEM:
        if (ThingActionParams.size() > 0) {
            bool handled = true;
            PInventory->AddCloseInventoryEvent();
            PInventory->OnItemSelected(ThingActionParams.back(), handled);
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_LEFT:
        if (ThingActionParams.size() > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams.back(), 2, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_RIGHT:
        if (ThingActionParams.size() > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams.back(), 3, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_DOWN:
        if (ThingActionParams.size() > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams.back(), 1, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_UP:
        if (ThingActionParams.size() > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams.back(), 0, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_CLOTHING_WEAR:
        if (ThingActionParams.size() > 0) {
            PInventoryClothing->AddClothing(ThingActionParams.back());
            PInventoryClothing->RefreshDescriptions();
            SetEvent(UI_EVENT_LEFT);
            CMenuEntry::GSetForceProcessing(true);
            for (long i = 0; i < 10; ++i)
                CList::GPreviousSelected.back()->Update(1.0f);
            SetEvent(UI_EVENT_LEFT);
            for (long j = 0; j < 10; ++j)
                CList::GPreviousSelected.back()->Update(1.0f);
            CMenuEntry::GSetForceProcessing(false);
        }
        break;
    case UI_ACTION_TYPE_UPDATE_MANNEQUIN_CLOTHING:
        if (action->ThingActionParams.size() > 0)
            PInventoryClothing->AddClothingToMannequin(action->ThingActionParams.back());
        break;
    case UI_ACTION_TYPE_RESET_CLOTHING_MANNEQUIN:
        PInventoryClothing->ResetMannequin();
        break;
    case UI_ACTION_TYPE_BUY:
        if (UIActionParams.size() > 0) {
            CComponent* entry = static_cast<CComponent*>(UIActionParams.back());
            if (PInventoryTrade->IsInventoryOpen())
                PInventoryTrade->BuyItemPC(entry, 1);
            else
                PInventoryTrade->BuyItem(entry, 1);
        }
        break;
    case UI_ACTION_TYPE_SELL:
        if (UIActionParams.size() > 0) {
            CComponent* entry = static_cast<CComponent*>(UIActionParams.back());
            if (PInventoryTrade->IsInventoryOpen())
                PInventoryTrade->SellItemPC(entry, 1);
            else
                PInventoryTrade->SellItem(entry, 1);
        }
        break;
    case UI_ACTION_TYPE_SET_BRIGHTNESS:
        CUserProfileManager::Get().SetGamma(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_SOUND:
        CUserProfileManager::Get().SetSoundVolume(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_MUSIC:
        CUserProfileManager::Get().SetMusicVolume(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_HEADPHONES: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetUsingHeadPhones(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_SUBTITLES: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetSubtitlingEnabled(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_VIBRATION: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetVibrationEnabled(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_CAMERA_UP_DOWN: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetCameraYZInverted(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_CAMERA_ROTATION: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetCameraXYInverted(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_OPEN_MAP:
        OPEN_INVENTORY(TCI_INVENTORY_MAP);
        break;
    case UI_ACTION_TYPE_OPEN_ABILITIES:
        OPEN_INVENTORY(TCI_HERO_ABILITIES);
        break;
    case UI_ACTION_TYPE_OPEN_WEAPONS:
        OPEN_INVENTORY(TCI_INVENTORY_WEAPONS);
        break;
    case UI_ACTION_TYPE_OPEN_STATS:
        OPEN_INVENTORY(TCI_INVENTORY_STATS);
        break;
    case UI_ACTION_TYPE_OPEN_CLOTHING:
        OPEN_INVENTORY(TCI_INVENTORY_CLOTHING);
        break;
    case UI_ACTION_TYPE_CLOSE_MAP:
        CLOSE_INVENTORY(TCI_INVENTORY_MAP);
        break;
    case UI_ACTION_TYPE_CLOSE_ABILITIES:
        CLOSE_INVENTORY(TCI_HERO_ABILITIES);
        break;
    case UI_ACTION_TYPE_CLOSE_WEAPONS:
        CLOSE_INVENTORY(TCI_INVENTORY_WEAPONS);
        break;
    case UI_ACTION_TYPE_CLOSE_STATS:
        CLOSE_INVENTORY(TCI_INVENTORY_STATS);
        break;
    case UI_ACTION_TYPE_CLOSE_CLOTHING:
        CLOSE_INVENTORY(TCI_INVENTORY_CLOTHING);
        break;
    case UI_ACTION_TYPE_CHANGESTATE:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.end();
            CActionParamULONG* state = static_cast<CActionParamULONG*>(*--it);
            CComponent* component = static_cast<CComponent*>(*--it);
            component->ChangeState(state->Value);
        }
        break;
    case UI_ACTION_TYPE_DELETE:
        if (action->UIActionParams.size() > 0) {
            CComponent* component = GetCorrectComponentInterface(static_cast<CComponent*>(action->UIActionParams.back()));
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            component->SetDeletion(deletion);
        }
        break;
    case UI_ACTION_TYPE_DELETE_PREVIOUS:
        if (UIActionParams.size() > 0) {
            CComponent* component = GetCorrectComponentInterface(static_cast<CComponent*>(UIActionParams.back()));
            SetEvent(UI_EVENT_LEFT);
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            component->SetDeletion(deletion);
        }
        break;
    case UI_ACTION_TYPE_DELETE_ALL:
        if (action->UIActionParams.size() > 0) {
            CComponent* component = GetCorrectComponentInterface(static_cast<CComponent*>(action->UIActionParams.back()));
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_ALL_INSTANCES;
            component->SetDeletion(deletion);
        }
        break;
    case UI_ACTION_TYPE_ADD_CHILD:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.end();
#pragma inline_depth(0) // inline budget: retail 0x0041F4D8 calls _List_iterator::operator--(int) out of line
            it--;
#pragma inline_depth()
            CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*it));
#pragma inline_depth(0) // inline budget: retail 0x0041F504 calls _List_iterator::operator--(int) out of line
            it--;
#pragma inline_depth()
            CCountedPointer<CComponent> child(GetCorrectComponentInterface(static_cast<CComponent*>(*it)));
            parent->AddChildDynamically(child);
        }
        break;
    case UI_ACTION_TYPE_CLOSE_MENU:
        CLOSE_INVENTORY_EVENT(TCI_INVENTORY);
        break;
    case UI_ACTION_TYPE_GUILD_SEAL_RECALL:
        if (PMapInventory->IsTeleportValid()) {
            PMapInventory->TeleportHeroToGuildSealRecallLocation();
            GTeleportOpenedFromMap = false;
            PMapInventory->AddCloseInventoryEvent();
        }
        break;
    case UI_ACTION_TYPE_BUY_BULK:
        if (UIActionParams.size() > 0 && action->UIActionParams.size() > 0) {
            CComponent* entry = static_cast<CComponent*>(UIActionParams.back());
            CActionParamSLONG* count = static_cast<CActionParamSLONG*>(action->UIActionParams.back());
            if (PInventoryTrade->IsInventoryOpen())
                PInventoryTrade->BuyItemPC(entry, count->Value);
            else
                PInventoryTrade->BuyItem(entry, count->Value);
        }
        break;
    case UI_ACTION_TYPE_SELL_BULK:
        if (UIActionParams.size() > 0 && action->UIActionParams.size() > 0) {
            CComponent* entry = static_cast<CComponent*>(UIActionParams.back());
            CActionParamSLONG* count = static_cast<CActionParamSLONG*>(action->UIActionParams.back());
            if (PInventoryTrade->IsInventoryOpen())
                PInventoryTrade->SellItemPC(entry, count->Value);
            else
                PInventoryTrade->SellItem(entry, count->Value);
        }
        break;
    case UI_ACTION_TYPE_SET_DIALOGUE:
        CUserProfileManager::Get().SetDialogueVolume(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_HUD:
        CUserProfileManager::Get().SetHUDAlpha(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_TUTORIALS: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetTutorialSystemEnabled(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_GUILD_MASTER: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetGuildMasterEnabled(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_HUD_TOOLTIPS: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetHUDTooltipEnabled(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_SHOW_BUDDY_NAMES: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetShowingBuddyNames(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_ACCEPT_CHANGES:
        if (CRedefinerList::CanAcceptChanges()) {
            if (!CMainGameComponent::Get())
                CFrontEndManager::GetInstance()->GetScoreboardOptionsToProfile();
            CUserProfileManager::Get().Save();
            if (!CMainGameComponent::Get())
                CFrontEndManager::GetInstance()->GotoPreviousScreen(false);
        }
        break;
    case UI_ACTION_TYPE_CLOSE_TELEPORT_MENU:
        if (PPlayer)
            CLOSE_INVENTORY_EVENT(TCI_INVENTORY_MAP);
        break;
    case UI_ACTION_TYPE_LOAD: {
        CActionParamULONG* file = static_cast<CActionParamULONG*>(UIActionParams.back());
        CTCInGameMenu* menu;
        PPlayer->GetTC(TCI_IN_GAME_MENU, &menu);
        menu->PCSetToLoad(file->Value);
        if (CPlayerGui::Get().IsLiveGUIMode())
            CPlayerGui::Get().ToggleLiveGui();
        break;
    }
    case UI_ACTION_TYPE_SAVE: {
        CActionParamULONG* file = static_cast<CActionParamULONG*>(UIActionParams.back());
        CTCInGameMenu* menu;
        PPlayer->GetTC(TCI_IN_GAME_MENU, &menu);
        menu->SetToSave(file->Value);
        break;
    }
    case UI_ACTION_TYPE_QUIT_TO_FRONT_END:
        game->GetWorld().ResetToFrontEnd();
        break;
    case UI_ACTION_TYPE_CLOSE_PAUSE_MENU: {
        CTCInGameMenu* menu;
        PPlayer->GetTC(TCI_IN_GAME_MENU, &menu);
        menu->CloseInGameMenuViaGameEvent();
        break;
    }
    case UI_ACTION_TYPE_PLAY_MOVIE:
        GFPlayFullScreenXBoxMovie(static_cast<CActionParamString*>(action->UIActionParams.back())->Value.ToCharString(), 0x20);
        break;
    case UI_ACTION_TYPE_CONSTRUCT_WEAPONS_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryWeapons* weapons;
        if (hero->GetTC(TCI_INVENTORY_WEAPONS, &weapons) && weapons) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("WEAPONS_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = CPlayerGui::Get().IsLiveGUIMode() ? weapons->ConstructScreenPC(screen) : weapons->ConstructScreen(screen);
                if (list) {
                    list->SetName("WEAPONS_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_CLOTHING_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryClothing* clothing;
        if (hero->GetTC(TCI_INVENTORY_CLOTHING, &clothing) && clothing) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("CLOTHING_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = CPlayerGui::Get().IsLiveGUIMode() ? clothing->ConstructScreenPC(screen) : clothing->ConstructScreen(screen);
                if (list) {
                    list->SetName("CLOTHING_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_PERSONALITY_LIST: {
        CTCInventoryStats* stats;
        if (PPlayer->GetTC(TCI_INVENTORY_STATS, &stats) && stats) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("PERSONALITY_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = stats->ConstructPersonalityList(screen);
                if (list) {
                    list->SetName("PERSONALITY_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_EXPERIENCE_LIST: {
        CTCInventoryStats* stats;
        if (PPlayer->GetTC(TCI_INVENTORY_STATS, &stats) && stats) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("EXPERIENCE_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = stats->ConstructExperienceList(screen);
                if (list) {
                    list->SetName("EXPERIENCE_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_ITEMS_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("ITEMS_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = CPlayerGui::Get().IsLiveGUIMode() ? inventory->ConstructScreenPC(screen) : inventory->ConstructScreen(screen);
                if (list) {
                    list->SetName("ITEMS_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_SKILLS_LIST: {
        CTCInventoryAbilities* abilities;
        if (PPlayer->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("SKILLS_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = abilities->ConstructScreen(screen);
                if (list) {
                    list->SetName("SKILLS_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_QUESTS_LIST: {
        CTCInventoryMap* map;
        if (PPlayer->GetTC(TCI_INVENTORY_MAP, &map) && map) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("QUESTS_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = map->ConstructQuestsScreen(screen);
                if (list) {
                    list->SetName("QUESTS_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_MAP_LIST: {
        CTCInventoryMap* map;
        if (PPlayer->GetTC(TCI_INVENTORY_MAP, &map) && map) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("MAP_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = map->ConstructScreen(screen);
                if (list) {
                    list->SetName("MAP_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CAction> closeAction(new CAction);
                    closeAction->ActionType = UI_ACTION_TYPE_CLOSE_MAP;
                    list->AddActionOnInactive(closeAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_STATS_LIST: {
        CTCInventoryStats* stats;
        if (PPlayer->GetTC(TCI_INVENTORY_STATS, &stats) && stats) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("STATS_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = stats->ConstructScreen(screen);
                if (list) {
                    list->SetName("STATS_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_PREVIOUS_SCREEN:
        if (game)
            CFrontEndManager::GetInstance()->Action(action);
        break;
    case UI_ACTION_TYPE_CHANGE_CHILD_STATE: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        CActionParamULONG* depth = static_cast<CActionParamULONG*>(*it);
        CActionParamString* childName = static_cast<CActionParamString*>(*++it);
        CActionParamULONG* state = static_cast<CActionParamULONG*>(*++it);
        std::list<CActionParam*>::reverse_iterator component = UIActionParams.rbegin();
        for (unsigned long i = depth->Value; i > 0; --i)
            ++component;
        CComponent* child = static_cast<CComponent*>(*component)->FindChild(childName->Value.ToCharString());
        if (child)
            child->ChangeState(state->Value);
        break;
    }
    case UI_ACTION_TYPE_CLOSE_QUICK_ACCESS_MENU: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
        CTCQuickAccessMenu* menu;
        hero->GetTC(TCI_QUICK_ACCESS_MENU, &menu);
        if (menu->IsActive() && !menu->IsQuitting())
            menu->Deactivate();
        break;
    }
    case UI_ACTION_TYPE_UNDO_BUY_ABILITY:
        PExperienceInventory->UndoBuyAbility((EHeroAbility)static_cast<CActionParamSLONG*>(action->UIActionParams.back())->Value, false);
        SetEvent(UI_EVENT_LEFT);
        break;
    case UI_ACTION_TYPE_TELEPORT_TO_BOAST: {
        CTCInventoryQuests* quests = 0;
        PPlayer->GetTC(TCI_INVENTORY_QUESTS, &quests);
        quests->TeleportToBoastingPodium();
        break;
    }
    case UI_ACTION_TYPE_DROP_QUEST:
        if (action->ThingActionParams.size() > 0)
            PMapInventory->DropQuest(action->ThingActionParams.back());
        break;
    case UI_ACTION_TYPE_UNDO_BUY_STAT:
        PExperienceInventory->UndoBuyStat((EHeroTrainableStatType)static_cast<CActionParamSLONG*>(action->UIActionParams.back())->Value, false);
        SetEvent(UI_EVENT_LEFT);
        break;
    case UI_ACTION_TYPE_ASSIGN_SPELL: {
        std::list<CActionParam*>::reverse_iterator it = UIActionParams.rbegin();
        ++it; ++it;
        CActionParamULONG* ability = static_cast<CActionParamULONG*>(*it);
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(action->UIActionParams.back());
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities)
            abilities->AssignAbility(ability->Value, slot->Value);
        break;
    }
    case UI_ACTION_TYPE_UNASSIGN_SPELL: {
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(action->UIActionParams.back());
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities)
            abilities->UnAssignAbility(slot->Value);
        break;
    }
    case UI_ACTION_TYPE_RESET_ASSIGNED_SPELLS: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities)
            abilities->ResetAssignedAbilities();
        break;
    }
    case UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT:
        if (UIActionParams.size() > 0) {
            CComponent* component = static_cast<CComponent*>(UIActionParams.back());
            AddExclusiveObserver(component);
            component->ObserveAllEvents();
        }
        break;
    case UI_ACTION_TYPE_REMOVE_EXCLUSIVE_INPUT:
        if (m_pExclusiveObserver && UIActionParams.size() > 0)
            RemoveExclusiveObserver(static_cast<CComponent*>(UIActionParams.back()));
        break;
    case UI_ACTION_TYPE_SHOW_SPELL_SELECTION:
        if (UIActionParams.size() > 0)
            static_cast<CSpellContainerList*>(static_cast<CComponent*>(UIActionParams.back()))->ShowSpellSelectionChild();
        break;
    case UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN:
        if (UIActionParams.size() > 1) {
            std::list<CActionParam*>::reverse_iterator it = UIActionParams.rbegin();
            CSpellContainerList* spells = static_cast<CSpellContainerList*>(static_cast<CComponent*>(*it));
            spells->SetSpellToAssign(static_cast<CActionParamSLONG*>(*++it)->Value);
        }
        break;
    case UI_ACTION_TYPE_CHANGE_ALPHA: {
        long alpha = 0xff;
        for (std::list<CActionParam*>::iterator it = action->UIActionParams.begin(); it != action->UIActionParams.end(); ++it) {
            CActionParam* param = *it;
            if (param->ActionParamType == UI_ACTION_PARAM_TYPE_SLONG)
                alpha = static_cast<CActionParamSLONG*>(param)->Value;
            if (param->ActionParamType == UI_ACTION_PARAM_TYPE_COMPONENT) {
                CComponent* component = static_cast<CComponent*>(param);
                CRGBColour colour = component->GetColour();
                colour.A = (unsigned char)alpha;
                component->ChangeColour(colour, 0.2f, false);
                component->GetStates()[0].Colour.A = (unsigned char)alpha;
            }
        }
        break;
    }
    case UI_ACTION_TYPE_POP_MAP_STATE: {
        CComponent* map = static_cast<CComponent*>(action->UIActionParams.front());
        CTCInventoryQuests* quests;
        game->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY_QUESTS, &quests);
        unsigned long state = quests->GetQuestPositionAndRemoveIt(action->ThingActionParams.front()) + 8;
        std::map<unsigned long, CUIState>::iterator it;
        it = map->GetStates().find(state);
        if (it != map->GetStates().end()) {
            std::map<unsigned long, CUIState> states;
            bool moved = false;
            for (it = map->GetStates().begin(); it != map->GetStates().end(); ++it) {
                if (it->first > state) {
                    std::map<unsigned long, CUIState>::value_type entry(it->first - 1, it->second);
                    states.insert(entry);
                    moved = true;
                }
                if (it->first < state)
                    states.insert(*it);
            }
            map->GetStates().clear();
            map->GetStates() = states;
            map->ChangeState(moved ? state : state - 1);
        }
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_SKILLS_MENU: {
        CGameEvent event(0x46);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_INVENTORY_MENU: {
        CGameEvent event(0x48);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_MAP_MENU: {
        CGameEvent event(0x4b);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_STATUS_MENU: {
        CGameEvent event(0x4f);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_OPTIONS_MENU: {
        CGameEvent event(0x49);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_BUY_TRADING_GOODS_LIST: {
        CGameEvent event(0x4c);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_SELL_TRADING_GOODS_LIST: {
        CGameEvent event(0x4d);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_WANTED_TRADING_GOODS_LIST: {
        CGameEvent event(0x4e);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_CLOSE_PC_BUY_TRADING_GOODS_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryTrade* trade;
        if (hero->GetTC(TCI_INVENTORY_TRADE, &trade) && trade)
            trade->CloseInventoryPC();
        break;
    }
    case UI_ACTION_TYPE_FLASH_DPAD_DOWN:
        CPlayerGui::Get().FlashQuickAccessDPadIcon(0xe, true);
        break;
    case UI_ACTION_TYPE_FLASH_DPAD_UP:
        CPlayerGui::Get().FlashQuickAccessDPadIcon(0xd, true);
        break;
    case UI_ACTION_TYPE_FLASH_DPAD_LEFT:
        CPlayerGui::Get().FlashQuickAccessDPadIcon(0xf, true);
        break;
    case UI_ACTION_TYPE_FLASH_DPAD_RIGHT:
        CPlayerGui::Get().FlashQuickAccessDPadIcon(0x10, true);
        break;
    case UI_ACTION_TYPE_STOP_DPAD_FLASHING:
        CPlayerGui::Get().FlashQuickAccessDPadIcon(0, false);
        break;
    case UI_ACTION_TYPE_RESTORE_DEFAULTS_GAMEPLAY:
        CUserProfileManager::Get().SetDefaultValuesGameplay();
        if (CMainGameComponent::Get()) {
            CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventory* inventory;
            if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory)
                inventory->AdjustGameplayOptionsSliders();
        } else if (CFrontEndManager::GetInstance())
            CFrontEndManager::GetInstance()->ResetSlidersToProfileValues();
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_RESTORE_DEFAULTS_AUDIO:
        CUserProfileManager::Get().SetDefaultValuesAudio();
        if (CMainGameComponent::Get()) {
            CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventory* inventory;
            if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory)
                inventory->AdjustAudioOptionsSliders();
        } else if (CFrontEndManager::GetInstance())
            CFrontEndManager::GetInstance()->ResetSlidersToProfileValues();
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_RESTORE_DEFAULTS_VIDEO:
        CUserProfileManager::Get().SetDefaultValuesVideo();
        if (CMainGameComponent::Get()) {
            CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventory* inventory;
            if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory)
                inventory->AdjustVideoOptionsSliders();
        } else if (CFrontEndManager::GetInstance())
            CFrontEndManager::GetInstance()->ResetSlidersToProfileValues();
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_RESOLUTION:
        CUserProfileManager::Get().SetResolution(static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_REFRESH_RATE:
        CUserProfileManager::Get().SetRefreshRate(static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_SCREEN_ASPECT_RATIO:
        CUserProfileManager::Get().SetScreenAspectRatio(static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_CONTROL_METHOD:
        CUserProfileManager::Get().SetNewProfileToUseWASD(static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value != 0);
        break;
    case UI_ACTION_TYPE_APPLY_PROFILE_VALUES:
        CUserProfileManager::Get().Apply();
        CUserProfileManager::Get().CaptureBackBufferForUIBackground();
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_SHADOW_DETAIL:
        CUserProfileManager::Get().SetShadowDetail(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_EFFECTS_DETAIL:
        CUserProfileManager::Get().SetEffectsDetail(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_VERTICAL_SYNC: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value != 0;
        CUserProfileManager::Get().SetVerticalSyncEnabled(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_ANTIALIASING:
        CUserProfileManager::Get().SetAntialiasing(static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_TEXTURE_DETAIL:
        CUserProfileManager::Get().SetTextureDetail(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_CAMERA_SENSITIVITY:
        CUserProfileManager::Get().SetCameraSensitivity(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_SET_CAMERA_RESETTING: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetCameraResetting(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_BOW_CAMERA: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetCameraBowYZInverted(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_SHOW_TARGETING_STATUS: {
        bool enabled = static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value > 0;
        CUserProfileManager::Get().SetShowTargetingStatus(enabled, false);
        SILENCE_SOUND_GROUPS();
        break;
    }
    case UI_ACTION_TYPE_SET_MESH_DETAIL:
        CUserProfileManager::Get().SetMeshDetail(static_cast<CActionParamFloat*>(action->UIActionParams.back())->Value, false);
        SILENCE_SOUND_GROUPS();
        break;
    case UI_ACTION_TYPE_PC_ACCEPT_VIDEO_CHANGES:
        CUserProfileManager::Get().Apply();
        CUserProfileManager::Get().CaptureBackBufferForUIBackground();
        SILENCE_SOUND_GROUPS();
        if (!CUserProfileManager::Get().WereVideoChangesAccepted())
            SetChange((EChange)1);
        break;
    case UI_ACTION_TYPE_CLOSE_TRADE_MENU_IF_LEAF: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryTrade* trade;
        if (hero->GetTC(TCI_INVENTORY_TRADE, &trade) && !trade->PCurrentOpenTradeMenu) {
            CGameEvent event(0x1b);
            event.Player = (signed char)CMainGameComponent::Get()->GetPlayerManager().GetMainPlayerNumber();
            CMainGameComponent::Get()->AddGameEvent(event);
        }
        break;
    }
    case UI_ACTION_TYPE_EXIT_LIVE_GUI_IF_LEAF: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory->IsInventoryOpen() && !inventory->CurrentOpenMenu)
            CPlayerGui::Get().ToggleLiveGui();
        break;
    }
    case UI_ACTION_TYPE_EXIT_LIVE_GUI:
        CPlayerGui::Get().ToggleLiveGui();
        break;
    case UI_ACTION_TYPE_PC_QUIT_TO_FRONT_END: {
        CTCInGameMenu* menu;
        if (PPlayer->GetTC(TCI_IN_GAME_MENU, &menu) && menu)
            menu->PCSetQuitToFrontEnd();
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_LOGBOOK_LIST: {
        CTCInventoryMap* map;
        if (PPlayer->GetTC(TCI_INVENTORY_MAP, &map) && map) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("LOGBOOK_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = map->ConstructLogbookList(screen);
                if (list) {
                    list->SetName("LOGBOOK_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_OPEN_PC_LOG_BOOK: {
        CTCInventoryMap* map;
        if (PPlayer->GetTC(TCI_INVENTORY_MAP, &map) && map) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("LOGBOOK_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = map->ConstructLogbookListPC(screen);
                if (list) {
                    list->SetName("LOGBOOK_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_PC_EXPERIENCE_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryStats* stats;
        if (hero->GetTC(TCI_INVENTORY_STATS, &stats) && stats) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("EXPERIENCE_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = stats->ConstructExperienceListPC(screen);
                if (list) {
                    list->SetName("EXPERIENCE_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_PC_PERSONALITY_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryStats* stats;
        if (hero->GetTC(TCI_INVENTORY_STATS, &stats) && stats) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("PERSONALITY_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = stats->ConstructPersonalityListPC(screen);
                if (list) {
                    list->SetName("PERSONALITY_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_STYLE_CARDS_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("STYLE_CARDS_LIST")) {
                GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                long category = static_cast<CActionParamSLONG*>(*++it)->Value;
                long first = static_cast<CActionParamSLONG*>(*++it)->Value;
                long last = static_cast<CActionParamSLONG*>(*++it)->Value;
                inventory->ConstructStyleCardsScreen(category, first, last, screen);
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_PC_STYLE_CARDS_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("STYLE_CARDS_LIST")) {
                GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                long category = static_cast<CActionParamSLONG*>(*++it)->Value;
                long first = static_cast<CActionParamSLONG*>(*++it)->Value;
                long last = static_cast<CActionParamSLONG*>(*++it)->Value;
                inventory->ConstructStyleCardsScreenPC(category, first, last, screen);
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_PC_QUESTS_LIST: {
        CTCInventoryMap* map;
        if (PPlayer->GetTC(TCI_INVENTORY_MAP, &map) && map) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("QUESTS_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = map->ConstructQuestsScreenPC(screen);
                if (list) {
                    list->SetName("QUESTS_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_ASSIGN_SPELL_PC: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(*it);
        CComponent* entry = static_cast<CComponent*>(*++it);
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities && abilities->SpellToAssign) {
            if (!AssignmentSpellsSwapping)
                abilities->AssignAbilityPC(abilities->SpellToAssign, slot->Value, entry);
            else
                abilities->AssignAbilitySwappingPC(abilities->SpellToAssign, slot->Value, abilities->SpellSlotToSwap, entry, abilities->PComponentSlotToSwap);
            AssignmentSpellsSwapping = false;
            abilities->SpellToAssign = (EHeroAbility)0;
        }
        break;
    }
    case UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(*it);
        CComponent* entry = static_cast<CComponent*>(*++it);
        CTCInventory* inventory = 0;
        CTCInventoryAbilities* abilities = 0;
        game->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &inventory);
        CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_HERO_ABILITIES, &abilities);
        if (inventory && abilities) {
            if (abilities->SpellToAssign) {
                if (!AssignmentItemsExpressionsSwapping) {
                    CDefPointer<const CHeroAbilityDef> ability = abilities->GetAbilityPDef(abilities->SpellToAssign);
                    inventory->AssignThingPC(ability->ABXYPriority, slot->Value, entry);
                } else {
                    CDefPointer<const CHeroAbilityDef> ability = abilities->GetAbilityPDef(abilities->SpellToAssign);
                    inventory->AssignThingSwappingPC(ability->ABXYPriority, slot->Value, inventory->ThingSlotToSwap, entry, inventory->PComponentSlotToSwap);
                }
                abilities->SpellToAssign = (EHeroAbility)0;
            } else if (inventory->ThingToAssign) {
                if (!AssignmentItemsExpressionsSwapping)
                    inventory->AssignThingPC(inventory->ThingToAssign, slot->Value, entry);
                else
                    inventory->AssignThingSwappingPC(inventory->ThingToAssign, slot->Value, inventory->ThingSlotToSwap, entry, inventory->PComponentSlotToSwap);
                inventory->ThingToAssign = 0;
            }
            AssignmentItemsExpressionsSwapping = false;
            AssignmentSpellsSwapping = false;
        }
        break;
    }
    case UI_ACTION_TYPE_ASSIGN_EXPRESSION_ITEM_PC: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(*it);
        CComponent* entry = static_cast<CComponent*>(*++it);
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory) {
            if (!AssignmentItemsExpressionsSwapping)
                inventory->AssignThingPC(inventory->ThingToAssign, slot->Value, entry);
            else
                inventory->AssignThingSwappingPC(inventory->ThingToAssign, slot->Value, inventory->ThingSlotToSwap, entry, inventory->PComponentSlotToSwap);
            inventory->ThingToAssign = 0;
            AssignmentItemsExpressionsSwapping = false;
            AssignmentSpellsSwapping = false;
        }
        break;
    }
    case UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CActionParamSLONG* thing = static_cast<CActionParamSLONG*>(*it);
            CActionParamSLONG* slot = static_cast<CActionParamSLONG*>(*++it);
            CComponent* entry = static_cast<CComponent*>(*++it);
            CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventory* inventory;
            if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory) {
                AssignmentItemsExpressionsSwapping = true;
                inventory->SetThingToAssign(thing->Value);
#pragma inline_depth(0) // inline budget: retail 0x004224B3 / 0x00422A7D calls SetThingSlotToSwap / SetComponentSlotToSwap out of line
                inventory->SetThingSlotToSwap(slot->Value);
                inventory->SetComponentSlotToSwap(entry);
#pragma inline_depth()
            }
        }
        break;
    case UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC:
        if (action->UIActionParams.size() > 0) {
            CActionParamSLONG* thing = static_cast<CActionParamSLONG*>(*action->UIActionParams.rbegin());
            CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventory* inventory;
            if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory) {
                AssignmentItemsExpressionsSwapping = false;
#pragma inline_depth(0) // inline budget: retail 0x00422547 calls SetThingToAssign out of line
                inventory->SetThingToAssign(thing->Value);
#pragma inline_depth()
            }
        }
        break;
    case UI_ACTION_TYPE_UNASSIGN_SPELL_PC: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(*it);
        CComponent* entry = static_cast<CComponent*>(*++it);
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities)
            abilities->UnAssignAbilityPC(slot->Value, entry);
        break;
    }
    case UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(*it);
        CComponent* entry = static_cast<CComponent*>(*++it);
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory)
            inventory->UnAssignThingPC(slot->Value, entry);
        break;
    }
    case UI_ACTION_TYPE_RESET_ASSIGNED_SPELLS_PC: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities)
            abilities->ResetAssignedAbilitiesPC();
        break;
    }
    case UI_ACTION_TYPE_SEND_OWNED_EVENT:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.end();
            CComponent* owner = static_cast<CComponent*>(*--it);
            SetOwnedEvent(owner, (EEvent)static_cast<CActionParamULONG*>(*--it)->Value);
        }
        break;
    case UI_ACTION_TYPE_SEND_OWNED_EVENT_FORCE_OBSERVATION:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.end();
            CComponent* owner = static_cast<CComponent*>(*--it);
            owner->ProcessEvent((EEvent)static_cast<CActionParamULONG*>(*--it)->Value);
        }
        break;
    case UI_ACTION_TYPE_ADD_OBSERVER:
        if (action->UIActionParams.size() > 0)
            AddObserver(static_cast<CComponent*>(action->UIActionParams.back()));
        break;
    case UI_ACTION_TYPE_REMOVE_OBSERVER:
        if (action->UIActionParams.size() > 0)
            RemoveObserver(static_cast<CComponent*>(action->UIActionParams.back()));
        break;
    case UI_ACTION_TYPE_MOVE_COMPONENT:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.end();
            float time = static_cast<CActionParamFloat*>(*--it)->Value;
            CActionParam2DVector* delta = static_cast<CActionParam2DVector*>(*--it);
            static_cast<CComponent*>(*--it)->ChangePositionDelta(delta->Value, time, false);
        }
        break;
    case UI_ACTION_TYPE_SEND_EVENT:
        if (action->UIActionParams.size() > 0)
            SetEvent((EEvent)static_cast<CActionParamULONG*>(action->UIActionParams.back())->Value);
        break;
    case UI_ACTION_TYPE_SCROLL_DESCRIPTION_DOWN:
        SetEvent(UI_EVENT_RIGHT_STICK_UP);
        break;
    case UI_ACTION_TYPE_SCROLL_DESCRIPTION_UP:
        SetEvent(UI_EVENT_RIGHT_STICK_DOWN);
        break;
    case UI_ACTION_TYPE_SCROLL_DESCRIPTION_DOWN_ONE_PIXEL:
        SetDescriptionScrollingOffset(1.0f);
        SetEvent(UI_EVENT_RIGHT_STICK_UP);
        SetDescriptionScrollingOffset(-1.0f);
        break;
    case UI_ACTION_TYPE_SCROLL_DESCRIPTION_UP_ONE_PIXEL:
        SetDescriptionScrollingOffset(1.0f);
        SetEvent(UI_EVENT_RIGHT_STICK_DOWN);
        SetDescriptionScrollingOffset(-1.0f);
        break;
    case UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CActionParamSLONG* spell = static_cast<CActionParamSLONG*>(*it);
            CActionParamSLONG* slot = static_cast<CActionParamSLONG*>(*++it);
            CComponent* entry = static_cast<CComponent*>(*++it);
            CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventoryAbilities* abilities;
            if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities) {
                AssignmentSpellsSwapping = true;
#pragma inline_depth(0) // inline budget: retail 0x00422928..0x00422945 calls the three setters out of line
                abilities->SetSpellToAssign((EHeroAbility)spell->Value);
                abilities->SetSpellSlotToSwap(slot->Value);
                abilities->SetComponentSlotToSwap(entry);
#pragma inline_depth()
            }
        }
        break;
    case UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CActionParamSLONG* spell = static_cast<CActionParamSLONG*>(*it);
            CActionParamSLONG* slot = static_cast<CActionParamSLONG*>(*++it);
            CComponent* entry = static_cast<CComponent*>(*++it);
            CTCInventoryAbilities* abilities = 0;
            game->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_HERO_ABILITIES, &abilities);
            CTCInventory* inventory = 0;
            CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &inventory);
            if (abilities && inventory) {
                AssignmentItemsExpressionsSwapping = true;
#pragma inline_depth(0) // inline budget: retail 0x00422A62..0x00422A7F calls the three setters out of line
                abilities->SetSpellToAssign((EHeroAbility)spell->Value);
                inventory->SetThingSlotToSwap(slot->Value);
                inventory->SetComponentSlotToSwap(entry);
#pragma inline_depth()
            }
        }
        break;
    case UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC:
        if (action->UIActionParams.size() > 0) {
            CActionParamSLONG* spell = static_cast<CActionParamSLONG*>(*action->UIActionParams.rbegin());
            CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventoryAbilities* abilities;
            if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities) {
                AssignmentSpellsSwapping = false;
#pragma inline_depth(0) // inline budget: retail 0x00422B0D calls SetSpellToAssign out of line
                abilities->SetSpellToAssign((EHeroAbility)spell->Value);
#pragma inline_depth()
            }
        }
        break;
    case UI_ACTION_TYPE_CONSTRUCT_EXPRESSIONS_LIST: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("EXPRESSIONS_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = abilities->ConstructPCExpressionsList(screen);
                if (list) {
                    list->SetName("EXPRESSIONS_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_MAGIC_LIST_PC: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryAbilities* abilities;
        if (hero->GetTC(TCI_HERO_ABILITIES, &abilities) && abilities) {
            std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
            CComponent* screen = static_cast<CComponent*>(*it);
            if (!screen->FindChild("MAGIC_LIST")) {
                CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
                CComponent* list = abilities->ConstructPCMagicList(screen);
                if (list) {
                    list->SetName("MAGIC_LIST");
                    CCountedPointer<CAction> deleteAction(new CAction);
                    deleteAction->ActionType = UI_ACTION_TYPE_DELETE;
                    deleteAction->UIActionParams.push_back(list);
                    list->AddActionOnInactive(deleteAction);
                    CCountedPointer<CComponent> child(list);
                    parent->AddChildDynamically(child);
                }
            }
        }
        break;
    }
    case UI_ACTION_TYPE_OBSERVE_EVENT: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        EEvent event = (EEvent)static_cast<CActionParamULONG*>(*it)->Value;
        static_cast<CComponent*>(*++it)->ObserveEvent(event);
        break;
    }
    case UI_ACTION_TYPE_IGNORE_EVENT: {
        std::list<CActionParam*>::reverse_iterator it = action->UIActionParams.rbegin();
        EEvent event = (EEvent)static_cast<CActionParamULONG*>(*it)->Value;
        static_cast<CComponent*>(*++it)->IgnoreEvent(event);
        break;
    }
    case UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UNTIL_CHILD_CONTAINING: {
        CComponent* child = static_cast<CComponent*>(action->UIActionParams.back());
        if (CList::GPreviousSelected.size() > 0)
            CList::GPreviousSelected.front()->ScrollUntilChildContaining(child);
        break;
    }
    case UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP: {
        CList::GPreviousSelected.front()->ChangeTargetPositionDelta(C2DVector(0.0f, -25.0f), 0.0f, false);
        break;
    }
    case UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN: {
        CList::GPreviousSelected.front()->ChangeTargetPositionDelta(C2DVector(0.0f, 25.0f), 0.0f, false);
        break;
    }
    case UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP_MAX:
        CList::GPreviousSelected.front()->ScrollToFirstChild();
        break;
    case UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN_MAX:
        CList::GPreviousSelected.front()->ScrollToLastChild();
        break;
    case UI_ACTION_TYPE_SCROLL_DESCRIPTION_UP_MAX:
        SetDescriptionScrollingToMax(true);
        SetEvent(UI_EVENT_RIGHT_STICK_DOWN);
        SetDescriptionScrollingToMax(false);
        break;
    case UI_ACTION_TYPE_SCROLL_DESCRIPTION_DOWN_MAX:
        SetDescriptionScrollingToMax(true);
        SetEvent(UI_EVENT_RIGHT_STICK_UP);
        SetDescriptionScrollingToMax(false);
        break;
    case UI_ACTION_TYPE_TAVERN_GAME_BET_DOWN: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCGamePlayer* player = 0;
        hero->GetTC(TCI_GAME_PLAYER, &player);
        if (player->GetGame())
            player->GetGame()->ChangeBet(false);
        break;
    }
    case UI_ACTION_TYPE_TAVERN_GAME_BET_UP: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCGamePlayer* player = 0;
        hero->GetTC(TCI_GAME_PLAYER, &player);
        if (player->GetGame())
            player->GetGame()->ChangeBet(true);
        break;
    }
    case UI_ACTION_TYPE_SEND_BACK_EVENT:
        SetEvent(UI_EVENT_BACK);
        break;
    case UI_ACTION_TYPE_RESET_KEYS:
        CRedefinerList::ResetKeysToDefaults(false);
        break;
    case UI_ACTION_TYPE_RESET_KEYS_WASD:
        CRedefinerList::ResetKeysToDefaults(true);
        break;
    case UI_ACTION_TYPE_CLOSE_ACTIVE_MENU: {
        CGameEvent event(0x51);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_CLOSE_FRAME: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory && inventory->CurrentOpenMenu) {
            CGameEvent event(0x51);
            event.Player = (signed char)CMainGameComponent::Get()->GetPlayerManager().GetMainPlayerNumber();
            CMainGameComponent::Get()->AddGameEvent(event);
        } else {
            CThing* trader = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventoryTrade* trade;
            if (trader->GetTC(TCI_INVENTORY_TRADE, &trade) && trade && trade->IsInventoryOpen()) {
                CGameEvent event(0x53);
                event.Player = (signed char)CMainGameComponent::Get()->GetPlayerManager().GetMainPlayerNumber();
                CMainGameComponent::Get()->AddGameEvent(event);
            } else {
                CGameEvent event(0x1b);
                event.Player = (signed char)CMainGameComponent::Get()->GetPlayerManager().GetMainPlayerNumber();
                CMainGameComponent::Get()->AddGameEvent(event);
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CLOSE_ACTIVE_TRADE_MENU: {
        CGameEvent event(0x53);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_CONTINUE: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCQuestCompletionUI* completion;
        if (hero->GetTC(TCI_QUEST_COMPLETION_UI, &completion) && completion->IsActive() && !completion->IsQuitting())
            completion->Deactivate();
        break;
    }
    case UI_ACTION_TYPE_RESPAWN: {
        CGameEvent event(8);
        event.AddUBYTE(1);
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_ADD_MESH_CHILD:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CActionParamULONG* thing = static_cast<CActionParamULONG*>(*it);
            CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(*++it));
            CActionParamString* templateName = static_cast<CActionParamString*>(*++it);
            CComponent* positionParent = 0;
            if (CPlayerGui::Get().IsLiveGUIMode() || (PInventoryTrade && PInventoryTrade->IsInventoryOpen()))
                positionParent = static_cast<CComponent*>(*++it);
            if (!parent->FindFirstChildOfType(UI_TYPE_MESH)) {
                CCountedPointer<CComponent> component(CreateComponent(templateName->Value.ToCharString(), false));
                CCountedPointer<CMesh> mesh;
                mesh.SetStaticCast(component);
                CDefPointer<const CThingObjectDef> def;
                CGameDefinitionManager::Get().GetDef(thing->Value, &def);
                mesh->SetMeshIndex(def->Graphic.BankIndex);
                CRightHandedSet orientation = CTCInventoryItem::GetStartingRightHandedSet(thing->Value);
                mesh->SetOrientation(orientation);
                if (def->Graphic.Type == 4) {
                    CDefPointer<const CAppearanceDef> appearance;
                    def->GetSubDef(&appearance);
                    mesh->SetAnimationIndex(appearance->Animation.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup())->AnimIndex);
                    mesh->SetMeshType((EEngineGraphicType)4);
                }
                parent->AddChildDynamically(component);
                if (CPlayerGui::Get().IsLiveGUIMode() || (PInventoryTrade && PInventoryTrade->IsInventoryOpen()))
                    positionParent->AddPositionChild(component.Get());
            }
        }
        break;
    case UI_ACTION_REMOVE_MESH_CHILD:
        if (action->UIActionParams.size() > 0) {
            CComponent* parent = GetCorrectComponentInterface(static_cast<CComponent*>(action->UIActionParams.front()));
            CComponent* mesh = parent->FindFirstChildOfType(UI_TYPE_MESH);
            if (mesh) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                mesh->SetDeletion(deletion);
            }
        }
        break;
    case UI_ACTION_LOAD_WEAPON_DESC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            long weapon = static_cast<CActionParamULONG*>(*it)->Value;
            CViewport* viewport = static_cast<CViewport*>(static_cast<CComponent*>(*++it));
            CComponent* parent = static_cast<CComponent*>(*++it);
            CComponent* positionParent = 0;
            if (CPlayerGui::Get().IsLiveGUIMode())
                positionParent = static_cast<CComponent*>(*++it);
            if (!parent->FindChild(CTCInventoryWeapons::GetWeaponDescriptionString())) {
                CCountedPointer<CComponent> container;
                if (CPlayerGui::Get().IsLiveGUIMode()) {
                    container.Reset(CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE", false));
                    positionParent->AddPositionChild(container.Get());
                } else {
                    container.Reset(CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_ONE_OFF", false));
                }
                CDefPointer<const CThingBaseDef> def;
                CGameDefinitionManager::Get().GetDef(weapon, &def);
                CDefPointer<const CWeaponDef> weaponDef;
                def->GetSubDef(&weaponDef);
                PWeaponsInventory->ConstructWeaponDescriptionContainer(container.Get(), weapon,
                    CTCWeapon::IsMeleeWeaponClass(weaponDef->Class) ? PWeaponsInventory->GetActiveMeleeWeaponDamage()
                                                                           : PWeaponsInventory->GetActiveProjectileWeaponDamage());
                PWeaponsInventory->SetWeaponMappingToMenuEntry(container.Get(), weapon);
                container->SetName(CTCInventoryWeapons::GetWeaponDescriptionString());
                viewport->AddFakeChild(container.Get());
                parent->AddChildDynamically(container);
            }
        }
        break;
    case UI_ACTION_UNLOAD_WEAPON_DESC:
        if (action->UIActionParams.size() > 0) {
            CComponent* parent = static_cast<CComponent*>(action->UIActionParams.front());
            CComponent* description = parent->FindChild(CTCInventoryWeapons::GetWeaponDescriptionString());
            if (description) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                description->SetDeletion(deletion);
            }
        }
        break;
    case UI_ACTION_LOAD_CLOTHING_DESC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CActionParamULONG* clothing = static_cast<CActionParamULONG*>(*it);
            CViewport* viewport = static_cast<CViewport*>(static_cast<CComponent*>(*++it));
            CComponent* parent = static_cast<CComponent*>(*++it);
            CComponent* positionParent = 0;
            if (CPlayerGui::Get().IsLiveGUIMode())
                positionParent = static_cast<CComponent*>(*++it);
            if (!parent->FindChild(CTCInventoryClothing::GetClothingDescriptionString())) {
                CCountedPointer<CComponent> container;
                if (CPlayerGui::Get().IsLiveGUIMode()) {
                    container.Reset(CreateComponent("PC_DESCRIPTION_INDEPENDANT", false));
                    positionParent->AddPositionChild(container.Get());
                } else {
                    container.Reset(CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
                }
                CCountedPointer<CText> text;
                text.SetStaticCast(container);
                text->SetName(CTCInventoryClothing::GetClothingDescriptionString());
                CWideString description;
                PInventoryClothing->GetItemDescription(description, clothing->Value, false);
                PInventoryClothing->SetClothingMappingToMenuEntry(container.Get(), clothing->Value);
                text->SetValue(description, true, true);
                viewport->AddFakeChild(container.Get());
                parent->AddChildDynamically(container);
                if (CTCInventoryItem::GetEngineGraphic(clothing->Value).BankIndex != 0) {
                    CCountedPointer<CComponent> icon;
                    if (CPlayerGui::Get().IsLiveGUIMode()) {
                        icon.Reset(CreateComponent("PC_ICON_INDEPENDANT", false));
                        positionParent->AddPositionChild(icon.Get());
                    } else {
                        icon.Reset(CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
                    }
                    if (icon->GetType() == UI_TYPE_SPRITE)
                        static_cast<CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(clothing->Value).BankIndex);
                    icon->SetName(CTCInventoryClothing::GetClothingIconString());
                    parent->AddChildDynamically(icon);
                }
            }
        }
        break;
    case UI_ACTION_UNLOAD_CLOTHING_DESC:
        if (action->UIActionParams.size() > 0) {
            CComponent* parent = static_cast<CComponent*>(action->UIActionParams.front());
            CComponent* description = parent->FindChild(CTCInventoryClothing::GetClothingDescriptionString());
            if (description) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                description->SetDeletion(deletion);
            }
            CComponent* icon = parent->FindChild(CTCInventoryClothing::GetClothingIconString());
            if (icon) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                icon->SetDeletion(deletion);
            }
        }
        break;
    case UI_ACTION_LOAD_ITEM_DESC:
        if (action->UIActionParams.size() > 3) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CViewport* viewport = static_cast<CViewport*>(static_cast<CComponent*>(*++it));
            CComponent* parent = static_cast<CComponent*>(*++it);
            CActionParamSLONG* quantity = static_cast<CActionParamSLONG*>(*++it);
            CComponent* positionParent = 0;
            if (CPlayerGui::Get().IsLiveGUIMode())
                positionParent = static_cast<CComponent*>(*++it);
            if (!parent->FindChild(CTCInventory::GetItemDescriptionString())) {
                CCountedPointer<CComponent> container;
                if (CPlayerGui::Get().IsLiveGUIMode()) {
                    container.Reset(CreateComponent("PC_DESCRIPTION_INDEPENDANT", false));
                    positionParent->AddPositionChild(container.Get());
                } else {
                    container.Reset(CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
                }
                CCountedPointer<CText> text;
                container->SetName(CTCInventory::GetItemDescriptionString());
                text.SetStaticCast(container);
                CWideString description;
                PInventory->ConstructItemDescription(description, item->Value);
                text->SetValue(description, true, true);
                parent->AddChildDynamically(container);
                if (viewport)
                    viewport->AddFakeChild(container.Get());
                CWideString itemName = CTCInventoryItem::GetDescription(item->Value);
                if (quantity->Value > 1) {
                    itemName += CTCInventoryStats::GetXString();
                    itemName += GFIntToWideString(quantity->Value);
                }
                long graphic = CTCInventoryItem::GetEngineGraphic(item->Value).BankIndex;
                if (graphic) {
                    CCountedPointer<CComponent> name;
                    if (CPlayerGui::Get().IsLiveGUIMode()) {
                        name.Reset(CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
                        positionParent->AddPositionChild(name.Get());
                    } else {
                        name.Reset(CreateComponent("UI_TEXT_ITEM_NAME_TEMPLATE_NEW", false));
                    }
                    CCountedPointer<CText> nameText;
                    nameText.SetStaticCast(name);
                    nameText->SetValue(itemName, true, true);
                    name->SetName(CTCInventory::GetItemNameTableString());
                    parent->AddChildDynamically(name);
                    CCountedPointer<CComponent> icon;
                    if (CPlayerGui::Get().IsLiveGUIMode()) {
                        icon.Reset(CreateComponent("PC_ICON_INDEPENDANT", false));
                        positionParent->AddPositionChild(icon.Get());
                    } else {
                        icon.Reset(CreateComponent("UI_ITEM_SPRITE_TEMPLATE", false));
                    }
                    if (icon->GetType() == UI_TYPE_SPRITE)
                        static_cast<CSprite*>(icon.Get())->SetGraphicIndex(graphic);
                    icon->SetName(CTCInventory::GetItemIconString());
                    parent->AddChildDynamically(icon);
                } else {
                    CCountedPointer<CComponent> name;
                    if (CPlayerGui::Get().IsLiveGUIMode()) {
                        name.Reset(CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
                        positionParent->AddPositionChild(name.Get());
                    } else {
                        name.Reset(CreateComponent("UI_TEXT_ITEM_NAME_TEMPLATE_NEW", false));
                    }
                    CCountedPointer<CText> nameText;
                    nameText.SetStaticCast(name);
                    nameText->SetValue(itemName, true, true);
                    name->SetName(CTCInventory::GetItemNameTableString());
                    parent->AddChildDynamically(name);
                }
            }
        }
        break;
    case UI_ACTION_UNLOAD_ITEM_DESC:
        if (action->UIActionParams.size() > 0) {
            CComponent* parent = static_cast<CComponent*>(action->UIActionParams.front());
            CComponent* description = parent->FindChild(CTCInventory::GetItemDescriptionString());
            if (description) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                description->SetDeletion(deletion);
            }
            CComponent* name = parent->FindChild(CTCInventory::GetItemNameTableString());
            if (name) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                name->SetDeletion(deletion);
            }
            CComponent* icon = parent->FindChild(CTCInventory::GetItemIconString());
            if (icon) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                icon->SetDeletion(deletion);
            }
        }
        break;
    case UI_ACTION_LOAD_WEAPON_MENU_ENTRY_NAME:
        if (action->UIActionParams.size() > 5) {
            std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
            CActionParamULONG* weapon = static_cast<CActionParamULONG*>(*it);
            long weaponIndex = weapon->Value;
            CComponent* entry = static_cast<CComponent*>(*++it);
            CActionParamCCountedPointer<CComponent>* titleChild = static_cast<CActionParamCCountedPointer<CComponent>*>(*++it);
            CActionParamCCountedPointer<CComponent>* augmentAction = static_cast<CActionParamCCountedPointer<CComponent>*>(*++it);
            CActionParamEBOOL* usable = static_cast<CActionParamEBOOL*>(*++it);
            CActionParamCCountedPointer<CComponent>* wieldAction = static_cast<CActionParamCCountedPointer<CComponent>*>(*++it);
            CComponent* positionParent = 0;
            if (CPlayerGui::Get().IsLiveGUIMode())
                positionParent = static_cast<CComponent*>(*++it);
            const bool loaded = entry->FindChild(CTCInventoryWeapons::GetWeaponTitleString()) != 0;
            if (loaded) {
                // Already built: an entry that can no longer be used or augmented loses its arrow and greys out.
                if (static_cast<CMenuEntry*>(entry)->IsMenuEntryAvailable() && !usable->Value) {
                    if (GetNumberAugmentations() > 0) {
                        const std::vector<long>& slots = PWeaponsInventory->GetPerItemDataForItem(weaponIndex)->Augmentations;
                        for (unsigned long i = 0; i < slots.size(); ++i) {
#pragma inline_depth(0) // inline budget: retail 0x004242F5 calls vector::operator[] out of line
                            if (slots[i] <= 0)
                                return;
#pragma inline_depth()
                        }
                    }
                    CComponent* arrow = entry->FindChild("PC_MENU_ENTRY_ARROW");
                    if (arrow) {
                        CDeletion deletion;
                        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                        arrow->SetDeletion(deletion);
                    }
                    entry->MakeUnavailableDynamically();
                }
                return;
            }
            CWideString name = CTCInventoryItem::GetDescription(weapon->Value);
            static_cast<CText*>(entry->FindFirstChildOfType(UI_TYPE_TEXT))->SetValue(name, true, true);
            CCountedPointer<CComponent> title;
            if (CPlayerGui::Get().IsLiveGUIMode()) {
                title.Reset(CreateComponent("PC_SUBTITLE", false));
                positionParent->AddPositionChild(title.Get());
            } else {
                title.Reset(CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW_ONE_OFF", false));
            }
            CCountedPointer<CText> titleText;
            title->SetName(CTCInventoryWeapons::GetWeaponTitleString());
            titleText.SetStaticCast(title);
            titleText->SetValue(name, true, true);
            entry->AddChildDynamically(title);
            CCountedPointer<CComponent> subList;
            if (CPlayerGui::Get().IsLiveGUIMode()) {
                subList.Reset(CreateComponent("PC_SUB_LIST", false));
            } else {
                subList.Reset(CreateComponent("UI_SUB_LIST", false));
            }
            CCountedPointer<CList> list;
            list.SetStaticCast(subList);
            list->AddNonScrollingChild(titleChild->Value);
            long graphic = CTCInventoryItem::GetEngineGraphic(weaponIndex).BankIndex;
            CCountedPointer<CComponent> icon;
            CCountedPointer<CComponent> listIcon;
            if (graphic) {
                if (CPlayerGui::Get().IsLiveGUIMode()) {
                    icon.Reset(CreateComponent("PC_ICON", false));
                    listIcon.Reset(CreateComponent("PC_ICON", false));
                    positionParent->AddPositionChild(icon.Get());
                    positionParent->AddPositionChild(listIcon.Get());
                } else {
                    icon.Reset(CreateComponent("UI_ONE_OFF_ICON_TRADE_TEMPLATE", false));
                    listIcon.Reset(CreateComponent("UI_ONE_OFF_ICON_TRADE_TEMPLATE_FOR_LIST", false));
                }
                if (icon->GetType() == UI_TYPE_SPRITE)
                    static_cast<CSprite*>(icon.Get())->SetGraphicIndex(graphic);
                if (listIcon->GetType() == UI_TYPE_SPRITE)
                    static_cast<CSprite*>(listIcon.Get())->SetGraphicIndex(graphic);
                entry->AddChildDynamically(icon);
                if (listIcon.Get())
                    list->AddNonScrollingChild(listIcon);
            }
            // One slot sprite per augmentation slot of the weapon, with the augmentation's sprite on the filled ones.
            bool hasFreeSlot = false;
            CDefPointer<const CThingObjectDef> def;
            CGameDefinitionManager::Get().GetDef(weaponIndex, &def);
            CDefPointer<const CObjectAugmentationsDef> augmentations;
            if (def->GetSubDef(&augmentations)) {
                const std::vector<long>& slots = PWeaponsInventory->GetPerItemDataForItem(weaponIndex)->Augmentations;
                for (unsigned long i = 0; i < slots.size(); ++i) {
                    CCharString slotTemplate;
                    CCharString augmentationTemplate;
                    switch (i) {
                    case 0:
                        slotTemplate = "PC_AUGMENTATION_FIRST_SLOT_TEMPLATE";
                        augmentationTemplate = "PC_AUGMENTATION_ON_WEAPON_FIRST_SLOT_TEMPLATE";
                        break;
                    case 1:
                        slotTemplate = "PC_AUGMENTATION_SECOND_SLOT_TEMPLATE";
                        augmentationTemplate = "PC_AUGMENTATION_ON_WEAPON_SECOND_SLOT_TEMPLATE";
                        break;
                    case 2:
                        slotTemplate = "PC_AUGMENTATION_THIRD_SLOT_TEMPLATE";
                        augmentationTemplate = "PC_AUGMENTATION_ON_WEAPON_THIRD_SLOT_TEMPLATE";
                        break;
                    case 3:
                        slotTemplate = "PC_AUGMENTATION_FOURTH_SLOT_TEMPLATE";
                        augmentationTemplate = "PC_AUGMENTATION_ON_WEAPON_FOURTH_SLOT_TEMPLATE";
                        break;
                    }
                    if (slotTemplate != "" && augmentationTemplate != "") {
                        CCountedPointer<CComponent> slot(CreateComponent(slotTemplate, false));
                        CCountedPointer<CComponent> augmentation;
#pragma inline_depth(0) // inline budget: retail 0x0042488D calls vector::operator[] out of line
                        if (slots[i] > 0) {
#pragma inline_depth()
                            CDefPointer<const CDefClassBase> augmentationDef;
#pragma inline_depth(0) // inline budget: retail 0x004248A8 calls vector::operator[] out of line
                            if (CGameDefinitionManager::Get().GetDef(slots[i], &augmentationDef)) {
#pragma inline_depth()
                                CDefPointer<const CAugmentationDef> augmentationSubDef;
                                if (augmentationDef->GetSubDef(&augmentationSubDef)) {
                                    augmentation.Reset(CreateComponent(augmentationTemplate, false));
                                    if (augmentation->GetType() == UI_TYPE_SPRITE)
                                        static_cast<CSprite*>(augmentation.Get())->SetGraphicIndex(augmentationSubDef->InventoryGraphic);
                                }
                            }
                        } else {
                            hasFreeSlot = true;
                        }
                        if (slot.Get()) {
                            if (positionParent)
                                positionParent->AddPositionChild(slot.Get());
                            entry->AddChildDynamically(slot);
                        }
                        if (augmentation.Get()) {
                            if (positionParent)
                                positionParent->AddPositionChild(augmentation.Get());
                            entry->AddChildDynamically(augmentation);
                        }
                    }
                }
            }
            const bool canAugment = hasFreeSlot && augmentAction->Value.Get() != 0 && GetNumberAugmentations() > 0;
            if (canAugment || usable->Value) {
                if (!CPlayerGui::Get().IsLiveGUIMode()) {
                    CCountedPointer<CComponent> back(CreateComponent("UI_BACK_FOR_LIST", false));
                    CCountedPointer<CComponent> select(CreateComponent("UI_SELECT_FOR_LIST", false));
                    list->AddNonScrollingChild(back);
                    list->AddNonScrollingChild(select);
                }
                long options = 0;
                if (usable->Value)
                    ++options;
                if (canAugment)
                    ++options;
                if (options > 0)
                    list->InitialiseOffsets(options);
                if (usable->Value)
                    list->AddScrollingChildAndSetOffsets(wieldAction->Value);
                if (canAugment)
                    list->AddScrollingChildAndSetOffsets(augmentAction->Value);
                entry->AddChildDynamically(subList);
                PWeaponsInventory->AddComponentToAvailableWeaponsEntries(entry);
            } else {
                CComponent* arrow = entry->FindChild("PC_MENU_ENTRY_ARROW");
                if (arrow) {
                    CDeletion deletion;
                    deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                    arrow->SetDeletion(deletion);
                }
                entry->MakeUnavailableDynamically();
            }
        }
        break;
    case UI_ACTION_LOAD_CLOTHING_MENU_ENTRY_NAME:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00424CA0 calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* clothing = static_cast<CActionParamULONG*>(*it);
            CComponent* entry = static_cast<CComponent*>(*++it);
            CComponent* positionParent = 0;
            if (CPlayerGui::Get().IsLiveGUIMode())
                positionParent = static_cast<CComponent*>(*++it);
            if (!entry->FindChild(CTCInventoryClothing::GetClothingTitleString())) {
                CWideString name = CTCInventoryItem::GetDescription(clothing->Value);
                static_cast<CText*>(entry->FindFirstChildOfType(UI_TYPE_TEXT))->SetValue(name, true, true);
                CCountedPointer<CComponent> title;
                if (CPlayerGui::Get().IsLiveGUIMode()) {
                    title.Reset(CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
                    positionParent->AddPositionChild(title.Get());
                } else {
                    title.Reset(CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
                }
                CCountedPointer<CText> titleText;
                title->SetName(CTCInventoryClothing::GetClothingTitleString());
                titleText.SetStaticCast(title);
                titleText->SetValue(name, true, true);
                entry->AddChildDynamically(title);
            }
        }
        break;
    case UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00424E7B calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CComponent* entry = static_cast<CComponent*>(*++it);
            CActionParamSLONG* quantity = static_cast<CActionParamSLONG*>(*++it);
            CText* text = static_cast<CText*>(entry->FindFirstChildOfType(UI_TYPE_TEXT));
            // Only an entry still showing the placeholder gets the item's name (and its quantity).
            if (text->GetValue() == L"INVALID NAME") {
                CWideString name = CTCInventoryItem::GetDescription(item->Value);
                if (quantity->Value > 1) {
                    name += CTCInventoryStats::GetXString();
                    name += GFIntToWideString(quantity->Value);
                }
                text->SetValue(name, true, true);
            }
        }
        break;
    case UI_ACTION_LOAD_SELL_ITEM_DESC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00424F75 calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CViewport* viewport = static_cast<CViewport*>(static_cast<CComponent*>(*++it));
            CComponent* parent = static_cast<CComponent*>(*++it);
            CComponent* positionParent = 0;
            if (PInventoryTrade->InventoryTradeOpen)
                positionParent = static_cast<CComponent*>(*++it);
            if (!parent->FindChild(CTCInventoryTrade::GetSellItemDescriptionString())) {
                long count = PInventoryTrade->GetNumberOfSellItems(parent);
                long itemIndex = item->Value;
                CWideString name = CTCInventoryItem::GetDescription(itemIndex);
                if (count > 0)
                    name += CTCInventoryStats::GetXString() + GFIntToWideString(count);
                CCountedPointer<CComponent> description(PInventoryTrade->ConstructTradeDescription(itemIndex, 1, false, false));
                if (PInventoryTrade->InventoryTradeOpen)
                    positionParent->AddPositionChild(description.Get());
                description->SetName(CTCInventoryTrade::GetSellItemDescriptionString());
                parent->AddChildDynamically(description);
                viewport->AddFakeChild(description.Get());
                CCountedPointer<CComponent> title;
                if (PInventoryTrade->InventoryTradeOpen) {
                    title.Reset(CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
                    positionParent->AddPositionChild(title.Get());
                } else {
                    title.Reset(CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
                }
                CCountedPointer<CText> titleText;
                titleText.SetStaticCast(title);
                titleText->SetValue(name, true, true);
                parent->AddChildDynamically(title);
                if (CTCInventoryItem::GetEngineGraphic(itemIndex).BankIndex != 0) {
                    CCountedPointer<CComponent> icon;
                    if (PInventoryTrade->InventoryTradeOpen) {
                        icon.Reset(CreateComponent("PC_ICON_INDEPENDANT", false));
                        positionParent->AddPositionChild(icon.Get());
                    } else {
                        icon.Reset(CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
                    }
                    if (icon->GetType() == UI_TYPE_SPRITE)
                        static_cast<CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(itemIndex).BankIndex);
                    icon->SetName(CTCInventoryTrade::GetSellItemIconString());
                    parent->AddChildDynamically(icon);
                }
            }
        }
        break;
    case UI_ACTION_UNLOAD_SELL_ITEM_DESC:
        if (action->UIActionParams.size() > 0) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00425357 calls list::begin out of line
            std::list<CActionParam*>::iterator first = params.begin();
#pragma inline_depth()
            CComponent* parent = static_cast<CComponent*>(*first);
            CComponent* description = parent->FindChild(CTCInventoryTrade::GetSellItemDescriptionString());
            if (description) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                description->SetDeletion(deletion);
            }
            CComponent* title;
            if (PInventoryTrade->InventoryTradeOpen)
                title = parent->FindChild("PC_SUBTITLE_INDEPENDANT");
            else
                title = parent->FindChild("UI_TEXT_TRADE_NAME_TEMPLATE_NEW");
            if (title) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                title->SetDeletion(deletion);
            }
            CComponent* icon = parent->FindChild(CTCInventoryTrade::GetSellItemIconString());
            if (icon) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                icon->SetDeletion(deletion);
            }
        }
        break;
    case UI_ACTION_LOAD_SELL_ACTION_MENU:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x0042545A calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CComponent* parent = static_cast<CComponent*>(*++it);
            if (PInventoryTrade->InventoryTradeOpen) {
                if (!parent->FindChild("PC_LIST_SELL_ACTIONS")) {
                    CTCInventoryTrade* inventory = PInventoryTrade;
                    inventory->ConstructSellActionsListPC(parent, item->Value, inventory->GetNumberOfSellItems(parent), true);
                }
            } else {
                if (!parent->FindChild("UI_LIST_SELL_ACTIONS")) {
                    CTCInventoryTrade* inventory = PInventoryTrade;
                    inventory->ConstructSellActionsList(parent, item->Value, inventory->GetNumberOfSellItems(parent), true);
                }
            }
        }
        break;
    case UI_ACTION_UNLOAD_SELL_ACTION_MENU:
        if (action->UIActionParams.size() > 0) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x0042552C calls list::begin out of line
            std::list<CActionParam*>::iterator first = params.begin();
#pragma inline_depth()
            CComponent* parent = static_cast<CComponent*>(*first);
            CComponent* list;
            if (PInventoryTrade->InventoryTradeOpen)
                list = parent->FindChild("PC_LIST_SELL_ACTIONS");
            else
                list = parent->FindChild("UI_LIST_SELL_ACTIONS");
            if (list) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                list->SetDeletion(deletion);
            }
            PInventoryTrade->RemoveFromItemSellList(parent);
        }
        break;
    case UI_ACTION_LOAD_BUY_ITEM_DESC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x004255E0 calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CViewport* viewport = static_cast<CViewport*>(static_cast<CComponent*>(*++it));
            CComponent* parent = static_cast<CComponent*>(*++it);
            CComponent* positionParent = 0;
            if (PInventoryTrade->InventoryTradeOpen)
                positionParent = static_cast<CComponent*>(*++it);
            if (!parent->FindChild(CTCInventoryTrade::GetBuyItemDescriptionString())) {
                long count = PInventoryTrade->GetNumberOfBuyItems(parent);
                long itemIndex = item->Value;
                CWideString name = CTCInventoryItem::GetDescription(itemIndex);
                if (count > 1 && !PInventoryTrade->IsSpecialCase(itemIndex))
                    name += CTCInventoryStats::GetXString() + GFIntToWideString(count);
                CCountedPointer<CComponent> description(PInventoryTrade->ConstructTradeDescription(itemIndex, 1, true, false));
                if (PInventoryTrade->InventoryTradeOpen)
                    positionParent->AddPositionChild(description.Get());
                description->SetName(CTCInventoryTrade::GetBuyItemDescriptionString());
                parent->AddChildDynamically(description);
                viewport->AddFakeChild(description.Get());
                CCountedPointer<CComponent> title;
                if (PInventoryTrade->InventoryTradeOpen) {
                    title.Reset(CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
                    positionParent->AddPositionChild(title.Get());
                } else {
                    title.Reset(CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
                }
                CCountedPointer<CText> titleText;
                titleText.SetStaticCast(title);
                titleText->SetValue(name, true, true);
                parent->AddChildDynamically(title);
                if (CTCInventoryItem::GetEngineGraphic(itemIndex).BankIndex != 0) {
                    CCountedPointer<CComponent> icon;
                    if (PInventoryTrade->InventoryTradeOpen) {
                        icon.Reset(CreateComponent("PC_ICON_INDEPENDANT", false));
                        positionParent->AddPositionChild(icon.Get());
                    } else {
                        icon.Reset(CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
                    }
                    if (icon->GetType() == UI_TYPE_SPRITE)
                        static_cast<CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(itemIndex).BankIndex);
                    icon->SetName(CTCInventoryTrade::GetBuyItemIconString());
                    parent->AddChildDynamically(icon);
                }
            }
        }
        break;
    case UI_ACTION_UNLOAD_BUY_ITEM_DESC:
        if (action->UIActionParams.size() > 0) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x004259C3 calls list::begin out of line
            std::list<CActionParam*>::iterator first = params.begin();
#pragma inline_depth()
            CComponent* parent = static_cast<CComponent*>(*first);
            CComponent* description = parent->FindChild(CTCInventoryTrade::GetBuyItemDescriptionString());
            if (description) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                description->SetDeletion(deletion);
            }
            CComponent* title;
            if (PInventoryTrade->InventoryTradeOpen)
                title = parent->FindChild("PC_SUBTITLE_INDEPENDANT");
            else
                title = parent->FindChild("UI_TEXT_TRADE_NAME_TEMPLATE_NEW");
            if (title) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                title->SetDeletion(deletion);
            }
            CComponent* icon = parent->FindChild(CTCInventoryTrade::GetBuyItemIconString());
            if (icon) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                icon->SetDeletion(deletion);
            }
        }
        break;
    case UI_ACTION_LOAD_BUY_ACTION_MENU:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00425AC6 calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CComponent* parent = static_cast<CComponent*>(*++it);
            if (PInventoryTrade->InventoryTradeOpen) {
                if (!parent->FindChild("PC_LIST_BUY_ACTIONS")) {
                    CTCInventoryTrade* inventory = PInventoryTrade;
                    inventory->ConstructBuyActionsListPC(parent, item->Value, inventory->GetNumberOfBuyItems(parent), true);
                }
            } else {
                // Retail looks the console list up under the sell list's name here.
                if (!parent->FindChild("UI_LIST_SELL_ACTIONS")) {
                    CTCInventoryTrade* inventory = PInventoryTrade;
                    inventory->ConstructBuyActionsList(parent, item->Value, inventory->GetNumberOfBuyItems(parent), true);
                }
            }
        }
        break;
    case UI_ACTION_UNLOAD_BUY_ACTION_MENU:
        if (action->UIActionParams.size() > 0) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00425B98 calls list::begin out of line
            std::list<CActionParam*>::iterator first = params.begin();
#pragma inline_depth()
            CComponent* parent = static_cast<CComponent*>(*first);
            CComponent* list;
            if (PInventoryTrade->InventoryTradeOpen)
                list = parent->FindChild("PC_LIST_BUY_ACTIONS");
            else
                list = parent->FindChild("UI_LIST_BUY_ACTIONS");
            if (list) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                list->SetDeletion(deletion);
            }
            PInventoryTrade->RemoveFromItemBuyList(parent);
        }
        break;
    case UI_ACTION_LOAD_WANTED_ITEM_DESC:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00425C4C calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CViewport* viewport = static_cast<CViewport*>(static_cast<CComponent*>(*++it));
            CComponent* parent = static_cast<CComponent*>(*++it);
            CComponent* positionParent = 0;
            if (PInventoryTrade->InventoryTradeOpen)
                positionParent = static_cast<CComponent*>(*++it);
            if (!parent->FindChild(CTCInventoryTrade::GetWantedItemDescriptionString())) {
                long itemIndex = item->Value;
                CWideString name = CTCInventoryItem::GetDescription(itemIndex);
                CCountedPointer<CComponent> description(PInventoryTrade->ConstructTradeDescription(itemIndex, 1, true, true));
                if (PInventoryTrade->InventoryTradeOpen)
                    positionParent->AddPositionChild(description.Get());
                description->SetName(CTCInventoryTrade::GetWantedItemDescriptionString());
                parent->AddChildDynamically(description);
                viewport->AddFakeChild(description.Get());
                CCountedPointer<CComponent> title;
                if (PInventoryTrade->InventoryTradeOpen) {
                    title.Reset(CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
                    positionParent->AddPositionChild(title.Get());
                } else {
                    title.Reset(CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
                }
                CCountedPointer<CText> titleText;
                titleText.SetStaticCast(title);
                titleText->SetValue(name, true, true);
                parent->AddChildDynamically(title);
                if (CTCInventoryItem::GetEngineGraphic(itemIndex).BankIndex != 0) {
                    CCountedPointer<CComponent> icon;
                    if (PInventoryTrade->InventoryTradeOpen) {
                        icon.Reset(CreateComponent("PC_ICON_INDEPENDANT", false));
                        positionParent->AddPositionChild(icon.Get());
                    } else {
                        icon.Reset(CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
                    }
                    if (icon->GetType() == UI_TYPE_SPRITE)
                        static_cast<CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(itemIndex).BankIndex);
                    icon->SetName(CTCInventoryTrade::GetWantedItemIconString());
                    parent->AddChildDynamically(icon);
                }
            }
        }
        break;
    case UI_ACTION_UNLOAD_WANTED_ITEM_DESC:
        if (action->UIActionParams.size() > 0) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00425FC1 calls list::begin out of line
            std::list<CActionParam*>::iterator first = params.begin();
#pragma inline_depth()
            CComponent* parent = static_cast<CComponent*>(*first);
            CComponent* description = parent->FindChild(CTCInventoryTrade::GetWantedItemDescriptionString());
            if (description) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                description->SetDeletion(deletion);
            }
            CComponent* title;
            if (PInventoryTrade->InventoryTradeOpen)
                title = parent->FindChild("PC_SUBTITLE_INDEPENDANT");
            else
                title = parent->FindChild("UI_TEXT_TRADE_NAME_TEMPLATE_NEW");
            if (title) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                title->SetDeletion(deletion);
            }
            CComponent* icon = parent->FindChild(CTCInventoryTrade::GetWantedItemIconString());
            if (icon) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                icon->SetDeletion(deletion);
            }
        }
        break;
    case UI_ACTION_LOAD_QUICK_MENU_EXPRESSIONS_ICON:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00426116 calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* item = static_cast<CActionParamULONG*>(*it);
            CComponent* parent = static_cast<CComponent*>(*++it);
            if (!parent->FindChild("UI_ICON_ON_LEFT_MENU_ENTRY") && CTCInventoryItem::GetEngineGraphic(item->Value).BankIndex != 0) {
                CCountedPointer<CComponent> icon(CreateComponent("UI_ICON_ON_LEFT_MENU_ENTRY", false));
                if (icon->GetType() == UI_TYPE_SPRITE)
                    static_cast<CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(item->Value).BankIndex);
                parent->AddChildDynamically(icon);
            }
        }
        break;
    case UI_ACTION_LOAD_FILE_DESC:
        if (action->UIActionParams.size() > 1) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00426228 calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamULONG* file = static_cast<CActionParamULONG*>(*it);
            CText* text = static_cast<CText*>(static_cast<CActionParamCCountedPointer<CComponent>*>(*++it)->Value.Get());
            if (text->GetValue() == L"INVALID FILE DESCRIPTION STRING") {
                CTCInGameMenu* menu;
                PPlayer->GetTC(TCI_IN_GAME_MENU, &menu);
                CWideString description;
                menu->ConstructSaveGameFileDescription(description, file->Value);
                text->SetValue(description, true, true);
            }
        }
        break;
    case UI_ACTION_LOAD_FILE_SAVEGAME_MINIMAP:
        if (action->UIActionParams.size() > 2) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00426315 calls list::begin out of line
            std::list<CActionParam*>::iterator it = params.begin();
#pragma inline_depth()
            CActionParamEBOOL* valid = static_cast<CActionParamEBOOL*>(*it);
            CSprite* minimap = static_cast<CSprite*>(static_cast<CActionParamCCountedPointer<CComponent>*>(*++it)->Value.Get());
            CActionParamULONG* file = static_cast<CActionParamULONG*>(*++it);
            unsigned long current = minimap->GetGraphicIndex();
            // Only a minimap still showing its default graphic is filled in.
            if (current == 0 || current == minimap->GetDefaultGraphicIndex()) {
                CTCInGameMenu* menu;
                PPlayer->GetTC(TCI_IN_GAME_MENU, &menu);
                if (valid->Value) {
                    const CPlayerGuiDef& guiDef = CPlayerGui::PeekGuiDef();
#pragma inline_depth(0) // inline budget: retail 0x004263D2 calls vector::operator[] out of line
                    const CCharString& region = menu->FileRegionMiniMapNames[file->Value];
#pragma inline_depth()
                    if (region != "") {
                        std::map<CCharString, long>::const_iterator found = guiDef.MiniMapGraphics.find(region);
                        if (found != guiDef.MiniMapGraphics.end()) {
                            unsigned long graphic = found->second;
                            if (graphic > 0)
                                minimap->SetGraphicIndex(graphic);
                        }
                    }
                } else {
                    minimap->SetGraphicIndex(GetMiscThingsDef()->SaveHeroGraphicIndex);
                }
            }
        }
        break;
    case UI_ACTION_TYPE_CHEAT_MORALITY: {
        CActionParamFloat* fraction = static_cast<CActionParamFloat*>(action->UIActionParams.back());
        CThing* hero = game->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroStats* stats;
        hero->GetTC(TCI_HERO_STATS, &stats);
        stats->SetMorality((long)(stats->GetMoralityMax() * fraction->Value));
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory))
            inventory->AddCloseInventoryEvent();
        break;
    }
    case UI_ACTION_TYPE_SCROLL_PHOTO_UP: {
        // Scroll the photo list by replaying the pad events with the list updated in between.
        MouseDisabled = true;
        CComponent* photos = static_cast<CComponent*>(action->UIActionParams.back());
        for (long i = 0; i < 5; ++i)
            photos->Update(1.0f);
        CMenuEntry::GSetForceProcessing(true);
        SetEvent(UI_EVENT_BACK);
        for (long j = 0; j < 5; ++j)
            photos->Update(1.0f);
        SetEvent(UI_EVENT_UP);
        for (long k = 0; k < 2; ++k)
            photos->Update(1.0f);
        SetEvent(UI_EVENT_SELECT);
        for (long l = 0; l < 50; ++l)
            photos->Update(1.0f);
        CMenuEntry::GSetForceProcessing(false);
        if (!CList::GPreviousSelected.empty())
            CList::GPreviousSelected.front()->SetLastInputTimeToBeNow();
        MouseDisabled = false;
        break;
    }
    case UI_ACTION_TYPE_SCROLL_PHOTO_DOWN: {
        MouseDisabled = true;
        CComponent* photos = static_cast<CComponent*>(action->UIActionParams.back());
        for (long i = 0; i < 5; ++i)
            photos->Update(1.0f);
        CMenuEntry::GSetForceProcessing(true);
        SetEvent(UI_EVENT_BACK);
        for (long j = 0; j < 5; ++j)
            photos->Update(1.0f);
        SetEvent(UI_EVENT_DOWN);
        for (long k = 0; k < 2; ++k)
            photos->Update(1.0f);
        SetEvent(UI_EVENT_SELECT);
        for (long l = 0; l < 50; ++l)
            photos->Update(1.0f);
        CMenuEntry::GSetForceProcessing(false);
        if (!CList::GPreviousSelected.empty())
            CList::GPreviousSelected.front()->SetLastInputTimeToBeNow();
        MouseDisabled = false;
        break;
    }
    case UI_ACTION_TYPE_LOAD_PHOTO: {
        // Load a saved photo (512x512 bitmap) into a fresh texture and show it on both sprites.
        std::list<CActionParam*>::iterator it = action->UIActionParams.begin();
        CSprite* photo = static_cast<CSprite*>(static_cast<CComponent*>(*it));
        CSprite* preview = static_cast<CSprite*>(static_cast<CComponent*>(*++it));
        CCountedPointer<CTexture>& photoTexture = photo->PTexture;
        if (!photoTexture.Get()) {
            CActionParamString* file = static_cast<CActionParamString*>(*++it);
            CCountedPointer<CTexture> texture(new CTexture);
            C2DExtentsI size(0x200, 0x200);
            texture->Initialise(size, 1, CPixelFormat(8, 8, 8, 8), 0, (ESurfacePool)1, false, false);
            bool loaded = texture->GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(file->Value, 3);
            if (loaded) {
                photoTexture = texture;
                preview->PTexture = texture;
            }
        }
        break;
    }
    case UI_ACTION_TYPE_PHOTO_CAPTION: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCPhotojournalCapture* capture;
        if (hero->GetTC(TCI_PHOTOJOURNAL_CAPTURE, &capture)) {
            CEditBox* caption = static_cast<CEditBox*>(static_cast<CComponent*>(action->UIActionParams.back()));
            CWideString text = caption->GetText();
            capture->OnCaptionEntered(text);
        }
        break;
    }
    case UI_ACTION_TYPE_DISCARD_PHOTO: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCPhotojournalCapture* capture;
        if (hero->GetTC(TCI_PHOTOJOURNAL_CAPTURE, &capture)) {
            capture->SetDiscardScreenshot(true);
            capture->ClosePhotojournalCapture();
        }
        break;
    }
    case UI_ACTION_TYPE_ACTIVATE_EDITBOX:
        static_cast<CEditBox*>(static_cast<CComponent*>(action->UIActionParams.back()))->SetActive(true);
        break;
    case UI_ACTION_TYPE_SET_EDITBOX_VALUES: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory))
            inventory->SetEditBoxValues();
        break;
    }
    case UI_ACTION_TYPE_DEACTIVATE_EDITBOX: {
        if (!game) {
            CFrontEndManager::GetInstance()->DeactivateAllEditBoxes(0);
            break;
        }
        bool keepValues = false;
        if (action->UIActionParams.size() != 0)
            keepValues = static_cast<CActionParamEBOOL*>(action->UIActionParams.back())->Value;
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory))
            inventory->DeactivateAllEditBoxes(keepValues);
        break;
    }
    case UI_ACTION_TYPE_OPEN_SCOREBOARD: {
        CGameEvent event(0x50);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_CHEAT_RENOWN: {
        CActionParamFloat* fraction = static_cast<CActionParamFloat*>(action->UIActionParams.back());
        CThing* hero = game->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroStats* stats;
        hero->GetTC(TCI_HERO_STATS, &stats);
        stats->ForceRenownLevelTo((long)(stats->RenownMaxLevel * fraction->Value));
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory))
            inventory->AddCloseInventoryEvent();
        break;
    }
    case UI_ACTION_TYPE_PLAY_SOUND:
        // Tutorial voice-over: stop whatever tutorial sound is still playing, then play this one once.
        if (!Restoring && action->UIActionParams.size() > 0) {
            CThing* hero = game->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventoryExperience* experience;
            hero->GetTC(TCI_INVENTORY_EXPERIENCE, &experience);
            CCharString sound = static_cast<CActionParamString*>(action->UIActionParams.front())->Value.ToCharString();
            if (experience && !experience->IsTutorialPlayed(sound)) {
                std::map<CCharString, unsigned long>& handles = experience->TutorialSoundHandles;
                for (std::map<CCharString, unsigned long>::iterator it = handles.begin(); it != handles.end(); ++it)
                    if (PSoundManager)
                        PSoundManager->Virtual09_00a3e3a0()->StopSample(it->second);
                handles[sound] = PlaySample(sound, 4);
            }
        }
        break;
    case UI_ACTION_TYPE_TAKE_QUEST_FOR_BOAST:
        if (action->ThingActionParams.size() > 0)
            PQuestInventory->TakeQuest(action->ThingActionParams.back());
        break;
    case UI_ACTION_TYPE_CLOSE_BOAST_MENU: {
        CThing* hero = game->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCBoastUI* boastUI;
        if (hero && hero->GetTC(TCI_BOAST_UI, &boastUI))
            boastUI->Deactivate();
        break;
    }
    case UI_ACTION_TYPE_SCROLL_LIST_UP: {
        CComponent* list = static_cast<CComponent*>(action->UIActionParams.back());
        list->ChangeTargetPositionDelta(C2DVector(0.0f, -25.0f), 0.0f, false);
        break;
    }
    case UI_ACTION_TYPE_SCROLL_LIST_DOWN: {
        CComponent* list = static_cast<CComponent*>(action->UIActionParams.back());
        list->ChangeTargetPositionDelta(C2DVector(0.0f, 25.0f), 0.0f, false);
        break;
    }
    case UI_ACTION_TYPE_CHOOSE_CLAN: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroOnlineScoreboard* scoreboard;
        if (hero->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard)) {
            std::list<CActionParam*>& params = action->UIActionParams;
#pragma inline_depth(0) // inline budget: retail 0x00426CD4 calls list::begin out of line
            std::list<CActionParam*>::iterator first = params.begin();
#pragma inline_depth()
            CActionParamString* clan = static_cast<CActionParamString*>(*first);
            CComponent* entry = static_cast<CComponent*>(action->UIActionParams.back());
            if (!entry->FindChild("PC_SUB_LIST"))
                scoreboard->OnPlayerSelectedClan(clan->Value, entry);
        }
        break;
    }
    case UI_ACTION_TYPE_SCOREBOARD_OK: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroOnlineScoreboard* scoreboard;
        if (hero->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard))
            scoreboard->OnOkClicked();
        break;
    }
    case UI_ACTION_TYPE_OPEN_PHOTOJOURNAL: {
        CGameEvent event(0x4a);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_EXIT_PHOTOJOURNAL_CAPTURE: {
        CThing* hero = game->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCPhotojournalCapture* capture;
        if (hero->GetTC(TCI_PHOTOJOURNAL_CAPTURE, &capture) && capture)
            capture->ClosePhotojournalCapture();
        break;
    }
    default:
        break;
    }
}

}
