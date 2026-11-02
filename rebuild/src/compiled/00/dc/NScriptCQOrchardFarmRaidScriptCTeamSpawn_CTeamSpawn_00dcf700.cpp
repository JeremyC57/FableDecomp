// NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::~CTeamSpawn() -- implicit entity-script destructor: destroy TeamMemberDefName and TeamMemberName, release OtherSpawnPoint (inline ~CScriptThing), then ~CEntityScriptBase
// Leaf: ??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCF700, 94 bytes, thiscall, tail jump to ~CEntityScriptBase 0x00F35B40; sole caller
//   the scalar deleting dtor 0x00DCF6E0 (slot 0 of the RTTI-less entity vtable 0x012DB2D4).
// Identity (proven): vtable 0x012DB2D4 is stored only by the landed ctor 0x00DCD160 (Ego_r.pdb / FableWin.pdb
//   ??0CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z, `ret 0Ch`), whose only caller
//   is the factory 0x00DCD0E0 (`push 40h` = Ego_r sizeof CTeamSpawn 64; bound ?Alloc@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@..)
//   referenced from CQ_OrchardFarmRaidScript::Main 0x00DCC770 next to the binding name "GuardTeamSpawn" (0x012DB2C4); the vtable's
//   slots 1-3 are the landed Main 0x00DCD350 / Init 0x00DCD1D0 / GetParentScript 0x00DCD1C0 (`mov eax,[ecx+14h]`). Ego_r.pdb
//   0x00E04400 and FableWin.pdb 0x0385D470 both spell ??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ. No 0x012DB2D4 store
//   in the body = compiler-generated dtor. The catalog label CActionPerformExpression::~CActionPerformExpression is wrong
//   (NTCScriptedControl::CActionPerformExpression is an engine action class; this body chains to ~CEntityScriptBase).
// Layout: Ego_r.pdb CTeamSpawn (0x40; entity scripts keep their PDB offsets, the landed ctor writes the same): CEntityScriptBase 0x14,
//   ParentClass +0x14, PMasterData +0x18, OtherSpawnPoint (CScriptThing) +0x1c, TeamID +0x28, TeamMemberLimit +0x2c,
//   TeamRespawnTime +0x30, TeamMemberName +0x34, TeamMemberDefName +0x38, BanditsLeftID +0x3c.
// Callees: CCharString::~CCharString 0x0099EAE0 (x2), the inline ~CScriptThing (vtable 0x01238C8C store, inline
//   ~CCountedPointer<CScriptThing> release through operator delete 0x00BFE9BC, then ~CBaseClass 0x0099A2E0), ~CEntityScriptBase
//   0x00F35B40 (tail).
// TU: retail's own out-of-line ctor (0x00DCD160, landed) is defined here, which emits the vtable, ??_G and this implicit dtor
//   (the landed CV_FisticuffsClubScript::CFisticuffsMember dtor 0x00E839F0 model); ~CScriptThing inline as in that row.
//   FABLE_RETAIL_CLEVELSCRIPTBASE: retail's ~CLevelScriptBase<T> is compiler-generated (chunk 713 branch).
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing() {}
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
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTeamSpawn : public CEntityScriptBase {       // vtable 0x012DB2D4, 0x40 bytes
    public:
        CQ_OrchardFarmRaidScript* ParentClass;                          // +0x14
        CQ_SunnyvaleMasterData*   PMasterData;                          // +0x18
        CScriptThing              OtherSpawnPoint;                      // +0x1c
        long                      TeamID;                               // +0x28
        long                      TeamMemberLimit;                      // +0x2c
        long                      TeamRespawnTime;                      // +0x30
        CCharString               TeamMemberName;                       // +0x34
        CCharString               TeamMemberDefName;                    // +0x38
        long                      BanditsLeftID;                        // +0x3c
        CTeamSpawn(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00DCD160
    private:
        virtual void Main();                                            // slot 1 0x00DCD350
        virtual void Init();                                            // slot 2 0x00DCD1D0
    public:
        virtual CScriptBase* GetParentScript() const;                   // slot 3 0x00DCD1C0
    };
};
CQ_OrchardFarmRaidScript::CTeamSpawn::CTeamSpawn(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}