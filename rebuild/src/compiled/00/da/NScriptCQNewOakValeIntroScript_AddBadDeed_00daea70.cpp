// CQ_NewOakValeIntroScript::AddBadDeed(long deed) -- the hero did a bad deed in the Oakvale intro: count it, take away the per-deed morality, and the first time (with no good deeds yet) show the first-bad-deed message and log the map tutorial, otherwise show the bad-deed message once per kind of deed
// Leaf: ?AddBadDeed@CQ_NewOakValeIntroScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DAEA70, length 383 bytes (+ int3 padding), `ret 4`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (landed), CCharString::CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, the file-static __fastcall AddLogbookTutorialEntry(const CCharString&)
// 0x00CBE9EE (landed); through the game script interface (+0x40, vtable 0x01260F0C): NewScriptFrame (slot 7),
// MsgIsGameInfoClickedPast (40), DisplayGameInfo(const CCharString&) (115), GiveHeroMorality(float) (156).
// Data: pGlobalScriptDef 0x0143E90C OVI_MoralityChangePerDeed at retail +0xD64 (PDB +0xE14, the retail def sits 0xB0 lower
// here, as the landed OVBR rows show); strings 0x012D8774 "TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED", 0x012D875C
// "TEXT_QST_LOG_BASICS_MAP", 0x012D8738 "TEXT_QST_048_SCRMSG_DID_BAD_DEED".
// Layout: retail = Ego_r.pdb - 0x14 (as the landed ctor 0x00DAAC00): GoodDeedsPerformed +0x54, BadDeedsPerformed
// +0x58, WhichBadDeedsPerformed +0xFC (bool per deed).
// Shape: the script wait idiom (`while (!msg) { frame; if (terminating) return; } if (terminating) return;`); the two
// arms are an if/else with ONE `WhichBadDeedsPerformed[deed] = true` after them (VC7.1 duplicates the store into each
// arm and keeps `deed` in edi from the top; an early `return` in the first arm leaves `deed` in memory and adds a
// frame); each string is a named CCharString local in its own block (built in the dead `deed` argument slot).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void __fastcall AddLogbookTutorialEntry(const CCharString& tag);   // 0x00CBE9EE

class CScriptDef { public:
    unsigned char _pad000[0xd64];
    float         OVI_MoralityChangePerDeed;            // +0xd64 (PDB +0xe14)
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

class CScriptBase { public:                                    // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

namespace NScript {
class CQ_NewOakValeIntroScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x54 - 0x48];
    long                  GoodDeedsPerformed;           // +0x54 (PDB +0x68)
    long                  BadDeedsPerformed;            // +0x58 (PDB +0x6c)
    unsigned char         _pad5c[0xfc - 0x5c];
    bool                  WhichBadDeedsPerformed[8];    // +0xfc
    void AddBadDeed(long deed);
};
} // namespace NScript
using NScript::CQ_NewOakValeIntroScript;

void NScript::CQ_NewOakValeIntroScript::AddBadDeed(long deed)
{
    ++BadDeedsPerformed;
    GSI->GiveHeroMorality(-pGlobalScriptDef->OVI_MoralityChangePerDeed);
    if (BadDeedsPerformed == 1 && GoodDeedsPerformed == 0) {
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString text("TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED");
            GSI->DisplayGameInfo(text);
        }
        while (!GSI->MsgIsGameInfoClickedPast()) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString tag("TEXT_QST_LOG_BASICS_MAP");
            AddLogbookTutorialEntry(tag);
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        if (!WhichBadDeedsPerformed[deed]) {
            if (IsActiveThreadTerminating())
                return;
            {
                CCharString text("TEXT_QST_048_SCRMSG_DID_BAD_DEED");
                GSI->DisplayGameInfo(text);
            }
            while (!GSI->MsgIsGameInfoClickedPast()) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    WhichBadDeedsPerformed[deed] = true;
}