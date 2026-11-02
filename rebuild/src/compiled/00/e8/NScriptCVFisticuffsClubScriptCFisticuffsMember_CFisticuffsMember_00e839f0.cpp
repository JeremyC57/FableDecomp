// NScript::CV_FisticuffsClubScript::CFisticuffsMember::~CFisticuffsMember() -- implicit entity-script destructor: destroy BooName and CheerName, deregister ShoutTimer, then ~CEntityScriptBase
// Leaf: ??1CFisticuffsMember@CV_FisticuffsClubScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E839F0, 45 bytes, thiscall, tail jump to ~CEntityScriptBase 0x00F35B40; sole caller
//   the scalar deleting dtor 0x00E839D0 (slot 0 of the RTTI-less entity vtable 0x012EB0F8).
// Identity (proven): vtable 0x012EB0F8 is stored only by the landed ctor 0x00E83920 (??0CFisticuffsMember@CV_FisticuffsClubScript@
//   NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z), whose only caller is the factory 0x00E838A0 (`push 30h` = sizeof
//   Ego_r CFisticuffsMember, 48), bound as "FisticuffsMember" at 0x00E822D4; its slots are the landed OnPredicateFail 0x00E83990 /
//   Init 0x00E83A20 and GetParentScript 0x00E83980 (`mov eax,[ecx+14h]` = ParentClass). FableWin.pdb carries
//   ??1CFisticuffsMember@CV_FisticuffsClubScript@NScript@@UAE@XZ (0x03A0A280, after ??_G 0x03A0A0C0); no vtable store in the body =
//   compiler-generated dtor. The catalog label CQ_MinionCampScript::StartMinionSpawnTimer is propagated, and the landed ??_G
//   0x00E839D0 calls this row through the stand-in `CLandscapeBackgroundPatch_dtor_00e839f0` (NAME_FIXES.tsv).
// Layout: Ego_r.pdb CV_FisticuffsClubScript::CFisticuffsMember (0x30, entity scripts keep their PDB offsets; the landed ctor
//   writes the same): CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18, PerformingCheerOrBoo +0x1c, FC_Status +0x20,
//   ShoutTimer +0x24, CheerName +0x28, BooName +0x2c.
// Callees: CCharString::~CCharString 0x0099EAE0 (x2), CTimer::~CTimer inline (engine/CTimer.h: interface slot 88 DeregisterTimer,
//   +0x160, through pDSTGame 0x0143E8F8), ~CEntityScriptBase 0x00F35B40 (tail).
// TU: retail's own out-of-line ctor (0x00E83920, landed) is defined here, which emits the vtable, ??_G and this implicit dtor (the
//   landed CQ_MinionCampScript::CBriarRose dtor 0x00D9FED0 model).
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
class CCharStringData;
class CCharString {
public:
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FisticuffsClubScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CFisticuffsMember : public CEntityScriptBase {
    public:
        CV_FisticuffsClubScript* ParentClass;                           // +0x14
        CQ_SunnyvaleMasterData*  PMasterData;                           // +0x18
        bool                     PerformingCheerOrBoo;                  // +0x1c
        long                     FC_Status;                             // +0x20
        CTimer                   ShoutTimer;                            // +0x24
        CCharString              CheerName;                             // +0x28
        CCharString              BooName;                               // +0x2c
        CFisticuffsMember(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
        virtual void Main();
        virtual void Init();
        virtual void OnPredicateFail();
    public:
        virtual CScriptBase* GetParentScript() const;
    };
};
CV_FisticuffsClubScript::CFisticuffsMember::CFisticuffsMember(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& me)   // 0x00E83920
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}