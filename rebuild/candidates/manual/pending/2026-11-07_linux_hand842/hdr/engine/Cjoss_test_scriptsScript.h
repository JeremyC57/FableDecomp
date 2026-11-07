#ifndef FABLE_ENGINE_CJOSS_TEST_SCRIPTSSCRIPT_H
#define FABLE_ENGINE_CJOSS_TEST_SCRIPTSSCRIPT_H
// Retail opt-in (chunk 842): NScript::Cjoss_test_scriptsScript, new header (no includer before chunk 842).
// Layout: no members of its own (Ego_r.pdb Cjoss_test_scriptsScript is 92 bytes = the Ego_r CLevelScriptBase, no data member;
// retail 0x48 = the retail CLevelScriptBase, the landed Alloc 0x00EF9EE0 `push 48h`). Base CLevelScriptBase<Cjoss_test_scriptsData>
// (the landed ctor 0x00EF9190; CLevelScriptBase<NScript::Cjoss_test_scriptsData> ctor 0x00CDD1F0 / scalar dtor 0x00CDD2B0).
// Ego_r.pdb name set: ?RegisterMain@ / ?Main@Cjoss_test_scriptsScript@NScript@@UAEXXZ (RegisterMain 0x00EF91C0 landed, Main
// 0x00EF9280: the "Main" CSpawnedFunc<Cjoss_test_scriptsScript> member pointer RegisterMain installs).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/Cjoss_test_scriptsScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class Cjoss_test_scriptsData;
class Cjoss_test_scriptsScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    Cjoss_test_scriptsScript(CScriptDataBase*,CGameScriptInterfaceBase&);   // 0x00EF9190
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);   // 0x00EF9EE0
    virtual void RegisterMain();                                         // 0x00EF91C0
    virtual void Main();                                                 // 0x00EF9280
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::Cjoss_test_scriptsScript) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CJOSS_TEST_SCRIPTSSCRIPT_H
