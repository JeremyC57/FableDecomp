// 005430f0 NUISystem::CIconText::InitIcons
// Flags:
// Retail-asm-helper: GFFloatToLongCeil.
#define FABLE_RETAIL_ICON_REVERSE_ITERATOR_PASS29
#define FABLE_RETAIL_ICONTEXT_FONT_INLINE_PASS29
#define FABLE_RETAIL_ICONTEXT_WIDTH_PAREN_PASS29
#define FABLE_RETAIL_ICONTEXT_CLEAR_OUTOFLINE_PASS29
#define FABLE_RETAIL_ICON_FLAGS_PASS29
// Leaf: ?InitIcons@CIconText@NUISystem@@UAEXXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CICONTEXT_VSLOTS
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
// Retail-asm: 0x00543195 `fistp dword ptr [ebp-14h]` after 0x00543192 `fadd dword ptr [ebp-24h]` (GFFloatToLongCeil) and 0x005432E8 `fistp dword ptr [ebp-1Ch]` -- the engine's inline-asm float-to-long helpers (shared engine/GFFloatToLongCeil.h / GFFloatToLongNear.h, Retail-asm-helper); no C++ form gives the bare dword fistp.
#include "fable_string.h"
void* __cdecl operator new[](unsigned int);
#include "engine/NUISystem_CIconText.h"
#ifdef _MSC_VER
// Inline-budget: retail 0048da89 and 0048daa2 select the size-built font accessors.
#pragma optimize("s",on)
#endif
#include "engine/CFontBank.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/GFFloatToLongCeil.h"
#include <math.h>
#include <wchar.h>
#define FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include "stlport/fable_algorithm.h"
namespace NUISystem {
void CIconText::InitIcons()
{
    float space_width = PFont->GetStringWidth(L"  ") - PFont->GetStringWidth(L" ");
    long num_spaces = 0;
    for (CEmbeddedIcon* icon_it = Icons.begin(); icon_it != Icons.end();) {
        const CEmbeddedIcon& icon = *icon_it;
        if (icon.Flags != ICON_INSIDE_TEXT) { ++icon_it; continue; }
        float total_icon_width = icon.Size.X;
        CEmbeddedIcon* follow_it = icon_it + 1;
        for (; follow_it != Icons.end(); ++follow_it) {
            CEmbeddedIcon& follow_icon = *icon_it;
            if (follow_icon.Flags != ICON_INSIDE_TEXT) continue;
            if (follow_icon.CharIndex != icon.CharIndex) break;
            total_icon_width += follow_icon.Size.X;
        }
        num_spaces += GFFloatToLongCeil(total_icon_width / space_width);
        icon_it = follow_it;
    }
    long source_length = Text.GetLength();
    long dest_length = source_length + num_spaces + 1;
    const wchar_t* source_text = Text.GetAsCharArray();
    wchar_t* dest_text = new wchar_t[dest_length];
    long source_index = 0;
    long dest_index = 0;
    for (CEmbeddedIcon* icon_it = Icons.begin(); icon_it != Icons.end();) {
        CEmbeddedIcon& icon = *icon_it;
        if (icon.Flags != ICON_INSIDE_TEXT) { ++icon_it; continue; }
        long num_characters;
        if (icon.CharIndex > source_length) num_characters = source_length - source_index;
        else num_characters = icon.CharIndex - source_index;
        if (num_characters) {
            wcsncpy(dest_text + dest_index, source_text + source_index, num_characters);
            source_index += num_characters;
            dest_index += num_characters;
        }
        dest_text[dest_index] = 0;
        float space_start = PFont->GetStringWidth(dest_text);
        float total_icon_width = icon.Size.X;
        float max_icon_height = icon.Size.Y;
        CEmbeddedIcon* follow_it = icon_it + 1;
        for (; follow_it != Icons.end(); ++follow_it) {
            CEmbeddedIcon& follow_icon = *icon_it;
            if (follow_icon.Flags == ICON_INSIDE_TEXT) {
                if (follow_icon.CharIndex != icon.CharIndex) break;
                total_icon_width += follow_icon.Size.X;
                if (follow_icon.Size.Y > max_icon_height) max_icon_height = follow_icon.Size.Y;
            }
        }
        num_spaces = GFFloatToLongCeil(total_icon_width / space_width);
        for (long space = 0; space < num_spaces; ++space) dest_text[dest_index++] = L' ';
        dest_text[dest_index] = 0;
        float space_end = PFont->GetStringWidth(dest_text);
        float icon_offset;
        if(space_start != 0) icon_offset=floor(space_end + space_start - total_icon_width)/2.0f;
        else icon_offset=0;
        for (follow_it = icon_it; follow_it != Icons.end(); ++follow_it) {
            CEmbeddedIcon& follow_icon = *follow_it;
            if (follow_icon.Flags != ICON_INSIDE_TEXT) continue;
            if (follow_icon.CharIndex != icon.CharIndex) break;
            follow_icon.Offset.X = icon_offset;
            follow_icon.Offset.Y = float(floor(PFont->GetFontMaxHeight() - max_icon_height)) / 2.0f;
            icon_offset += follow_icon.Size.X;
        }
        icon_it = follow_it;
    }
    long num_characters = source_length - source_index;
    if (num_characters) {
        wcsncpy(dest_text + dest_index, source_text + source_index, num_characters);
        source_index += num_characters;
        dest_index += num_characters;
    }
    dest_text[dest_index] = 0;
    float current_position = PositionOffset.X;
    for (std::vector<CEmbeddedIcon>::reverse_iterator icon_it = Icons.rbegin(); icon_it != Icons.rend(); ++icon_it) {
        CEmbeddedIcon& icon = *icon_it;
        if (icon.Flags != ICON_RIGHT_JUSTIFIED) continue;
        icon.Offset.X = current_position - icon.Size.X;
        icon.Offset.Y = (PFont->GetFontMaxHeight() - icon.Size.Y) / 2.0f;
        current_position = current_position - icon.Size.X - 2.0f;
    }
    std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    IconHandles.resize(Icons.size());
    DisplayText = CWideString(dest_text);
    C2DExtentsF extents = PFont->GetStringExtents(DisplayText, false);
    Size.X = extents.X;
    Size.Y = extents.Y;
    delete[] dest_text;
}
}