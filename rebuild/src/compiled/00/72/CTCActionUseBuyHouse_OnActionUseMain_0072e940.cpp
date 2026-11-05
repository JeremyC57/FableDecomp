// CTCActionUseBuyHouse::OnActionUseMain(CTCBuyableHouse* house, long money) -- unless the hero's buy-house tutorial takes over, ask the hero to buy the house (or tell him he cannot afford it) or to sell it
// Leaf: ?OnActionUseMain@CTCActionUseBuyHouse@@QAEXPAVCTCBuyableHouse@@J@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail 0x0072E940, 512 bytes (`ret 8`). Callees: CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer
// @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
// (inlined GetInterface, CTCHero = 0x29), CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0,
// CWideString() @ 0x0099AED0, CTCBuyableHouse::GetPrice @ 0x006C0120, CCharString(const char*, long) @ 0x0099EBF0,
// CDataBank::GetTextBySymbol @ 0x009C95E0 (CMainGameComponent::Get() 0x013B86A0 -> +0x14), CWideString::operator+= @ 0x0099B8D0,
// CWideString::operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0, ConstructString @ 0x009C7B70
// (__fastcall), CWideString::operator!=(const wchar_t*) @ 0x0099B150 (L"" 0x0122D70C), CSubtitleRenderer::SetText
// @ 0x00492E4B ((&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790 -> +0x18, font at +0x1614C).
// Layout: engine/CTCBuyableHouse.h (OwnedByPlayer +0x11, MaxDressLevel +0x20); CTCActionUseBuyHouse as the landed
// OnActionUseRent 0x0072E810 (WaitingForConfirmation +0x18, HouseQuestion +0x1c); CThing Dead bit 0 of +0x91.
// Shape: as the sibling OnActionUseRent: the symbol and the looked-up text temporaries sit in the dead argument
// slots; the confirm flag is a named bool tested before the SetText flags; each MARITAL/plain symbol pick is an
// if/else of two whole `text += GetTextBySymbol("...")` statements that VC7.1 cross-jumps (`push -1 ; jle ;
// push A ; jmp ; push B` then one shared call), not a ternary inside the call (that loads the pointer into eax).
#include <stddef.h>
#include "engine/CTCBuyableHouse.h"

class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                      // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCBase { protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40 (Ego_r.pdb CTCBase, protected)
};
class CTCHero { public:
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};

bool __fastcall ConstructString(CWideString* text, long value);   // 0x009C7B70
class CFontBank;
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;  // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x14];
    CDataBank*    PTextBank;                                 // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank*    PFont;                                     // +0x1614C
    CDataBank* GetTextBank() const { return PTextBank; }
};
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool clickPast,
                 bool pauseGame);                            // 0x00492E4B
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                      // +0x18
};

struct CTCBuyableHouseCalls : CTCBuyableHouse {
    long GetPrice();                                         // 0x006C0120
};

enum EHouseQuestion { HOUSE_QUESTION_NONE = 0, HOUSE_QUESTION_BUY = 1, HOUSE_QUESTION_RENT = 2 };
class CTCActionUseBuyHouse : public CTCBase {
public:
    void*          __vftable;                                // +0x00 (CTCBase)
    void*          ParentThing;                              // +0x04
    unsigned char  _pad08[0x18 - 0x08];
    bool           WaitingForConfirmation;                   // +0x18
    EHouseQuestion HouseQuestion;                            // +0x1c
    void OnActionUseMain(CTCBuyableHouse* house, long money);
};

void CTCActionUseBuyHouse::OnActionUseMain(CTCBuyableHouse* house, long money)
{
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead()) {
        CTCHero* heroTC;
        if (hero->GetTC(TCI_HERO, &heroTC) && heroTC->GiveTutorialForCategory(0x23, false))
            return;
    }
    HouseQuestion = HOUSE_QUESTION_BUY;
    CWideString text;
    bool confirm = true;
    if (!house->OwnedByPlayer) {
        long price = ((CTCBuyableHouseCalls*)house)->GetPrice();
        if (price != -1 && price <= money) {
            if (house->MaxDressLevel > 0)
                text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_BUY");
            else
                text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_BUY");
        } else {
            if (house->MaxDressLevel > 0)
                text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_CANNOT_AFFORD");
            else
                text += CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_CANNOT_AFFORD");
            confirm = false;
        }
    } else {
        if (house->MaxDressLevel > 0)
            text = CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_SELL");
        else
            text = CMainGameComponent::Get()->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_SELL");
    }
    ConstructString(&text, ((CTCBuyableHouseCalls*)house)->GetPrice());
    if (text != L"") {
        if (confirm) {
            WaitingForConfirmation = true;
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer.SetText(*CMainGameComponent::Get()->PFont, text, 4, 0.0f, false, true);
        } else {
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer.SetText(*CMainGameComponent::Get()->PFont, text, 2, 0.0f, false, true);
        }
    }
}