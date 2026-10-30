// CBriarRose::DoOnSpeakToSpeech() -- Briar Rose's reply when spoken to: picks one of three lines by the minion-camp script's timer (more than 48: _START, more than 24: _MIDDLE, else _END) and says it through DoSpeech unless the script is done
// Leaf: DoOnSpeakToSpeech
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA27B0, 176 bytes incl. padding (plain `ret`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (line prefix 0x012D7138), the timer service (global
//          0x0143E8F8) slot 90 (+0x168, GetTimer), IsDone @ 0x00F35B30 (manifest label propagated), CCharString::operator+=
//          (const char*) @ 0x0099F100 (suffixes 0x012CD3B8 / 0x012D7114 / 0x012D710C), CBriarRose::DoSpeech @ 0x00DA2610,
//          CCharString::~CCharString @ 0x0099EAE0.
// Layout = Ego_r.pdb CBriarRose (80 bytes, the landed DoSpeech model): ParentClass +0x14 (CQ_MinionCampScript*), whose
//          timer sits at +0x60.
// Shape: the parent's timer is read through an inline CTimer getter taking the timer by reference (`add eax,60h ; mov eax,[eax]`).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator+=(const char* text);          // 0x0099F100
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
class CTimer { public:
    long TimerIndex;                          // +0x00
};
inline long GetTime(const CTimer& timer) { return g_pGameScriptInterface->GetTimer(timer.TimerIndex); }

struct CQ_MinionCampScript {
    unsigned char _pad[0x60];
    CTimer        CampTimer;                  // +0x60
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CBriarRose : public CEntityScriptBase {
public:
    void*                 __vftable;          // +0x00
    void*                 Game;               // +0x04
    unsigned char         Me[0x0c];           // +0x08
    CQ_MinionCampScript*  ParentClass;        // +0x14
    void*                 PMasterData;        // +0x18
    void DoSpeech(const CCharString& line);   // 0x00DA2610
    void DoOnSpeakToSpeech();
};

void CBriarRose::DoOnSpeakToSpeech()
{
    CCharString line("TEXT_QST_B08_BRIAR_ONSPEAKTO");  // 0x012D7138
    if (GetTime(ParentClass->CampTimer) > 48) {
        if (IsActiveThreadTerminating())
            return;
        line += "_START";                         // 0x012CD3B8
    } else if (GetTime(ParentClass->CampTimer) > 24) {
        if (IsActiveThreadTerminating())
            return;
        line += "_MIDDLE";                         // 0x012D7114
    } else {
        if (IsActiveThreadTerminating())
            return;
        line += "_END";                         // 0x012D710C
    }
    DoSpeech(line);
}