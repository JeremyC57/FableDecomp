// CQ_AmbushTradersScript::AddBanditHealthBar(const CScriptThing& bandit) -- add a green quest health bar for a hired bandit (icon "HUD_QUEST_ICON_BANDIT", numbered "_0<n>" from the second bandit on, a random 2..6 past six) and count it; -1 if the thread is terminating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F07A80, length 251 bytes, `ret 4`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCharString::operator+=(const char*)
// 0x0099F100, operator+=(const CCharString&) 0x0099F0A0, GFIntToCharString(long) 0x0099F830 (`__fastcall`, hidden result
// in ecx), rand 0x00BFEB16, CScriptBase::IsActiveThreadTerminating 0x00CB7940 (x3); through the game script interface
// (+0x40): DisplayQuestInfo(bool) (slot 321, +0x504, 0x00891920), AddQuestInfoBarHealth(const CScriptThing&, const
// CRGBColour&, const CCharString&, float) (slot 325, +0x514, 0x00891AD0). Literals 0x012C88C8, "_0" 0x0126DCBC.
// Layout: Ego_r.pdb CQ_AmbushTradersScript - 0x14: HiredBanditCount +0x8c.
// Shape: the icon string is a function-scope local; the colour is a CRGBColour temporary (R, G, B, A stores, the 0xff
// in al) sharing the number temporary's frame word; the three terminating exits cross-jump onto one destructor call.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator+=(const char* text);         // 0x0099F100
    const CCharString& operator+=(const CCharString& other); // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830 (hidden result in ecx)
extern "C" int __cdecl rand();                         // 0x00BFEB16 (MSVCR71)

class CRGBColour { public:                                    // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CScriptThing;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x44];
    long                  HiredBanditCount;          // +0x8c
    long AddBanditHealthBar(const CScriptThing& bandit);
};
} // namespace NScript
using NScript::CQ_AmbushTradersScript;

long NScript::CQ_AmbushTradersScript::AddBanditHealthBar(const CScriptThing& bandit)
{
    CCharString icon("HUD_QUEST_ICON_BANDIT");
    if (HiredBanditCount > 0) {
        if (IsActiveThreadTerminating())
            return -1;
        icon += "_0";
        long number = HiredBanditCount + 1;
        if (number > 6) {
            if (IsActiveThreadTerminating())
                return -1;
            number = rand() % 5 + 2;
        }
        icon += GFIntToCharString(number);
    } else {
        if (IsActiveThreadTerminating())
            return -1;
        m_pInterface->DisplayQuestInfo(true);
    }
    long id = m_pInterface->AddQuestInfoBarHealth(bandit, CRGBColour(0, 255, 0, 255), icon, 1.0f);
    ++HiredBanditCount;
    return id;
}