// CTCInventoryExperience::GetXString(long count) -- "" for zero, else the localised "x" prefix + the count as text
// Leaf: ?GetXString@CTCInventoryExperience@@AAE?AVCWideString@@J@Z
// Retail 0x0056EA4E, 76 bytes, ret 8 (hidden CWideString result + count; `this` is passed by every
// caller -- e.g. 0x00574A4E with NumItems at +0x218 -- but never read).
// Identity: Ego_r.pdb / FableWin.pdb ?GetXString@CTCInventoryExperience@@AAE?AVCWideString@@J@Z (private); the landed caller
// 0x00574885 ConstructCategoryStrength binds the same spelling. Callee 0x005CC772 is the public static __fastcall
// CTCInventoryStats::GetXString (Ego_r.pdb / FableWin.pdb ?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ, engine/CTCInventoryStats.h).
// Shape: count == 0 builds the result straight from the empty literal L"" (0x0122D70C) through
// CWideString(const wchar_t*) 0x0099B6B0. Otherwise `CTCInventoryStats::GetXString() + GFIntToWideString(count)`: VC
// evaluates the right operand first (the __fastcall GFIntToWideString 0x00405F00 -- hidden result in
// ecx = the one-word frame slot, count in edx -- whose address is pushed as operator+'s stack
// argument), then CTCInventoryStats::GetXString 0x005CC772 (localised TEXT_GUI_MENU_XSTRING, hidden result
// homed in the dead `count` slot [ebp+0Ch]), then the __fastcall operator+(const CWideString&,
// const CWideString&) 0x0099BE70 into the hidden result; both temporaries die through
// ~CWideString 0x0099B510 (last built first).
// Frame-pointer body that ends in `leave`: `#pragma optimize("y", off)` + `("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_PRIVATE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryExperience.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                            // 0x00405F00

CWideString CTCInventoryExperience::GetXString(long count)
{
    if (count == 0)
        return CWideString(L"");
    return CTCInventoryStats::GetXString() + GFIntToWideString(count);
}
