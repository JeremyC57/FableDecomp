// CAC_Owner::GetRandomSpeech() -- pick one of 10 owner speech lines at random, never either of the last two, yielding a script frame per rejected pick; the empty string when the script is done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E362B0, length 210 bytes (`ret 4`, CCharString by hidden pointer).
// Class: manifest label CAC_Owner kept (an entity-script class; the same body sits in three script modules at
// 0x00E2E440 / 0x00E362B0 / 0x00E53CB0 with different counts and table offsets -- the label is propagated there).
// Callees: CCharString default ctor @ 0x0099E4B0, copy ctor @ 0x0099EC30, operator= @ 0x0099EFB0, dtor @ 0x0099EAE0,
// rand @ 0x00BFEB16, the script's IsDone @ 0x00F35B30, the game script interface (+4) slot 7 (+0x1c).
// Shape: `for (;;) { if (IsActiveThreadTerminating()) return none; pick; if (ok) break; yield; }` then IsDone re-checked around the
// bookkeeping (a `while (!IsActiveThreadTerminating())` loop flips the branch layout); an empty string, a copy of it returned on every IsDone exit, and the chosen line copied out last (no NRVO:
// each return copy-constructs the hidden result); the line table is a CCharString array at +0x74 of the record (+0x14).
#define SPEECH_COUNT 10
#define SPEECH_TABLE_OFFSET 0x74
namespace NScript { class CPredicateBase; }
class CCharString
{ public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

extern "C" int __cdecl rand();                      // 0x00BFEB16 (MSVCR71)

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CSpeechRecord                                // the owner's speech table
{
    unsigned char _pad00[SPEECH_TABLE_OFFSET];
    CCharString Lines[SPEECH_COUNT];
};

class CEntityScriptBase
{
public:
    virtual void v00();
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CAC_Owner
: public CEntityScriptBase {
    CGameScriptInterface* GSI;                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CSpeechRecord* Record;                          // +0x14
    unsigned char _pad18[0x1c - 0x18];
    long LastSpeech;                                // +0x1c
    long PreviousSpeech;                            // +0x20
    CCharString GetRandomSpeech();
};

CCharString CAC_Owner::GetRandomSpeech()
{
    CCharString empty;
    CCharString none(empty);
    CCharString speech;
    long index;
    for (;;)
    {
        if (IsActiveThreadTerminating())
            return none;
        index = rand() % SPEECH_COUNT;
        if (index != LastSpeech && index != PreviousSpeech)
            break;
        GSI->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return none;
    PreviousSpeech = LastSpeech;
    LastSpeech = index;
    if (IsActiveThreadTerminating())
        return none;
    speech = Record->Lines[index];
    return speech;
}