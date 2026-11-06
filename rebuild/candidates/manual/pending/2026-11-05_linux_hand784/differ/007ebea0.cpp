// CTCActionUseBed::OnActionUse(CThing& user) -- the hero uses a bed: remember the user; refuse silently while the bed's building belongs to a village in limbo or hostile for crime, while the hero is missing or in melee, or while a fade is running; otherwise run the base use, record the user's player, find the bed's building (and whether one of the village's people lives or works there), then show the right subtitle: "can't sleep" when sleeping is disabled, the user is berserk or the bed is taken; the yes/no check for an owned bed, a villager's bed, or a bought (not rented) house; the tavern's priced offer; or the "illegal" warning; and post the bed game event
// Grade: DIFFER(2124v2145) -- prologue, owned-entity limbo test, the inline user / null IP temporaries, the owner site (out-of-line IP ctor 0x004A96F0 + operator= 0x004A9760) and the village loop are retail-shaped; residue: VC7.1 keeps the hero in ebp and the IP vtable in ebx (retail: user in ebp, hero spilled to [esp+1Ch], vtable immediate), and the FAIL / ask subtitle blocks cross-jump differently (retail: berserk FAIL alone, bed + neither FAIL shared after the UseableByHero test, CHECK/ILLEGAL tails jumping into one SetText)
// Next idea: find the source spelling that keeps the berserk FAIL apart from the bed FAIL (retail keeps them in separate blocks with different temp slots, maybe separate scopes) and then re-check the hero spill, which may follow from the block layout
// Leaf: ?OnActionUse@CTCActionUseBed@@UAEXAAVCThing@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EBEA0, 2145 bytes, thiscall `ret 4`, /GS cookie frame.
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10, CIntelligentPointer<CThing>
//   ctor 0x004A96F0 / operator= 0x004A9760 (out-of-line copies), CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer
//   0x00449970, CVectorMap::LowerBound 0x0040F020 (inlined GetTC), CTCVillageMember::GetPVillage 0x007EE540, CThingCreatureBase::IsInMelee
//   0x00661E80, CTCActionUseBase::OnActionUse 0x007EAB90, CCharString(const char*, long) 0x0099EBF0 / operator=(const char*) 0x0099EFE0 /
//   dtor 0x0099EAE0, CTCBase::PeekWorld 0x00686A80, CSubtitleRenderer::SetText (CCharString) 0x00492FB4 / (CWideString) 0x00492F69,
//   CDefClassBase::GetSubDef<CTavernDef> 0x007ECD00, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, NGameText::ConstructString
//   0x009C7B70, CWideString(const wchar_t*) 0x0099B6B0 / dtor 0x0099B510, CTCBase::AddToGlobalTCs 0x00686ED0, CMemoryBuffer::Copy
//   0x009F1760, CMainGameComponent::AddGameEvent 0x0041649C, __security_check_cookie 0x00BFE9F9.
// Layout: Ego_r.pdb CTCActionUseBed (WaitingForFadeOut +0x14, WaitingForFadeIn +0x15, WaitingForConfirmation +0x17,
//   PlayerNumberOfThingUsingBed +0x18, Cost +0x1C, UseableByHero +0x20, OwnedByHero +0x21, PBuilding +0x23, PThingUser +0x2B);
//   CTCOwnedEntity POwner +0x0D; CTCBed Empty +0x0C; CTCBuyableHouse OwnedByPlayer +0x11 / Rented +0x12; CTavernDef BedCost +0x28;
//   retail CTCVillage (STLport lists): VillagePeople +0x2C, IsEnemyBecauseOfCrime +0x7C, LaggyLimbo +0xB3; CThingAICreature
//   HomeBuilding +0x168 / WorkBuilding +0x170; CWorld HeroSleepingEnabled +0xD9; CMainGameComponent PTextBank +0x14,
//   PCutSceneMainFont +0x1614C; CPlayerGui SubtitleRenderer +0x18; CPlayer PlayerCharacter +0x34.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"

class CTavernDef : public CSubDefClassBase {                    // RTTI .?AVCTavernDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTavernDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad[0x28 - sizeof(CSubDefClassBase)];
    long BedCost;                                                   // +0x28
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CTavernDef>(CDefPointer<const CTavernDef>* out) const   // 0x007ECD00
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#include "engine/CThing.h"
class CThingBaseDef : public CDefClassBase { };
inline const CThingBaseDef& CThing::PeekBaseDef() const { return *PBaseDef; }

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for <CThing>
public:
    CIntelligentPointer() { SetPItem(0); }
    CIntelligentPointer(T& item) { SetPItem((CBaseObject*)&item); }
    CIntelligentPointer(T* item);                       // 0x004A96F0 (out of line for <CThing>)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760 (out of line for <CThing>)
    void Assign(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
};
#pragma pack(push, 1)
class CThingBuilding : public CThing { };
class CThingCreatureBase : public CThing { public:
    bool IsInMelee() const;                             // 0x00661E80
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CThingAICreature : public CThingCreatureBase { public:
    unsigned char _pad98[0x168 - sizeof(CThing)];
    CIntelligentPointer<CThingBuilding> HomeBuilding;   // +0x168
    CIntelligentPointer<CThingBuilding> WorkBuilding;   // +0x170
};
#pragma pack(pop)
class CTCVillage { public:
    unsigned char _pad00[0x2c];
    std::list<CIntelligentPointer<CThingAICreature> > VillagePeople;   // +0x2c
    unsigned char _pad30[0x7c - 0x30];
    bool IsEnemyBecauseOfCrime;                         // +0x7c
    unsigned char _pad7d[0xb3 - 0x7d];
    bool LaggyLimbo;                                    // +0xb3
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:
    unsigned char _pad00[0x0d];
    CIntelligentPointer<CThing> POwner;                 // +0x0d
};
#pragma pack(pop)
class CTCVillageMember { public:
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};
class CTCBed { public:
    unsigned char _pad00[0x0c];
    bool Empty;                                         // +0x0c
};
class CTCBuyableHouse { public:
    unsigned char _pad00[0x11];
    bool OwnedByPlayer;                                 // +0x11
    bool Rented;                                        // +0x12
};
class CPlayer { public:
    unsigned char _pad00[0x34];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    CThingPlayerCreature* GetPPlayerCharacter() const { return PlayerCharacter.Get(); }
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                           // 0x00449970
};
class CWorld { public:
    unsigned char _pad00[0xd9];
    bool HeroSleepingEnabled;                           // +0xd9
};
class CFontBank;
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
bool __fastcall ConstructString(CWideString& text, long value);         // 0x009C7B70
}
struct CMemoryBuffer { void Copy(unsigned long length, const void* data); };   // 0x009F1760
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;   // +0x14
    unsigned char _pad1c[0x1614C - 0x1C];
    CCountedPointer<CFontBank> PCutSceneMainFont;      // +0x1614C
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CCharString& text, const CCharString& a, const CCharString& b, const CCharString& c,
                 unsigned long duration, float delay, bool d, bool e);         // 0x00492FB4
    void SetText(CFontBank* font, const CWideString& text, const CWideString& a, const CWideString& b, const CWideString& c,
                 unsigned long duration, float delay, bool d, bool e);         // 0x00492F69
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80
    CPlayerManager& GetPlayerManager();                 // 0x00686D40
public:
    void AddToGlobalTCs();                              // 0x00686ED0
};
class CTCActionUseBase : public CTCBase { public:
    CDefPointer<const CDefClassBase> PActionUseDef;     // +0x0c
    bool Usable;                                        // +0x10
    virtual void OnActionUse(CThing& user);             // 0x007EAB90
};
#pragma pack(push, 1)
class CTCActionUseBed : public CTCActionUseBase { public:
    bool WaitingForFadeOut;                             // +0x14
    bool WaitingForFadeIn;                              // +0x15
    bool InAtomicAction;                                // +0x16
    bool WaitingForConfirmation;                        // +0x17
    long PlayerNumberOfThingUsingBed;                   // +0x18
    long Cost;                                          // +0x1c
    bool UseableByHero;                                 // +0x20
    bool OwnedByHero;                                   // +0x21
    bool ForceTrespassOnceFadedOut;                     // +0x22
    CIntelligentPointer<CThing> PBuilding;              // +0x23
    CIntelligentPointer<CThing> PThingUser;             // +0x2b
    virtual void OnActionUse(CThing& user);
};
#pragma pack(pop)

void CTCActionUseBed::OnActionUse(CThing& user)
{
    PThingUser.Assign(CIntelligentPointer<CThing>(user));
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    CTCOwnedEntity* owned;
    if (ParentThing->GetTC(TCI_OWNED_ENTITY, &owned)) {
        CTCVillageMember* member;
        if (owned->POwner.Get() && owned->POwner.Get()->GetTC(TCI_VILLAGE_MEMBER, &member)) {
            if (member->GetPVillage() && (member->GetPVillage()->LaggyLimbo || member->GetPVillage()->IsEnemyBecauseOfCrime))
                return;
        }
    }
    if (!hero || hero->IsInMelee() || WaitingForFadeOut || WaitingForFadeIn)
        return;
    CTCActionUseBase::OnActionUse(user);
    PlayerNumberOfThingUsingBed = user.GetPlayerNumber();
    CCharString message("");
    CThing* parent = ParentThing;
    PBuilding.Assign(CIntelligentPointer<CThing>());
    bool occupied = false;
    if (!PeekWorld().HeroSleepingEnabled) {
        g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    {
        CTCOwnedEntity* owned;
        if (parent->GetTC(TCI_OWNED_ENTITY, &owned)) {
            PBuilding = CIntelligentPointer<CThing>(owned->POwner.Get());
            CTCVillageMember* member;
            if (PBuilding.Get()->GetTC(TCI_VILLAGE_MEMBER, &member)) {
                CTCVillage* village = member->GetPVillage();
                if (village) {
                    for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = village->VillagePeople.begin();
                         it != village->VillagePeople.end(); ++it) {
                        if (it->Get()->HomeBuilding.Get() == PBuilding.Get() || it->Get()->WorkBuilding.Get() == PBuilding.Get()) {
                            occupied = true;
                            break;
                        }
                    }
                }
                else
                    occupied = true;
            }
            else
                occupied = true;
        }
        else
            occupied = true;
    }
    CTCBed* bed = 0;
    parent->GetTC(TCI_BED, &bed);
    if (user.HasTC(TCI_BERSERK)) {
        g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    if (bed && !bed->Empty) {
        g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    if (OwnedByHero) {
        message = "TEXT_GUI_USEBED_CHECK";
        Cost = 0;
        g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), message, "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
    } else if (UseableByHero) {
        if (!PBuilding.Get()) {
            message = "TEXT_GUI_USEBED_ILLEGAL";
            Cost = -1;
            g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), message, "",
                                                   "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        } else if (!occupied) {
            message = "TEXT_GUI_USEBED_CHECK";
            Cost = 0;
            g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), message, "",
                                                   "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        } else {
            CTCBuyableHouse* house;
            if (PBuilding.Get()->HasTC(TCI_BUYABLE_HOUSE) && PBuilding.Get()->GetTC(TCI_BUYABLE_HOUSE, &house)
                && house->OwnedByPlayer && !house->Rented) {
                message = "TEXT_GUI_USEBED_CHECK";
                Cost = 0;
                g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), message, "",
                                                       "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
            } else if (PBuilding.Get()->HasTC(TCI_TAVERN)) {
                message = "TEXT_GUI_USEBED_COST";
                CDefPointer<const CTavernDef> def;
                PBuilding.Get()->PeekBaseDef().GetSubDef(&def);
                Cost = def->BedCost;
                CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(message);
                NGameText::ConstructString(text, Cost);
                g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), text, L"",
                                                       L"TEXT_GUI_TAVERNGAMES_GENERIC_YES", L"TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
            } else {
                message = "TEXT_GUI_USEBED_ILLEGAL";
                Cost = -1;
                g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), message, "",
                                                       "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
            }
        }
    } else {
        g_pPlayerGui->SubtitleRenderer.SetText(CMainGameComponent::Get()->PCutSceneMainFont.Get(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    AddToGlobalTCs();
    WaitingForConfirmation = true;
    CGameEvent event(9);
    event.Player = (signed char)PlayerNumberOfThingUsingBed;
    event.AddUBYTE(1);
    CMainGameComponent::Get()->AddGameEvent(event);
}
