// 009c7a60 NGameText::ConstructStringInternal
// Flags: /O2 /Oy /W3 /GS
// Leaf: ?ConstructStringInternal@NGameText@@YA_NAAVCWideString@@JZZ
// Shape: index is initialised before the va_list and the first-argument test is the while condition itself (no enclosing if); va_arg
//   is read inside the body before ++index, while key is still alive. The placeholder is L"%" (0x0129B470) + the index through the inline
//   GFIntToWideString (default constructor, Sprintf(L"%d") with the format at 0x0122DE44, copy into the result, destructor).
// Naming (NF146, 2026-11-20, q4285): ported off the row-local NGameText::CWideString / CWideStringData model (and its four same-TU
//   noinline member copies) onto fable_string.h (FABLE_RETAIL_CWIDESTRING); the header-only form matches, so no same-TU copy is needed.
//   Own leaf to the PDB ?ConstructStringInternal@NGameText@@YA_NAAVCWideString@@JZZ (Ego_r.pdb 0x00B157D0 + FableWin.pdb 0x02F6E140:
//   CWideString&, long, ...), the spelling the landed 0x009C7BA0 binds; was ?ConstructStringInternal@NGameText@@YA_NAAUCWideString@1@HZZ.
//   Callees now bind the plain CWideString spellings other landed rows bind (0x0099AED0, 0x0099BA70 Sprintf, 0x0099B720, 0x0099B510,
//   0x0099BFF0, 0x0099AF20 Contains, 0x0099B6B0, 0x0099C1E0 Replace); the made-up FormatIndex helper is the PDB inline GFIntToWideString
//   (engine/GFIntToWideString.h); the separator formatter 0x00A0DAC0 takes its landed self spelling FormatNumberWithSeparators(long,
//   unsigned short*) (descriptive, no PDB name), was the NGameText::FormatNumber_AddThousandsSeparators stand-in; the two format strings
//   are the retail literals L"%d" / L"%" (read from Fable.exe), were the FormatNumber / Prefix data stand-ins.
#define FABLE_RETAIL_CWIDESTRING
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/GFIntToWideString.h"
#include <stdarg.h>

void __fastcall FormatNumberWithSeparators(long value, unsigned short* text);   // 0x00A0DAC0

namespace NGameText {

bool __cdecl ConstructStringInternal(CWideString& output, long first, ...)
{
    bool result = true;
    long index = 1;
    va_list args;
    va_start(args, first);
    long number = first;
    while (number != -1) {
        CWideString key = L"%" + GFIntToWideString(index);
        if (!output.Contains(key)) {
            result = false;
            break;
        }
        unsigned short buffer[32];
        FormatNumberWithSeparators(number, buffer);
        output.Replace(key, CWideString(buffer));
        number = va_arg(args, long);
        ++index;
    }
    va_end(args);
    return result;
}

}
