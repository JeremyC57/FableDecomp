// CTCActionUseBuyHouse::OnActionUseMain(CTCBuyableHouse* house, long money) -- unless the hero's buy-house tutorial takes over, ask the hero to buy the house (or tell him he cannot afford it) or to sell it
// Retail 0x0072E940, 512 bytes (`ret 8`). Callees: the player-manager accessor @ 0x00686D40, CPlayerManager::GetMainPlayer
// @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
// (inlined GetInterface, CTCHero = 0x29), CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0,
// CWideString() @ 0x0099AED0, CTCBuyableHouse::GetPrice @ 0x006C0120, CCharString(const char*, long) @ 0x0099EBF0,
// CDataBank::GetTextBySymbol @ 0x009C95E0 (g_pMainGameComponent 0x013B86A0 -> +0x14), CWideString::operator+= @ 0x0099B8D0,
// CWideString::operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0, ConstructString @ 0x009C7B70
// (__fastcall), CWideString::operator!=(const wchar_t*) @ 0x0099B150 (L"" 0x0122D70C), CSubtitleRenderer::SetText
// @ 0x00492E4B (g_pPlayerGui 0x013B8790 -> +0x18, font at +0x1614C).
// Layout: engine/CTCBuyableHouse.h (OwnedByPlayer +0x11, MaxDressLevel +0x20); CTCActionUseBuyHouse as the landed
// OnActionUseRent 0x0072E810 (WaitingForConfirmation +0x18, HouseQuestion +0x1c); CThing Dead bit 0 of +0x91.
// Shape: as the sibling OnActionUseRent: the symbol and the looked-up text temporaries sit in the dead argument
// slots; the confirm flag is a named bool tested before the SetText flags; each MARITAL/plain symbol pick is an
// if/else of two whole `text += GetTextBySymbol("...")` statements that VC7.1 cross-jumps (`push -1 ; jle ;
// push A ; jmp ; push B` then one shared call), not a ternary inside the call (that loads the pointer into eax).
#include <stddef.h>
#include "engine/CTCBuyableHouse.h"

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                         // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                      // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                       // 0x00449970
};
CPlayerManager* GetPlayerManager();                 // 0x00686D40
class CTCHero { public:
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                           // 0x0099AED0
    ~CWideString();                                          // 0x0099B510
    const CWideString& operator+=(const CWideString& other); // 0x0099B8D0
    const CWideString& operator=(const CWideString& other);        // 0x0099B7D0
    bool operator!=(const wchar_t* text) const;              // 0x0099B150
};
bool __fastcall ConstructString(CWideString* text, long value);   // 0x009C7B70
class CFontBank;
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;  // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
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
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                      // +0x18
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
extern CPlayerGui* g_pPlayerGui;                             // 0x013B8790

struct CTCBuyableHouseCalls : CTCBuyableHouse {
    long GetPrice();                                         // 0x006C0120
};

enum EHouseQuestion { HOUSE_QUESTION_NONE = 0, HOUSE_QUESTION_BUY = 1, HOUSE_QUESTION_RENT = 2 };
class CTCActionUseBuyHouse {
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
    CThing* hero = GetPlayerManager()->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead) {
        CTCHero* heroTC;
        if (hero->GetInterface(&heroTC, 0x29) && heroTC->GiveTutorialForCategory(0x23, false))
            return;
    }
    HouseQuestion = HOUSE_QUESTION_BUY;
    CWideString text;
    bool confirm = true;
    if (!house->OwnedByPlayer) {
        long price = ((CTCBuyableHouseCalls*)house)->GetPrice();
        if (price != -1 && price <= money) {
            if (house->MaxDressLevel > 0)
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_BUY");
            else
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_BUY");
        } else {
            if (house->MaxDressLevel > 0)
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_CANNOT_AFFORD");
            else
                text += g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_CANNOT_AFFORD");
            confirm = false;
        }
    } else {
        if (house->MaxDressLevel > 0)
            text = g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_MARITAL_HOME_SELL");
        else
            text = g_pMainGameComponent->GetTextBank()->GetTextBySymbol("TEXT_GUI_BUYABLE_HOUSE_SELL");
    }
    ConstructString(&text, ((CTCBuyableHouseCalls*)house)->GetPrice());
    if (text != L"") {
        if (confirm) {
            WaitingForConfirmation = true;
            g_pPlayerGui->SubtitleRenderer.SetText(*g_pMainGameComponent->PFont, text, 4, 0.0f, false, true);
        } else {
            g_pPlayerGui->SubtitleRenderer.SetText(*g_pMainGameComponent->PFont, text, 2, 0.0f, false, true);
        }
    }
}