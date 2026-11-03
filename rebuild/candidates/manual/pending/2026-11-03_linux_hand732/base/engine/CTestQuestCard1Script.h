#ifndef FABLE_ENGINE_CTESTQUESTCARD1SCRIPT_H
#define FABLE_ENGINE_CTESTQUESTCARD1SCRIPT_H
// Original Ego_r TPI 0x117c1: CLevelScriptBase<NScript::CEmptyData>, one public bool.
// Layout: native constructor 00eeeea0 stores Game +0x40 and MasterScriptData +0x44;
// Main writes Accepted4SecondBoast +0x48. Alloc 00eef540 allocates 0x4c bytes.
// Native owner vtable 012f5a78 slot 2 and both PDB name sets establish Main.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CEmptyData;
class CTestQuestCard1Script : public CLevelScriptBase<CEmptyData> {
public:
    CTestQuestCard1Script(CScriptDataBase*, CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    bool Accepted4SecondBoast;
    virtual void Main();
    void WaitForBoasts();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CTestQuestCard1Script) == 0x4c);
#endif
