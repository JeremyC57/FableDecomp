// NScript::CQ_MinionCampScript::ShouldSpawnPointSpawn(const CScriptThing& spawnPoint) -- true when spawning is on, the given spawn point is the chosen one and the thread is not terminating
// Leaf: ?ShouldSpawnPointSpawn@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9F070, 62 bytes, `__fastcall`, one stack argument (`ret 4`), bool in al.
//   FableWin symbol ?ShouldSpawnPointSpawn@CQ_MinionCampScript@NScript@@QAE_NABVCScriptThing@@@Z. Caller: the
//   CMC_SpawnPoint entity script's Main @ 0x00D9EE20 (call at 0x00D9EEDB, on its ParentClass).
// Label correction: the catalog "CScriptBase_CheckThreadTerminated" is descriptive.
// Callees: the spawn point's implementation (CScriptThing::PImp_Data, +4) virtual slot 78 (+0x138) IsSameThing,
//   CScriptBase::IsActiveThreadTerminating() @ 0x00CB7940.
// Layout (retail = Ego_r.pdb CQ_MinionCampScript - 0x14, STLport lists 4 bytes): ShouldSpawn +0x98,
//   SpawnPointToSpawn (CScriptThing) +0x9C (its PImp_Data +0xA0).
// Shape: speed TU (no pragma). Nested ifs on ShouldSpawn and `spawnPoint == SpawnPointToSpawn` (CScriptThing::operator==
//   inline: lhs.PImp_Data->IsSameThing(rhs.PImp_Data)), then the script translator's guard
//   `if (IsActiveThreadTerminating()) return false; return true;`, which VC7.1 folds to the byte-wide
//   `neg al ; sbb al,al ; inc al` (a `return !IsActiveThreadTerminating();` gives the full-width `sbb eax,eax ; inc eax`).
struct CScriptThingImp {                                // the implementation PImp_Data points at
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual bool IsSameThing(CScriptThingImp* other);   // slot 78 (+0x138)
};
class CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();
    CScriptThingImp* PImp_Data;                         // +0x04 CCountedPointer<CScriptThing>::Data
    void*            PImp_Info;                         // +0x08 CCountedPointer<CScriptThing>::Info
    bool operator==(const CScriptThing& other) const { return PImp_Data->IsSameThing(other.PImp_Data); }
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    void*         Game;                                 // +0x40
    void*         MasterScriptData;                     // +0x44
    unsigned char _state48[0x98 - 0x48];                // QuestStartScreenShown .. RightSpawnPoints
    bool          ShouldSpawn;                          // +0x98
    CScriptThing  SpawnPointToSpawn;                    // +0x9c
    bool ShouldSpawnPointSpawn(const CScriptThing& spawnPoint);
};
bool CQ_MinionCampScript::ShouldSpawnPointSpawn(const CScriptThing& spawnPoint)
{
    if (ShouldSpawn) {
        if (spawnPoint == SpawnPointToSpawn) {
            if (IsActiveThreadTerminating())
                return false;
            return true;
        }
    }
    return false;
}
}