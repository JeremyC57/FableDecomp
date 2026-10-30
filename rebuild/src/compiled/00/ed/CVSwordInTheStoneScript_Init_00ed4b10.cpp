// CV_SwordInTheStoneScript::Init() -- reset the sword-in-the-stone quest state: attempt count and every progress flag cleared, the crowd sword timer set to 5 and the "current puller" handle reset to the empty one
// Leaf: ?Init@CV_SwordInTheStoneScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED4B10, length 122 bytes (padded to 128).
// Callees: g_pGameScriptInterface (0x0143E8F8) slot 89 (+0x164, SetTimer(long timer, long time)), the inline
// CScriptThing::operator= (CCountedPointer assignment as the landed 0x008AB980: the count block's destroy function at
// block +4 with the object at block +8 in ecx, operator delete @ 0x00BFE9BC).
// Layout (retail = PDB - 0x14) as the ctor 0x00ED4AA0: SwordRemovalAttemptCount +0x48, flags +0x4c..+0x53, +0x60/+0x61,
// CrowdMemberPullingCurrent +0x64 (pimpl Data +0x68, Info +0x6c), CrowdMemberPullingNull +0x70, CrowdSwordTimer +0x7c.
// Shape: the first three members (+0x48..+0x4d) are cleared through an inline Reset() of an unpadded sub-record: stores
// through the record's `this` keep the timer's Index load (`mov edx,[esi+7Ch]`) AFTER them, right after the
// g_pGameScriptInterface load as retail; three plain member stores let VC7.1 hoist the Index load above them.
#define GSI_V10(p) virtual void v##p##0(); virtual void v##p##1(); virtual void v##p##2(); virtual void v##p##3(); virtual void v##p##4(); virtual void v##p##5(); virtual void v##p##6(); virtual void v##p##7(); virtual void v##p##8(); virtual void v##p##9();
void __cdecl operator delete(void* p);             // 0x00BFE9BC
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { CGameScriptInterface* gsi = g_pGameScriptInterface; long index = TimerIndex; gsi->SetTimer(index, time); return *this; }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                    // +0x40
    void*                 Data;                    // +0x44
};
class CCPPointerInfo { public:
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
template <class T> class CCountedPointer { public:
    CScriptThingImp* Data;
    CCPPointerInfo*  Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CScriptThingImp* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
struct CV_SwordInTheStoneScript : CQuestScriptObject {
#pragma pack(push, 1)
    struct SAttempt {                              // the sword-removal attempt state (6 bytes, unpadded)
        long Count;                                // +0x48
        bool Cutscene;                             // +0x4c
        bool CrowdPulling;                         // +0x4d
        void Reset() { Count = 0; Cutscene = false; CrowdPulling = false; }
    } Attempt;
#pragma pack(pop)
    bool         CrowdSwordPullActive;             // +0x4e
    bool         HeroHasBeenToldSwordLegend;       // +0x4f
    bool         HeroCongratulated;                // +0x50
    bool         CrowdDisperse;                    // +0x51
    bool         HeroHasTakenSword;                // +0x52
    bool         SwordInTheStoneIntroComplete;     // +0x53
    long         HeroStrengthPhysique;             // +0x54
    long         HeroStrengthToughness;            // +0x58
    long         HeroStrengthHealth;               // +0x5c
    bool         StatsInitialised;                 // +0x60
    bool         AttackedByHero;                   // +0x61
    CScriptThing CrowdMemberPullingCurrent;        // +0x64
    CScriptThing CrowdMemberPullingNull;           // +0x70
    CTimer       CrowdSwordTimer;                  // +0x7c
    void Init();
};
void CV_SwordInTheStoneScript::Init()
{
    Attempt.Reset();
    CrowdSwordTimer = 5;
    HeroHasBeenToldSwordLegend = false;
    HeroCongratulated = false;
    CrowdDisperse = false;
    HeroHasTakenSword = false;
    SwordInTheStoneIntroComplete = false;
    StatsInitialised = false;
    AttackedByHero = false;
    CrowdSwordPullActive = false;
    CrowdMemberPullingCurrent = CrowdMemberPullingNull;
}