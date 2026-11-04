// NScript::CMarkTestScriptsScript::CShopKeeper::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CShopKeeper@CMarkTestScriptsScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFDA60, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00efcb16 "ShopKeeper" in 00efca00; Alloc 00efd9c0; vtable 012f6e44 slot 3; PDB ?GetParentScript@CShopKeeper@CMarkTestScriptsScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00efda60.
//   ?GetParentScript@CShopKeeper@CMarkTestScriptsScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CMarkTestScriptsScript.h (shared header: nested class CShopKeeper, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CMarkTestScriptsScript.h"
CScriptBase* NScript::CMarkTestScriptsScript::CShopKeeper::GetParentScript() const
{
    return &ParentClass;
}
