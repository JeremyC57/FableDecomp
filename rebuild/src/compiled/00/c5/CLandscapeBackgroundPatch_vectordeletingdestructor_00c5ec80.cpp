// CSalKsPin's scalar deleting destructor -- calls ~CSalKsPin 0x00C5EC50 out of line, then operator delete on flag 1
// Leaf: ??_GCSalKsPin@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5EC80, 30 bytes, `ret 4`; slot 0 of vtable 0x012B9148.
// Name (NF404, joins the T3 nf401 0x00C5ECA0 unit): slot 0 of vtable 0x012B9148, the CSalKsPin vtable the CSalOutputKS constructor 0x00C5ED90
//   stores for m_pin; the GameCODA 2.10 SDK (CODAApi.lib Release/SalOutputKS.obj, section 106) names it ??_GCSalKsPin@@UAEPAXI@Z (its build
//   expands ~CSalKsPin, 53 bytes; retail's copy calls ~CSalKsPin 0x00C5EC50). The unit's constructor row emits this same 30-byte COMDAT, so it
//   is re-rowed in the nf86 / nf88 pick-any form (no strong definition to clash). Was the descriptive stand-in
//   ?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z.
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALOUTPUTKS_KSPIN_DTOR_DECL_ONLY
#include "engine/CSalOutputKS.h"
namespace {
// Fidelity: scaffolding, not retail: a local CSalKsPin is constructed (the inline constructor stores vtable 0x012B9148, so VC7.1 emits
// it and its scalar deleting destructor) and destroyed through the out-of-line ~CSalKsPin, which this TU leaves declared only
// (FABLE_RETAIL_CSALOUTPUTKS_KSPIN_DTOR_DECL_ONLY). Internal to this TU; never called.
class CSalKsPinReference {
public:
    static void Reference(CSalKsPin* object);
};
// Out of class, so it is an ordinary (not inline) function that VC7.1 compiles even though nothing calls it.
void CSalKsPinReference::Reference(CSalKsPin* object)
{
    CSalKsPin pin; (void)object;
}
}
