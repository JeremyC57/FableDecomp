// CPersistContext::Transfer<CIntelligentPointer<CThing> >(const char*,CIntelligentPointer<CThing>&) -- transfer with an original default reference.
// Leaf: ??$Transfer@V?$CIntelligentPointer@VCThing@@@@@CPersistContext@@QAEXPBDAAV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: 0x006C9C60,106 bytes,thiscall; assigned237-byte span also contains two later functions.
// Flags: none
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "engine/CIntelligentPointer.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
template void CPersistContext::Transfer<CIntelligentPointer<CThing> >(const char*,CIntelligentPointer<CThing>&);