// NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail() -- when the dragon's thing was killed (by anyone: empty killer name) set the parent fight script's DragonState to 4 (dead; DoMission 0x00D26190 waits for 4)
// Grade: DIFFER(64v64) -- esi v eax only: retail reloads the parent into the dead this-register (`mov esi,[esi+14h] ; mov dword [esi+48h],4`), ours uses eax
// Chunk 894 (9 grades): prior draft (local CCharString / CQ_DragonBossFightScript models), direct store, reference local, const& setter, named bool, this shared-header rebase, pointer ParentClass, early-return form: all eax. c2prio shows no candidate for the parent. The landed sibling 0x00DE1F10 (long store, pointer ParentClass) uses eax in retail too, so the enum/long difference is not the lever. Landed siblings reuse esi only for read-modify-write (`inc [esi+N]`: 0x00D6AE30, 0x00DE1DB0, 0x00D43120). Next: a retail form that keeps `this` live into the store block (e.g. a second this-relative access after the store that c2 then folds), measured with c2prio --trace.
// Leaf: ?OnPredicateFail@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D259A0, 64 bytes, virtual __thiscall (slot 5 OnPredicateFail), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E), CScriptThing::MsgIsKilledBy (virtual slot 18), ~CCharString 0x0099EAE0.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}; Ego_r.pdb CDragon: ParentClass is a reference (+0x14), PMasterData +0x18, then three health thresholds.
// The value 4: engine/CQ_DragonBossFightScript.h leaves EDragonState without enumerators (only the value 4 is evidenced), so the store casts.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_DragonBossFightScript.h"
namespace NScript {
class CQ_DragonBossFightScript::CDragon : public CEntityScriptBase {
protected:
    CQ_DragonBossFightScript& ParentClass;     // +0x14
    CQ_SunnyvaleMasterData* PMasterData;       // +0x18
private:
    float MediumHealth, LowHealth, VeryLowHealth;
    virtual void Main();
    virtual void Init();
    virtual void OnPredicateFail();
public:
    virtual CScriptBase* GetParentScript() const;
};
void CQ_DragonBossFightScript::CDragon::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        ParentClass.DragonState = (EDragonState)4;
}
}
