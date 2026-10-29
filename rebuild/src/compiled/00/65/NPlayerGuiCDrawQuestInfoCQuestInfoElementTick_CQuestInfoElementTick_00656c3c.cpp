#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick::CQuestInfoElementTick(long, bool, float) -- a tick-box quest-info element: base element(scale), the graphic, an empty wide-string text and the ticked flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656C3C, length 55 bytes.
// Callees: the CQuestInfoElement base constructor @ 0x00656795 (one float argument, pushed as a copy:
// `fld [esp+0Ch] ; push ecx ; fstp [esp]`; it stores the base vtable 0x0125A2B4, zeroes ID/ScriptID and the handle
// vector and stores Scale), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0 with the empty literal @ 0x0122D70C.
// Vtable 0x0125A518 = NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick (rtti-vtables.tsv).
// Layout (PDB Graphic +0x20, Text +0x24, Ticked +0x28; retail 4 lower after the base's 12-byte handle vector):
// Graphic +0x1c, Text +0x20, Ticked +0x24 (the landed UpdateTick @ 0x00644EC4 reads Ticked at +0x24).
// Shape: a size-optimised TU like the landed CQuestInfoElementCounterList constructor (0x00656C0A): the base call,
// the derived vtable and the Graphic store, the text constructor, then the flag byte; `ret 0Ch`.
#include <stddef.h>

class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);              // 0x0099B6B0
};

namespace NPlayerGui {

struct CQuestInfoElement {
    virtual void v00();
    unsigned char _body[0x1c - 4];                 // ID, ScriptID, Scale, PrimitiveHandles
    CQuestInfoElement(float scale);                // 0x00656795
};

struct CQuestInfoElementTick : CQuestInfoElement { // vtable 0x0125A518
    virtual void v00();
    long        Graphic;                           // +0x1c
    CWideString Text;                              // +0x20
    bool        Ticked;                            // +0x24
    CQuestInfoElementTick(long graphic, bool ticked, float scale);
};

CQuestInfoElementTick::CQuestInfoElementTick(long graphic, bool ticked, float scale)
    : CQuestInfoElement(scale), Graphic(graphic), Text(L""), Ticked(ticked)
{
}

}