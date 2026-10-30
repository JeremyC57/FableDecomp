// CV_BordelloScript::WatchForBordelloStatus() -- script thread: whenever the Bordello region is unloaded, wait until it is loaded again, then mark the hero as having left (clear the partying and magician-sleeping flags)
// Leaf: WatchForBordelloStatus
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E3A030, length 263 bytes, plain `ret`.
// Label: catalog "CQ_AmbushTradersScript::WatchForAllTradersDead" is propagated; the body is started as the thread
// "WatchForBordelloStatus" (0x012E3918) by the CV_BordelloScript code at 0x00E39E64 (functor slot +0x34) and touches
// CV_BordelloScript's fields.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; through the game script interface (+0x40, retail vtable
// 0x01260F0C): IsRegionLoaded(const CCharString&) (slot 12, +0x30, 0x0088E300), NewScriptFrame (slot 7, +0x1c,
// 0x006E7410); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0. Literal "Bordello" 0x012E39A4.
// Layout: Ego_r.pdb CV_BordelloScript - 0x14 (landed Init 0x00E399D0): HeroPartying +0x48, MagicianSleeping +0x4a,
// PlayerHasLeftRegion +0x4e.
// Shape: `if (!loaded) { if (term) return; while (!loaded) { yield; if (term) return; } if (term) return; ... }`
// inside the outer `while (!term) { ...; yield; }` -- VC rotates both loops and duplicates the inner `while`
// condition; with the implicit "Bordello" temporary dying inside each condition the `!` result is materialised
// (neg/sbb/inc) in bl; a `for (;;) { if (term) return; if (loaded) break; yield; }` wait drops one test. The two flag
// clears reuse al (known zero after the terminating test).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                // 0x00CB7940
};

namespace NScript {
class CV_BordelloScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    bool HeroPartying;                               // +0x48
    bool HeroTricking;                               // +0x49
    bool MagicianSleeping;                           // +0x4a
    bool CutscenePlaying;                            // +0x4b
    bool PlayerOwned;                                // +0x4c
    bool BecomeNunnery;                              // +0x4d
    bool PlayerHasLeftRegion;                        // +0x4e
    void WatchForBordelloStatus();
};
} // namespace NScript
using NScript::CV_BordelloScript;

void NScript::CV_BordelloScript::WatchForBordelloStatus()
{
    while (!IsActiveThreadTerminating()) {
        if (!m_pInterface->IsRegionLoaded("Bordello")) {
            if (IsActiveThreadTerminating())
                return;
            while (!m_pInterface->IsRegionLoaded("Bordello")) {
                m_pInterface->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            PlayerHasLeftRegion = true;
            HeroPartying = false;
            MagicianSleeping = false;
        }
        m_pInterface->NewScriptFrame();
    }
}