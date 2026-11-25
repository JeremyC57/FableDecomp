#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "rebuild_abi.h"
#include "engine/CIVCountedPointer.h"
namespace NGameText { class CTextEntry; }
template void CIVCountedPointer<const NGameText::CTextEntry>::Release();
template CIVCountedPointer<const NGameText::CTextEntry>::~CIVCountedPointer();
