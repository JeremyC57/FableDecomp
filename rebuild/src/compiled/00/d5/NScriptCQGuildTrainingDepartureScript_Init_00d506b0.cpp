// CQ_GuildTrainingDepartureScript::Init() -- teleport the hero to the departure start marker, fade the screen back in and reset the result fields
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D506B0, length 176 bytes; vtable 0x012CF670 slot 3 (factory "Q_GuildTrainingDeparture" /
// "S_QGTD", 0x58 bytes = Ego_r.pdb 0x6c - 0x14).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
// and through the quest's game script interface (+0x40): GetThingWithScriptName (slot 72), GetHero (slot 70),
// EntityTeleportToThing (slot 472), FadeScreenIn (slot 374).
// Layout: Finished +0x48, MeleeGrade +0x4c, SkillGrade +0x50, WillGrade +0x54 (Ego_r.pdb offsets - 0x14).
// Shape: the marker name is an implicit conversion temporary, so its CCharString is built before any argument is
// evaluated; arguments then evaluate right to left (the `false`, the CScriptThing temporary returned through the hidden
// pointer, GetHero()). The temporary's inline destructor (vtable store, counted-pointer release, ~CBaseClass) runs
// before the string's; the zero register used for `false` is reused for the four field clears at the end.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

namespace NScript {
class CQ_GuildTrainingDepartureScript : public CQuestScriptBase {
public:
    bool Finished;                                       // +0x48
    long MeleeGrade;                                     // +0x4c
    long SkillGrade;                                     // +0x50
    long WillGrade;                                      // +0x54
    void Init();
};
} // namespace NScript
using NScript::CQ_GuildTrainingDepartureScript;

void NScript::CQ_GuildTrainingDepartureScript::Init()
{
    GSI->EntityTeleportToThing(GSI->GetHero(), GSI->GetThingWithScriptName("HeroDepartureStartMarker"), false);
    GSI->FadeScreenIn();
    Finished = false;
    MeleeGrade = 0;
    SkillGrade = 0;
    WillGrade = 0;
}