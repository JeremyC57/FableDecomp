// NScript::CV_ChapelOfEvilScript::CV_ChapelOfEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script-object constructor: inlined intermediate base (CScriptBase(), owner at +0x44, thing at +0x40), own vtable, a CCharString at +0x88
// Leaf: ??0CV_ChapelOfEvilScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E68F70, length 40 bytes.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (the script module's root constructor, see
// GOTCHAS -- the manifest's "CHeroMorphDef" label is propagated), CCharString::CCharString()
// @ 0x0099E4B0. The vtable 0x012E8080 has no RTTI locator (quest-script module), so the class is
// named CVtable_012E8080 as the family landings do.
// Shape: the base runs on `this`, the first argument is stored at +0x44 and the second at +0x40,
// BOTH before this class's vtable store -- init-list members of this class would leave the
// vtable store between them, so the two pointers belong to an inlined intermediate base
// constructor whose own vtable store is dead (abstract, `__declspec(novtable)`; the same
// script-object base that the landed CVtable_012f55b8 family stores its argument at +0x40 for).
// Then the vtable is stored and the name at +0x88 is default-constructed out of line.
// Naming wave 35: identity out-of-line copy of the ctor the Alloc 00e6e230 inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores script vtable 012e8080, ret 8 (two args); PDB ??0CV_ChapelOfEvilScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CV_ChapelOfEvilScript proven by its Main 00e693f0 entity registrations; was CVtable_012E8080::CVtable_012E8080 (wave3/labels_alloc.tsv); PDB ??0CV_ChapelOfEvilScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_012E8080@@QAE@PAX0@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
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
class CV_ChapelOfEvilScript : public CScriptObject {
public:       // vtable 0x012E8080
    unsigned char _pad[0x88 - 0x48];        // +0x48..+0x88
    CCharString   m_name;                   // +0x88
    virtual void v00();
    CV_ChapelOfEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CV_ChapelOfEvilScript();    // ??1CV_ChapelOfEvilScript@NScript@@UAE@XZ
};
}
using NScript::CV_ChapelOfEvilScript;

NScript::CV_ChapelOfEvilScript::CV_ChapelOfEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game)
    , m_name()
{
}