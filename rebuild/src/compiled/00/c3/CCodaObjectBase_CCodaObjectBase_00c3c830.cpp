// CCodaObjectBase::~CCodaObjectBase() -- GameCODA 2.10 root class destructor: nothing to release, MSVC still re-installs the
//   root vptr (`mov dword ptr [ecx], 0x012B5868 ; ret`).
// Leaf: ??1CCodaObjectBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C830, 7 bytes.
// Name (owner decision 2026-11-03 D3): the root (vtable 0x012B5868) is GameCODA's CCodaObjectBase (pass 11 CPKObject; Codex pass 12
//   CodaEnvironment.h); the EH unwind funclets 0x012163D0 / 0x012163F0 call this destructor by that name. Earlier label: a generic
//   "CBase" / SetMemberConst_00c3c830 tiny-body shape.
// NF404 (joins the T3 nf401 0x00C5ECA0 unit): the destructor is an inline member in retail: ~CSalOutputKS 0x00C5ECA0 expands it in place,
//   and the original GameCODA 2.10 SDK CODAApi.lib emits ??1CCodaObjectBase@@UAE@XZ only as a 7-byte pick-any (Any) COMDAT (14 objects, e.g.
//   Object.obj, SalPlatform.obj), so 0x00C3C830 is that out-of-line copy. The unit's rows emit the same COMDAT from engine/CCodaObjectBase.h
//   (FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR); this row's former strong definition clashed with them (LNK4006 under /FORCE, NoDuplicates v
//   Any). Re-rowed in the pick-any form: the header's inline body, emitted through the reference below.
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CCodaObjectBase.h"
namespace {
// Fidelity: scaffolding, not retail: the qualified (non-virtual) destructor call names the header's inline destructor, so VC7.1 emits it
// out of line as a pick-any COMDAT (retail's 0x00C3C830, which retail's EH unwind funclets 0x012163D0 / 0x012163F0 call), together with
// the vtable and ??_G it references, as the HEAD row did. Internal to this TU; never called.
class CCodaObjectBaseDestructorReference {
public:
    static void Reference(CCodaObjectBase* object);
};
// Out of class, so it is an ordinary (not inline) function that VC7.1 compiles even though nothing calls it.
void CCodaObjectBaseDestructorReference::Reference(CCodaObjectBase* object)
{
    object->CCodaObjectBase::~CCodaObjectBase();
}
}
