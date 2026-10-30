// CQ_PrisonWardenGameScript::WardenKeyMiniGame() -- sparkles the prison cell key rack and keeps it targetable only while the hero is within 3 units, until the hero uses it; then marks the diary key taken, shows this race's diary code number (year one or two) and removes the sparkles
// Leaf: WardenKeyMiniGame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDE230, 1017 bytes, plain `ret`.
// Callees: ~CScriptThing 0x004AA840 (out of line on the key-used path), ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; the inline CScriptThing::GetPos (impl slot 6, g_ZeroVector
//   0x0143E8E0) and MsgIsUsedBy (impl slot 20); interface slots 7, 40 MsgIsGameInfoClickedPast, 70, 72, 100 CreateEffect,
//   108 RemoveThing, 115 DisplayGameInfo, 469 EntitySetTargetable.
// Layout: HeroHasSecretDiaryKey +0x49 (retail = PDB - 0x14); master data PrisonRaceNumber +0xe8 (PDB offset).
// Shape: for(;;) with the terminating return FIRST (rotated so the entry test and the loop-end test share one inline
//   destructor copy); the key-used block is inside the loop and ends in a return, all of it under
//   `#pragma inline_depth(0)` so its exits call ~CScriptThing out of line. Relocation targets checked (probe/rpair.py).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
extern C3DVector g_ZeroVector;                          // 0x0143E8E0

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};


template <class T> class CCountedPointer { public:
    T*  Data;
    CCPPointerInfo*  Info;
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

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return g_ZeroVector; return PImp.Data->GetPos(); }
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(user); }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0xe8];
    long          PrisonRaceNumber;                     // +0xe8
};

}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_PrisonWardenGameScript : public CScriptBase {
public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool                    SecretDiaryOpened;          // +0x48 (retail = PDB - 0x14)
    bool                    HeroHasSecretDiaryKey;      // +0x49
    void WardenKeyMiniGame();
};

void CQ_PrisonWardenGameScript::WardenKeyMiniGame()
{
    CScriptThing keyRack = GSI->GetThingWithScriptName("PrisonCellKeyRack");
    CScriptThing sparkles = GSI->CreateEffect("SPARKLES_SMALL", keyRack.GetPos(), "", 0.0f, false, false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsOver(GSI->GetHero(), keyRack, 3.0f)) {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetTargetable(keyRack, false);
        } else {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetTargetable(keyRack, true);
        }
        if (keyRack.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
            HeroHasSecretDiaryKey = true;
            GSI->EntitySetTargetable(keyRack, false);
            if (PMasterData->PrisonRaceNumber % 2 == 0) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->DisplayGameInfo("TEXT_QST_055_DIARY_CODE_NUMBER_YEAR_TWO");
                while (!GSI->MsgIsGameInfoClickedPast()) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                GSI->DisplayGameInfo("TEXT_QST_055_DIARY_CODE_NUMBER_YEAR_ONE");
                while (!GSI->MsgIsGameInfoClickedPast()) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            GSI->RemoveThing(sparkles, false, true);
            return;
#pragma inline_depth()
        }
        GSI->NewScriptFrame();
    }
}
}