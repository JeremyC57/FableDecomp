// CQR_EscortTraderScript::EndMission() -- close the Greatwood/Orchard Farm route, remove the quest card, then either reward the hero (renown, quest completed) or fail the quest
// Leaf: ?EndMission@CQR_EscortTraderScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1E670, length 293 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const wchar_t*)
// 0x0099B6B0 / ~CWideString 0x0099B510, CScriptBase::IsActiveThreadTerminating 0x00CB7940 (twice, one per arm);
// through the game script interface (+0x40, retail vtable 0x01260F0C): MiniMapAllowRouteBetweenRegions (slot 354,
// +0x588, 0x0088E3D0), GetActiveQuestName (slot 655, +0xa3c, 0x00891880, by value), RemoveQuestCardFromHero (slot 294,
// +0x498, 0x008914E0), GiveHeroRenownPoints(long) (slot 161, +0x284, 0x008990C0), SetQuestAsCompleted (slot 288,
// +0x480, 0x00892F80), SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) (slot 289, +0x484,
// 0x00892FD0). Literals 0x012C5664 "OrchardFarm", 0x012C5650 "GreatwoodEntrance", L"" 0x0122D70C.
// Data: the script def @ 0x0143E90C, EscortTraderRenown at retail +0x9bc (Ego_r.pdb 0xa5c - 4 - 4 per container).
// Layout: Ego_r.pdb CQR_EscortTraderScript - 0x14: MissionSucceeded +0x48, MissionFailed +0x49.
// Shape: every string is an implicit conversion temporary (built before the interface load, right to left);
// each arm repeats its own IsActiveThreadTerminating early return, sharing one exit block.
typedef unsigned short wchar_t;                     // VC7.1 default: wchar_t is not a native type
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                  // 0x0099B6B0
    ~CWideString();                                    // 0x0099B510
};
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

class CScriptDef { public:
    unsigned char _pad0[0x9bc];
    long EscortTraderRenown;                         // +0x9bc
};
extern CScriptDef* pGlobalScriptDef;                     // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                // 0x00CB7940
};

struct CQR_EscortTraderScript : CScriptBase {
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    bool                  MissionSucceeded;          // +0x48
    bool                  MissionFailed;             // +0x49
    void EndMission();
};

void CQR_EscortTraderScript::EndMission()
{
    m_pInterface->MiniMapAllowRouteBetweenRegions("GreatwoodEntrance", "OrchardFarm", true);
    m_pInterface->RemoveQuestCardFromHero(m_pInterface->GetActiveQuestName());
    if (!MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        m_pInterface->GiveHeroRenownPoints(pGlobalScriptDef->EscortTraderRenown);
        MissionSucceeded = true;
        m_pInterface->SetQuestAsCompleted(m_pInterface->GetActiveQuestName(), false, false, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        m_pInterface->SetQuestAsFailed(m_pInterface->GetActiveQuestName(), false, L"", false);
    }
}