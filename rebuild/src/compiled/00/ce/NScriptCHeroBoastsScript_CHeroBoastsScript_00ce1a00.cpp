// NScript::CHeroBoastsScript::CHeroBoastsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script-object constructor: inlined intermediate base (CScriptBase(), owner at +0x44, thing at +0x40), own vtable, a CCharString at +0x48
// Leaf: ??0CHeroBoastsScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE1A00, length 40 bytes.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (the script module's root constructor, see
// GOTCHAS -- the manifest's "CHeroMorphDef" label is propagated), CCharString::CCharString()
// @ 0x0099E4B0. The vtable 0x012C3688 has no RTTI locator (quest-script module), so the class is
// named CVtable_012C3688 as the family landings do.
// Shape: the base runs on `this`, the first argument is stored at +0x44 and the second at +0x40,
// BOTH before this class's vtable store -- init-list members of this class would leave the
// vtable store between them, so the two pointers belong to an inlined intermediate base
// constructor whose own vtable store is dead (abstract, `__declspec(novtable)`; the same
// script-object base that the landed CVtable_012f55b8 family stores its argument at +0x40 for).
// Then the vtable is stored and the name at +0x48 is default-constructed out of line.
// Naming wave 35: identity CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ce6c40 at 00cd53df with name "HeroBoasts"; Alloc news + stores script vtable 012c3688; PDB ?Alloc@CHeroBoastsScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 8 = PDB arity; PDB ??0CHeroBoastsScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012C3688::CVtable_012C3688 (wave3/labels_alloc.tsv); PDB ??0CHeroBoastsScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_012C3688@@QAE@PAX0@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
#include "fable_string.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {   // abstract: no vtable store of its own
    void* m_pThing;                         // +0x40
    void* m_pOwner;                         // +0x44
    CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};

namespace NScript {
class CHeroBoastsScript : public CScriptObject {
public:       // vtable 0x012C3688
    CCharString   m_name;                   // +0x48
    virtual void v00();
    CHeroBoastsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CHeroBoastsScript();    // ??1CHeroBoastsScript@NScript@@UAE@XZ
};
}
using NScript::CHeroBoastsScript;

NScript::CHeroBoastsScript::CHeroBoastsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game)
    , m_name()
{
}