// CQ_RansomVictimScript::CheckVictimHit(CScriptThing victim) -- watch thread: note the ransom victim's health, then frame by frame until the victim is hit and has lost health; when that happens (and the thread is not terminating) clear the "victim took no damage" bonus flag
// Levers: ~CScriptThing __forceinline; CCountedPointer dtor zero order `Data = 0; Info = 0;`; for(;;) with the terminating return first (twin of 0x00DA2860, flag RansomVictimVictimNoDamage)
// Leaf: CheckVictimHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE81B0, length 319 bytes (+ int3 padding), `ret 0Ch` (the 12-byte
// CScriptThing is passed by value and destroyed here).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; _ftol 0x00BFEA70; CCharString(const char*, long) 0x0099EBF0
// / ~CCharString 0x0099EAE0; the thing's implementation object slot 21 MsgIsHitBy (+0x54) through the inline
// CScriptThing::MsgIsHitBy (vtable 0x01238C8C); operator delete 0x00BFE9BC and CBaseClass::~CBaseClass 0x0099A2E0 (the
// inline by-value destructor); through the quest's interface (+0x40): NewScriptFrame (slot 7), GetHealth (264).
// String: "" 0x0122D70E.
// Layout: quest base (GSI +0x40, PMasterData +0x44); CQ_SunnyvaleMasterData::RansomVictimVictimNoDamage +0x12A (Ego_r.pdb,
// no shift, as the landed CQ_BountyHuntScript::WatchForLeaderKilled 0x00D17090 reads the same block).
// Shape: the landed WatchForLeaderKilled by-value CScriptThing model; the starting health is truncated to a long
// (`_ftol`) and compared back as `GetHealth(victim) < health` (fild ; fcompp ; test ah,41h); the hit test is a named
// bool over one `&&` chain holding the "" conversion temporary (flag in ebx); with two exits the inline destructor
// is expanded at both.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing;
template <class T> class CCountedPointer { public:                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
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

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (PImp.Data) return PImp.Data->MsgIsHitBy(attacker); return false; }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0x12a];
    bool          RansomVictimVictimNoDamage;              // +0x12a
};

}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_RansomVictimScript : public CScriptBase {
public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void CheckVictimHit(CScriptThing victim);
};

void CQ_RansomVictimScript::CheckVictimHit(CScriptThing victim)
{
    long health = (long)GSI->GetHealth(victim);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool hit = victim.MsgIsHitBy("") && GSI->GetHealth(victim) < health;
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            PMasterData->RansomVictimVictimNoDamage = false;
            return;
        }
        GSI->NewScriptFrame();
    }
}
}