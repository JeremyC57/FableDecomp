// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::`scalar deleting destructor'(unsigned int flags) -- the implicit
//   destructor inline (eh vector destructor iterator over skeleton_object[4] with ~CCharString, then ~CEntityScriptBase out of line), then
//   free this when bit 0 is set
// Leaf: ??_GCSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC3EA0, 50 bytes, `ret 4`; built without /EHsc like the landed rows of this script TU.
// Model (NF434): engine/CQ_OpeningGraveyardSecretPassageScript.h's nested CSecretPassageSkeleton (Ego_r.pdb layout at retail offsets, 0x34;
//   skeleton_object +0x20) on the shared engine/CEntityScriptBase.h, replacing the local models (a local CScriptBase whose in-class
//   virtual bodies emitted pick-any copies of CScriptBase::RegisterMain / Main / Init / OnPersist, and a local CEntityScriptBase).
//   The destructor is compiler-generated (FableWin.pdb), so the ??_G copy this TU emits with the vftable is the leaf.
// Emission: the PDB constructor (header opt-in FABLE_RETAIL_CQ_OPENINGGRAVEYARDSECRETPASSAGESCRIPT_CTORS) is defined inline here and
//   expanded by an anonymous-namespace helper. VC7.1 still emits an unreferenced out-of-line ??0CSecretPassageSkeleton copy (HEAD emitted
//   the same name as a strong local-model definition; retail has none: the skeleton Alloc expands it).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_OPENINGGRAVEYARDSECRETPASSAGESCRIPT_CTORS
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
inline NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::CSecretPassageSkeleton(
    CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(parent), PMasterData((CQ_SunnyvaleMasterData*)data)
{
}
namespace {
// Fidelity: scaffolding, not retail -- expands the constructor so the TU emits the vftable and the ??_G leaf (anonymous namespace,
//   hashed name; the 0x00BB1740 / 0x00BCA060 pattern).
CEntityScriptBase* EmitSkeleton(NScript::CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return new NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton(parent, data, me);
}
}
