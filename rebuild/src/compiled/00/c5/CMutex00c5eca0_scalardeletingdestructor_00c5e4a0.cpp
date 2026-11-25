// CSalThread's scalar deleting destructor -- ~CSalThread inlined (vtable 0x012B90EC, Close: CloseHandle unless null), then operator delete on flag 1
// Leaf: ??_GCSalThread@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E4A0, 52 bytes, `ret 4`; slot 0 of vtable 0x012B90EC.
// Name (NF404, joins the T3 nf401 0x00C5ECA0 unit): the GameCODA 2.10 SDK member (CODAApi.lib Release/SalOutputKS.obj, section 78, defines
//   ??_GCSalThread@@UAEPAXI@Z as a pick-any (Any) COMDAT with this body, relocations masked); retail vtable 0x012B90EC is the CSalThread vtable
//   the CSalOutputKS constructor 0x00C5ED90 stores for its member. The unit's ~CSalOutputKS / CSalOutputKS rows emit this COMDAT from
//   engine/CSalOutputKS.h; it is re-rowed in the nf86 / nf88 pick-any form, so no strong definition here clashes with their copies. Was the
//   descriptive stand-in ??_GCMutex_00c5eca0@@UAEPAXI@Z.
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CSalOutputKS.h"
namespace {
// Fidelity: scaffolding, not retail: the qualified (non-virtual) destructor call names the header's inline destructor, so VC7.1
// emits it out of line as a pick-any COMDAT together with the vtable and the scalar deleting destructor it references. Internal to
// this TU; never called.
class CSalThreadReference {
public:
    static void Reference(CSalThread* object);
};
// Out of class, so it is an ordinary (not inline) function that VC7.1 compiles even though nothing calls it.
void CSalThreadReference::Reference(CSalThread* object)
{
    object->CSalThread::~CSalThread();
}
}
