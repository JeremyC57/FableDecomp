// CTCBuyableHouse::FrameUpdate() -- (CTCBase slot 9) set up the dressing once, then every 75 frames: create or kill the main sign post (announcing a newly buyable house on the subtitle line), create or kill the dressing sign post for an owned house with a dearer next level, keep the next rent day while the house is rented and lived in, and pay out rent when due
// Leaf: ?FrameUpdate@CTCBuyableHouse@@UAEXXZ
// Flags: none
// Retail 0x006C1250, 638 bytes, `ret`. Identity: CTCBase vtable slot 9 is FrameUpdate (engine/CTCBase.h, 0x006868A0 the base body;
//   CTCBuyableHouse vtable 0x0125D6A4 slot 9); Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCBuyableHouse@@UAEXXZ (the catalog's
//   CTCBuyableHouse::vfunc_9 / the batch's `Update`).
// Callees: SetupDressingAtLevel 0x006C0330, CWorld::GetFrame 0x0049D870 (static __fastcall), NeedsMainSignPost 0x006BFB90,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCBase::GetWorld 0x00686A70 -> CWorld::GetGameScriptInterface() (inline, +0x38)
//   -> CGameScriptInterface::IsHeroControlledByPlayer() const (slot 394, +0x628), CreateMainSignPost 0x006C0F00,
//   IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 (__fastcall), CPlayerGui::UpdateMiniMapMarkers 0x004378C0 on the
//   singleton 0x013B8790, CCharString 0x0099EBF0 / operator= 0x0099EFE0 / dtor 0x0099EAE0, CThing::PeekComponent 0x004C7A70 ->
//   CMainGameComponent::PeekCutSceneMainFont() (inline, +0x1614C), CSubtitleRenderer::SetText(const CFontBank&, const CCharString&, ...)
//   0x00492ED1 and Pause 0x0048DD94 on the GUI's SubtitleRenderer (+0x18), CThing::Kill 0x004C9B80, CreateDressingSignPost
//   0x006C0F70, CVectorMap::LowerBound const 0x004365B0, CTCVillageMember::GetPVillage 0x007EE540, std::count_if<..,
//   HasBuildingAsHomeOrWork> 0x006600EC (__stdcall in retail, as the landed instance), CTCBase::PeekWorld 0x00686A80 ->
//   CWorld::PeekGameTimeManager().GetDayCount() (inline, +0x1C -> +0x14), CreateRentMoney 0x006C0C90.
// Layout: engine/CTCBuyableHouse.h (FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE: the Ego_r.pdb members), engine/CThing.h (UsingComponents
//   +0x20, Components +0x44), engine/CPlayerGui.h (SubtitleRenderer +0x18; DisplayingText +0xE5 = the singleton's byte +0xFD),
//   engine/CWorld.h, engine/CTCVillage.h (VillagePeople +0x2C, read through FableWin.pdb's inline PeekVillagePeople() const). CBuyableHouseDef has no shared header (engine/_quarantine only):
//   local model with the Ego_r.pdb members FrameUpdate reads (Price vector +0x28, DaysBetweenRentPayments +0x40).
// Shape: the same-TU helpers IsHouseBeingUsed(bool) const / GetRentsDue / GetPriceForNextDressLevel / NeedsDressingSignPost are
//   defined out of line in this TU (VC7.1 /O2 auto-inlines them: IsHouseBeingUsed(false) keeps only the home-or-work count_if);
//   the font goes straight into the SetText argument. count_if gets the const list's const_iterators (PeekVillagePeople): their
//   copy is trivial, so retail pushes the two node words (0x006C1449); the non-const iterator's user copy constructor would build
//   them in their slots. HasTC is __forceinline (FABLE_RETAIL_CTHING_HASTC_FORCEINLINE): retail tests the bitset words in place.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CSUBTITLERENDERER_PAUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGui.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCBuyableHouse.h"

class CThingAICreature;

class CBuyableHouseDef : public CSubDefClassBase {           // RTTI .?AVCBuyableHouseDef@@ : CSubDefClassBase (Ego_r.pdb members)
public:
    bool IsBuyable;                                          // +0x25
    bool InitiallyEmpty;                                     // +0x26
    std::vector<long> Price;                                 // +0x28
    std::vector<long> Rent;                                  // +0x34
    long DaysBetweenRentPayments;                            // +0x40
    long MaxRentBags;                                        // +0x44
    bool IsScripted;                                         // +0x48
    bool IsBuyableWithoutSignpost;                           // +0x49
};

class HasBuildingAsWork {          // Ego_r.pdb {CThing* Building}; FableWin ??0HasBuildingAsWork@@QAE@ABVCThing@@@Z; operator() out of line
public:
    HasBuildingAsWork(const CThing& building) : Building(const_cast<CThing*>(&building)) {}
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const;
    CThing* Building;
};
class HasBuildingAsHomeOrWork {    // Ego_r.pdb {CThing* Building}; FableWin ??0HasBuildingAsHomeOrWork@@QAE@ABVCThing@@@Z; operator() 0x0065FBE1
public:
    HasBuildingAsHomeOrWork(const CThing& building) : Building(const_cast<CThing*>(&building)) {}
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const;
    CThing* Building;
};
namespace std {
// The STLport count_if instances 0x006C1C70 (HasBuildingAsWork) / 0x006600EC (HasBuildingAsHomeOrWork), `ret 0Ch` in retail
// (the landed instance rows declare them __stdcall). Declaration only: the row calls the instances.
template <class _InputIter, class _Predicate> long __stdcall count_if(_InputIter __first, _InputIter __last, _Predicate __pred);
}

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // 0x004CC340 <CIntelligentPointer<CThing> >

bool CTCBuyableHouse::IsHouseBeingUsed(bool as_workplace) const
{
    const CTCVillageMember* member;
    if (ParentThing->PeekTC(&member)) {
        CTCVillage* village = member->GetPVillage();
        if (village) {
            long count;
            if (as_workplace)
                count = std::count_if(village->PeekVillagePeople().begin(), village->PeekVillagePeople().end(), HasBuildingAsWork(*ParentThing));
            else
                count = std::count_if(village->PeekVillagePeople().begin(), village->PeekVillagePeople().end(), HasBuildingAsHomeOrWork(*ParentThing));
            if (count > 0)
                return true;
        }
    }
    return false;
}

long CTCBuyableHouse::GetRentsDue() const
{
    if (!PDef.IsNull() && Rented) {
        if (PeekWorld().PeekGameTimeManager().GetDayCount() >= DayNextRentIsDue) {
            long rents = (PeekWorld().PeekGameTimeManager().GetDayCount() - DayNextRentIsDue) / PDef->DaysBetweenRentPayments + 1;
            if (rents > 0)
                return rents;
        }
    }
    return 0;
}

long CTCBuyableHouse::GetPriceForNextDressLevel() const
{
    if (!PDef.IsNull() && CurrentDressLevel >= 0 && (unsigned long)(CurrentDressLevel + 1) < PDef->Price.size())
        return PDef->Price[CurrentDressLevel + 1] - PDef->Price[CurrentDressLevel];
    return -1;
}

bool CTCBuyableHouse::NeedsDressingSignPost()
{
    return OwnedByPlayer && GetPriceForNextDressLevel() > 0;
}

void CTCBuyableHouse::FrameUpdate()
{
    if (!InitiallySetupDressLevel) {
        SetupDressingAtLevel(CurrentDressLevel);
        InitiallySetupDressLevel = true;
    }
    long frame = CWorld::GetFrame();
    if (frame < FrameToNextCheckIfWeNeedToCreateAnything)
        return;
    FrameToNextCheckIfWeNeedToCreateAnything = frame + 75;
    if (NeedsMainSignPost()) {
        if (!MainSignPost.Get() && GetWorld().GetGameScriptInterface().IsHeroControlledByPlayer()
            && !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
            CreateMainSignPost();
            if (IS_THING_ALIVE(MainSignPost) && !HasPreviouslyBeenBuyable) {
                CPlayerGui::Get().UpdateMiniMapMarkers(false);
                CCharString text("TEXT_GUI_BUYABLE_HOUSE_AVAILABLE");
                if (ParentThing->HasTC(TCI_SHOP) && !ParentThing->HasTC(TCI_TAVERN))
                    text = "TEXT_GUI_BUYABLE_HOUSE_AVAILABLE_SHOP";
                CPlayerGui::Get().GetSubtitleRenderer().SetText(ParentThing->PeekComponent().PeekCutSceneMainFont(), text, 0x22, 0.0f, false, true);
                CPlayerGui::Get().GetSubtitleRenderer().Pause(true);
            }
            HasPreviouslyBeenBuyable = true;
        }
    } else if (MainSignPost.Get()) {
        MainSignPost->Kill(true);
        CPlayerGui::Get().UpdateMiniMapMarkers(false);
    }
    if (NeedsDressingSignPost()) {
        if (!DressingSignPost.Get())
            CreateDressingSignPost();
    } else if (DressingSignPost.Get()) {
        DressingSignPost->Kill(true);
    }
    if (IsHouseBeingUsed(false) && Rented) {
        if (DayNextRentIsDue == 0x7fffffff)
            DayNextRentIsDue = PeekWorld().PeekGameTimeManager().GetDayCount() + 1;
    } else {
        DayNextRentIsDue = 0x7fffffff;
    }
    if (GetRentsDue() > 0 || VirtualMoneyBags > 0)
        CreateRentMoney();
}