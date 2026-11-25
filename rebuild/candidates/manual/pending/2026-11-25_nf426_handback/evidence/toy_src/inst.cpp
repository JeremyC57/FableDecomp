#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
class CClockDef;
template void CDefPointer<const CClockDef>::Release();
template CDefPointer<const CClockDef>::~CDefPointer();
