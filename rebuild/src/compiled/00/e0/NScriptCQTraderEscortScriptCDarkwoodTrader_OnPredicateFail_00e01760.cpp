// NScript::CQ_TraderEscortScript::CDarkwoodTrader::OnPredicateFail() -- when the trader's timer is still running:
// one trader fewer, stop the escort timer, then have the nearest "DarkwoodTrader" comment HERO_KILLED_TRADER (killed by
// the hero), TRADER_DIED (two left) or LAST_TRADER (one left); finally clear the byte at +0x10b of the state at +0x18.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E01760, length 416 bytes (0x195 code + int3 pad).
// Callees: g_pGameScriptInterface (0x0143E8F8) GetTimer slot 90 / SetTimer slot 89; 0x0099EBF0 CCharString(const char*,
// long); CScriptThing slot 18 MsgIsKilledBy; CGameScriptInterface (+4) slot 77 GetNearestWithScriptName (by value);
// 0x00E01900 CQ_TraderEscortScript::MakeTraderComment(const CCharString&, const CScriptThing&, ETraderComment)
// (mangled name from the label TSVs); 0x004AA840 ~CScriptThing; 0x0099EAE0 ~CCharString. Slots: ghidra_out/cgsi_slots_aligned.tsv.
// Layout: descriptive (no PDB class): game +4, CScriptThing +8, parent script +0x14 (timer +0x48, traders left +0x50),
// state pointer +0x18, timer +0x2c. ETraderComment enumerator names unknown (values 0 and 1 used).
// Shape: implicit-literal CCharString arguments; the nested by-value GetNearestWithScriptName temporary is destroyed after
// the call. Matched first try.
// Leaf: OnPredicateFail
class CCharString { public:
    char* m_pString;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

namespace NScript {
class CQ_TraderEscortScript { public:
    enum ETraderComment { TRADER_COMMENT_0, TRADER_COMMENT_1 };
    unsigned char _pad00[0x48];
    long m_TraderTimer;                                  // +0x48
    unsigned char _pad4c[0x4];
    long m_TradersLeft;                                  // +0x50
    bool MakeTraderComment(const CCharString& text, const CScriptThing& trader, ETraderComment comment); // 0x00E01900
    struct CDarkwoodTrader;
};
struct STraderState {
    unsigned char _pad00[0x10b];
    bool m_flag10b;                                      // +0x10b
};
struct CQ_TraderEscortScript::CDarkwoodTrader {
    virtual void v00();
    CGameScriptInterface* m_pGame;                       // +0x04
    CScriptThing m_Thing;                                // +0x08
    CQ_TraderEscortScript* m_pScript;                    // +0x14
    STraderState* m_pState;                              // +0x18
    unsigned char _pad1c[0x10];
    long m_Timer;                                        // +0x2c
    void OnPredicateFail();
};

void CQ_TraderEscortScript::CDarkwoodTrader::OnPredicateFail()
{
    if (g_pGameScriptInterface->GetTimer(m_Timer) > 0) {
        --m_pScript->m_TradersLeft;
        g_pGameScriptInterface->SetTimer(m_pScript->m_TraderTimer, 0);
        if (m_Thing.MsgIsKilledBy("SCRIPT_NAME_HERO"))
            m_pScript->MakeTraderComment("HERO_KILLED_TRADER", m_pGame->GetNearestWithScriptName(m_Thing, "DarkwoodTrader"), TRADER_COMMENT_0);
        else if (m_pScript->m_TradersLeft == 2)
            m_pScript->MakeTraderComment("TRADER_DIED", m_pGame->GetNearestWithScriptName(m_Thing, "DarkwoodTrader"), TRADER_COMMENT_1);
        else if (m_pScript->m_TradersLeft == 1)
            m_pScript->MakeTraderComment("LAST_TRADER", m_pGame->GetNearestWithScriptName(m_Thing, "DarkwoodTrader"), TRADER_COMMENT_0);
        m_pState->m_flag10b = false;
    }
}
}