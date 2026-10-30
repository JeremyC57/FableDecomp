// CV_MayorsInvitationScript::Init() -- clears the quest flags and counters and looks up the Oakvale hero spawn point ("OakvaleHSP") into destinationHSP
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6BD0, 222 bytes (padded to 224), plain `ret`. Slot 3 of vtable 0x012F0528.
// Callees: CGameScriptInterface slot 72 GetThingWithScriptName(const CCharString&) (+0x120, 0x008A7D60, CScriptThing by
//   value through the hidden pointer), CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   ~CBaseClass 0x0099A2E0 (the temporary's base), operator delete 0x00BFE9BC (the counted block). String "OakvaleHSP"
//   0x012F053C.
// Shape: field clears in source order with one zero register; the literal is an implicit conversion temporary; the
//   CScriptThing assignment is the inline counted-pointer operator= (source Info/Data read into locals first, release
//   of the old block re-reading the member, add-ref of the new); the returned temporary's inline destructor re-stores
//   the CScriptThing vtable, releases and zeroes its block and calls the base destructor.
// Layout (retail = Ego_r.pdb - 0x14): flags +0x48..+0x50, SecretsLearned +0x54, WhichQuest +0x58,
//   MonitorThunderHeroDuelRunning +0x5c, destinationHSP +0x60 (PImp Data +0x64, Info +0x68).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                     // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_MayorsInvitationScript {
public:
    unsigned char _base[0x40];                     // CScriptBase
    CGameScriptInterface* Game;                    // +0x40
    void* Data;                                    // +0x44
    bool MissionSucceeded;                         // +0x48
    bool MissionFailed;                            // +0x49
    bool MissionOver;                              // +0x4a
    bool HeroAbortedQuest;                         // +0x4b
    bool HeroLearnedSecret;                        // +0x4c
    bool HeroReturnedFromParanoidQuest;            // +0x4d
    bool HeroHasDefeatedThunder;                   // +0x4e
    bool HeroHasLostToThunder;                     // +0x4f
    bool QuestCardGiven;                           // +0x50
    long SecretsLearned;                           // +0x54
    long WhichQuest;                               // +0x58
    long MonitorThunderHeroDuelRunning;            // +0x5c
    CScriptThing destinationHSP;                   // +0x60
    void Init();
};
} // namespace NScript
using NScript::CV_MayorsInvitationScript;
void NScript::CV_MayorsInvitationScript::Init()
{
    MissionSucceeded = false;
    MissionFailed = false;
    MissionOver = false;
    HeroAbortedQuest = false;
    SecretsLearned = 0;
    MonitorThunderHeroDuelRunning = 0;
    HeroLearnedSecret = false;
    WhichQuest = 0;
    HeroHasDefeatedThunder = false;
    HeroHasLostToThunder = false;
    HeroReturnedFromParanoidQuest = false;
    QuestCardGiven = false;
    destinationHSP = Game->GetThingWithScriptName("OakvaleHSP");
}