// CSalKsPin::~CSalKsPin() -- empty body; the base ~CSalFile is expanded (vtable 0x012B90D4 store, Close)
// Leaf: ??1CSalKsPin@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5EC50, 33 bytes.
// Name (NF404, joins the T3 nf401 0x00C5ECA0 unit): the GameCODA 2.10 SDK member (CODAApi.lib Release/SalOutputKS.obj, section 104, defines
//   ??1CSalKsPin@@UAE@XZ as a pick-any (Any) COMDAT with this body, relocations masked); retail vtable 0x012B9148 is the CSalKsPin vtable the
//   CSalOutputKS constructor 0x00C5ED90 stores for its member. The unit's ~CSalOutputKS / CSalOutputKS rows emit this COMDAT from
//   engine/CSalOutputKS.h; it is re-rowed in the nf86 / nf88 pick-any form, so no strong definition here clashes with their copies. Was the
//   descriptive stand-in ??1CVtable_012b90d4_00c5ec50@@QAE@XZ.
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CSalOutputKS.h"
namespace {
// Fidelity: scaffolding, not retail: the qualified (non-virtual) destructor call names the header's inline destructor, so VC7.1
// emits it out of line as a pick-any COMDAT together with the vtable and the scalar deleting destructor it references. Internal to
// this TU; never called.
class CSalKsPinReference {
public:
    static void Reference(CSalKsPin* object);
};
// Out of class, so it is an ordinary (not inline) function that VC7.1 compiles even though nothing calls it.
void CSalKsPinReference::Reference(CSalKsPin* object)
{
    object->CSalKsPin::~CSalKsPin();
}
}
