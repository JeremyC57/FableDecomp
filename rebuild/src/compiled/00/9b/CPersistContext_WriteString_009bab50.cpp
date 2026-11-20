// CPersistContext::WriteString(const CCharString& string) -- text save: append the string to the context's output string
// Leaf: ?WriteString@CPersistContext@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BAB50, 8 bytes: `mov ecx,[ecx+20h] ; jmp 0x0099F0A0` (CCharString::operator+=(const CCharString&),
//   tail call on PStringToSaveTo +0x20).
// Naming (NF129, 2026-11-20, q4213): Ego_r.pdb 0x00B0F550 + FableWin.pdb 0x02FEA160 ?WriteString@CPersistContext@@QAEXABVCCharString@@@Z;
//   Ego_r donor order PersistError 0x00B0F490 -> SetAsFoundItem 0x00B0F540 -> WriteString 0x00B0F550 = retail 0x009BAA90 / 0x009BAB40 /
//   0x009BAB50 (hand621), engine/CPersistContext.h declares it, and the landed CParticleEmitter::DatabaseSerialise 0x00AEF1F0 binds it.
//   The previous landed spelling (`T::Run` forwarding to a `Sub::Run_0099f0a0` at +0x20) was a propagated placeholder over these bytes.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"

void CPersistContext::WriteString(const CCharString& string)
{
    *PStringToSaveTo += string;
}
