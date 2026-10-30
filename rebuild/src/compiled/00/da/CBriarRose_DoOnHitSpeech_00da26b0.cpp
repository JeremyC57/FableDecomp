// CBriarRose::DoOnHitSpeech() -- Briar Rose's line when hit: three times in four the default hit line, otherwise one of three lines picked by the minion-camp script's timer (more than 48: _START, more than 24: _MIDDLE, else _END); said through DoSpeech unless the script is done
// Leaf: DoOnHitSpeech
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA26B0, 244 bytes (plain `ret`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (line prefix 0x012D711C), rand @ 0x00BFEB16, IsDone
//          @ 0x00F35B30 (manifest label propagated), the timer service (global 0x0143E8F8) slot 90 (+0x168, GetTimer),
//          CCharString::operator+=(const char*) @ 0x0099F100 (suffixes 0x012CD3B8 / 0x012D7114 / 0x012D710C),
//          CCharString::operator=(const char*) @ 0x0099EFE0 (the default line 0x012D70E8), CBriarRose::DoSpeech
//          @ 0x00DA2610, ~CCharString @ 0x0099EAE0.
// Layout: the landed sibling DoOnSpeakToSpeech (0x00DA27B0) model (Ego_r.pdb CBriarRose, ParentClass +0x14 whose timer
//          sits at +0x60).
// Shape: `rand() % 4 == 0` (signed remainder: `and 80000003h ; jns ; dec ; or -4 ; inc`) guards the timer lines,
//          which repeat DoOnSpeakToSpeech's chain after one extra IsDone test; the default arm is the `else` (laid out last).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator+=(const char* text);          // 0x0099F100
    const CCharString& operator=(const char* text);           // 0x0099EFE0
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
    void DoOnHitSpeech();
};

extern "C" int __cdecl rand();                  // 0x00BFEB16

void CBriarRose::DoOnHitSpeech()
{
    CCharString line("TEXT_QST_B08_BRIAR_ONHIT");  // 0x012D711C
    if (rand() % 4 == 0) {
        if (IsActiveThreadTerminating())
            return;
        if (GetTime(ParentClass->CampTimer) > 48) {
            if (IsActiveThreadTerminating())
                return;
            line += "_START";                      // 0x012CD3B8
        } else if (GetTime(ParentClass->CampTimer) > 24) {
            if (IsActiveThreadTerminating())
                return;
            line += "_MIDDLE";                     // 0x012D7114
        } else {
            if (IsActiveThreadTerminating())
                return;
            line += "_END";                        // 0x012D710C
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        line = "TEXT_QST_B08_BRIAR_ONHIT_DEFAULT";   // 0x012D70E8
    }
    DoSpeech(line);
}