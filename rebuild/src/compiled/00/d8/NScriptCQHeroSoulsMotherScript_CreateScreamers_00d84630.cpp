// CQ_HeroSoulsMotherScript::CreateScreamers() -- start the super-screamer timer, gather the ScreamerHere points, show the quest bar and, until the bar is full, keep creating screamers at random points while fewer than the allowed number are alive; then remove the bar and record the mother's soul as collected
// Leaf: ?CreateScreamers@CQ_HeroSoulsMotherScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D84630, length 662 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, rand 0x00BFEB16, ~vector<CScriptThing>
//   0x008AC970 (out of line), ~CBaseClass 0x0099A2E0 / operator delete 0x00BFE9BC (inline ~CScriptThing),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing::GetPos (slot 6); through the interface (+0x40):
//   NewScriptFrame (7), GetAllThingsWithScriptName (75), CreateCreature (91), DisplayQuestInfo (321), AddQuestInfoBar
//   (324), UpdateQuestInfoBar (332), RemoveQuestInfoElement (338); the timer service (global 0x0143E8F8) SetTimer (89).
// Data: the script def 0x0143E90C: HS_ScreamerCount +0xc70 (float), HS_ScreamersAtOnce +0xc74,
//   HS_SecondsBeforeNewSuperScreamer +0xc84.
// Layout (retail = Ego_r.pdb - 0x14, the csargs map 12 bytes): ScreamersAlive +0x58, BarToFill +0x5c, BarID +0x60,
//   NewSuperScreamerTimer +0x68; PMasterData (+0x44) CQ_SunnyvaleMasterData::CollectedSoulFromMother +0x66.
// Shape: the CScriptThing model of the landed CV_ChapelOfEvilScript::WatchForWeaponUnsheathed 0x00E6A340 (the discarded
//   CreateCreature result is destroyed inline); CRGBColour as the landed CQ_SecretPassageScript::EndMission 0x00DF0A50.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                              // 0x008AC970 for CScriptThing
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class CRGBColour { public:                                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
extern "C" int __cdecl rand();                                         // 0x00BFEB16
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;        // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
class CScriptDef { public:
    unsigned char _pad0[0xc70];
    float HS_ScreamerCount;                                 // +0xc70
    long  HS_ScreamersAtOnce;                               // +0xc74
    unsigned char _padc78[0xc84 - 0xc78];
    long  HS_SecondsBeforeNewSuperScreamer;                 // +0xc84
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x66];
    bool CollectedSoulFromMother;                           // +0x66
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HeroSoulsMotherScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                           // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                    // +0x44
    bool                    PlayedMotherCS;                 // +0x48
    unsigned char           csargs[0xc];                    // +0x4c map<CCharString, CCharString>
    long                    ScreamersAlive;                 // +0x58
    float                   BarToFill;                      // +0x5c
    long                    BarID;                          // +0x60
    bool                    QuestStartScreened;             // +0x64
    bool                    GotSuperScreamer;               // +0x65
    CTimer                  NewSuperScreamerTimer;          // +0x68
    void CreateScreamers();
};
void CQ_HeroSoulsMotherScript::CreateScreamers()
{
    NewSuperScreamerTimer = pGlobalScriptDef->HS_SecondsBeforeNewSuperScreamer;
    std::vector<CScriptThing> screamerPoints;
    Game->GetAllThingsWithScriptName("ScreamerHere", screamerPoints);
    Game->DisplayQuestInfo(true);
    BarID = Game->AddQuestInfoBar(pGlobalScriptDef->HS_ScreamerCount, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255),
                                  "HUD_QUEST_ICON_UNDEAD", "", 1.0f);
    Game->UpdateQuestInfoBar(BarID, (float)ScreamersAlive, -1.0f, -1.0f);
    while (BarToFill < pGlobalScriptDef->HS_ScreamerCount) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ScreamersAlive < pGlobalScriptDef->HS_ScreamersAtOnce) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateCreature("CREATURE_NW_SCREAMER_01", screamerPoints[rand() % screamerPoints.size()].GetPos(), "HS_Screamer", true);
        }
        Game->UpdateQuestInfoBar(BarID, BarToFill, -1.0f, -1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(BarID);
    PMasterData->CollectedSoulFromMother = true;
}
}