// Flags: 
// 00581353 std::vector<CDefPointer<const CTattooDef> >::~vector
// Leaf: ??1?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@V?$allocator@V?$CDefPointer@$$CBVCTattooDef@@@@@std@@@std@@QAE@XZ
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
class CTattooDef;
// Fidelity: the native tattoo vector destructor is a size-mode template owner.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template std::vector<CDefPointer<const CTattooDef> >::~vector();
