// NScript::CV_MurderTwistScript::CMurderTwistBuriedGem::Main() -- a script action body: register an "is thing alive" wait condition on the action's thing, pump the interface once, then keep pumping until the script thread is being terminated
// Leaf: ?Main@CMurderTwistBuriedGem@CV_MurderTwistScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EAD800, length 91 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the condition's thing copy,
// counted-pointer addref), WaitFor @ 0x00F35B10 (`this->GetScriptBase()` via vtable slot 3, then
// 0x00CB7920 with the condition; manifest auto-name "CWorldMap::AllocateMapThingUID" is wrong),
// CIsThingAlive::~CIsThingAlive @ 0x00E241B0, IsFinished @ 0x00F35B30 (GetScriptBase()->
// IsActiveThreadTerminating, named as in the landed CAT_BanditContactEnd::Main), game-interface vtable
// slot 7 (+0x1c, Pump) through the pointer at +4.
// CIsThingAlive (PDB, 16 bytes) = {vptr 0x012C32F0, CScriptThing Thing +4}; it is a stack temporary
// built inline (vtable store + out-of-line CScriptThing copy), passed by address and destroyed out
// of line once the wait is registered. The action's own CScriptThing is at +8. The vtable
// 0x012EF7C8 (script module, no RTTI; slot 1 = this Main) names the class; the manifest's
// "CCOE_AltarEvil::Main" label is propagated. The first Pump is unconditional; the loop after it is a
// rotated `while (!((const CEntityScriptBase*)this)->IsActiveThreadTerminating())`.
// Naming wave 35: identity reg 00ea8ad3 "MurderTwistBuriedGem" in 00ea8830; Alloc 00ead730; vtable 012ef7c8 slot 1; PDB ?Main@CMurderTwistBuriedGem@CV_MurderTwistScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was CVtable_012ef7c8::Main (wave3/labels_alloc.tsv); PDB ?Main@CMurderTwistBuriedGem@CV_MurderTwistScript@NScript@@EAEXXZ in FableWin.pdb.
//   Was `?Main@CVtable_012ef7c8@@QAEXXZ`; this source now defines the PDB member (class CVtable_012ef7c8 renamed CMurderTwistBuriedGem); same bytes.
struct CGameScriptInterface_Calls {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void Pump();                                    // slot 7 (+0x1c)
};

struct CScriptThing_Calls {
    void* __vftable;
    void* PImp_Data;
    void* PImp_Count;
    CScriptThing_Calls(const CScriptThing_Calls& other);    // 0x004ABE90
};

namespace NScript {
class CIsThingAlive { public:                                      // vtable 0x012C32F0
    virtual void v00();
    CScriptThing_Calls Thing;                               // +4
    CIsThingAlive(const CScriptThing_Calls& thing) : Thing(thing) { }
    virtual ~CIsThingAlive();                                       // 0x00E241B0
};
}
using NScript::CIsThingAlive;

class CEntityScriptBase { public:   // the PDB class the accessor belongs to
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};
namespace NScript {
class CV_MurderTwistScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CMurderTwistBuriedGem;
};
}
using NScript::CV_MurderTwistScript;
namespace NScript {
class CV_MurderTwistScript::CMurderTwistBuriedGem {
public:
    virtual void v00();
    CGameScriptInterface_Calls* m_pInterface;               // +0x04
    CScriptThing_Calls          m_thing;                    // +0x08
    void WaitFor(CIsThingAlive* condition);                 // 0x00F35B10
private:
    virtual void Main();                          // PDB ?Main@CMurderTwistBuriedGem@CV_MurderTwistScript@NScript@@EAEXXZ
};
}

void NScript::CV_MurderTwistScript::CMurderTwistBuriedGem::Main()
{
    {
        CIsThingAlive condition(m_thing);
        WaitFor(&condition);
    }
    m_pInterface->Pump();
    while (!((const CEntityScriptBase*)this)->IsActiveThreadTerminating())
        m_pInterface->Pump();
}