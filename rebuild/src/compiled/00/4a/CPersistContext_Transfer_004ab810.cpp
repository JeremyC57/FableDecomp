// CPersistContext::Transfer<double>(const char* name, double& value) -- transfer a double with a default of 0.0
// Leaf: ??$Transfer@N@CPersistContext@@QAEXPBDAAN@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AB810, length 37 bytes (+ int3 padding).
// Callees: CPersistContext::Transfer<double>(const char*, double&, const double&) @ 0x004AB710 (CheckCRC, then the
// mode switch with `fld qword ptr` copies; its label "Transfer<signed char>" is propagated). The catalog label
// "Transfer<long>" is propagated too: the default is the double 0.0 from the pool @ 0x0122ED70.
// Shape: an 8-byte frame temporary for the literal default (`fld [0.0] ; fstp qword ptr [esp]`), its address pushed,
// then the value and name slots; `this` stays in ecx; `add esp,8 ; ret 8`.
// Name fix (agentNF54): the local CPersistContext stand-in replaced by the shared engine/CPersistContext.h. The callee 0x004AB710 is the
//   member template instance ??$Transfer@N@CPersistContext@@QAEXPBDAANABN@Z (Ego_r 0x004B25A0 + FableWin 0x01B5E170; was the
//   non-template ?Transfer@CPersistContext@@QAEXPBDAANABN@Z), and this row is the two-argument instance
//   ??$Transfer@N@CPersistContext@@QAEXPBDAAN@Z (FableWin 0x01B5E630; was the non-template ?Transfer@CPersistContext@@QAEXPBDAAN@Z):
//   the header's inline Transfer<T>(name, value) = `Transfer(name, value, GetDefaultVal<T>())` (FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS +
//   GETDEFAULTVAL_BODY; the traits default is 0.0).
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

template void CPersistContext::Transfer<double>(const char* name, double& value);
