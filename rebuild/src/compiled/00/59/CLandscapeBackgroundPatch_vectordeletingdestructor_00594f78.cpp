#pragma optimize("s", on)
// Leaf: `vector_deleting_destructor'
// Callee 0x00851720 is ??1CVirtualKeyboard@@QAE@XZ (Ego_r + FableWin; landed in chunk 707), respelled from the stand-in
// AutoTinyScalarDestructor_00851720 at that landing. This row itself is ??_GCVirtualKeyboard@@QAEPAXI@Z (FableWin; NAME_FIXES_QUEUE c707).
#define FABLE_RETAIL_CVIRTUALKEYBOARD
#include "engine/CVirtualKeyboard.h"
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    static_cast<CVirtualKeyboard*>(self)->~CVirtualKeyboard();
    if (flags & 1)
        operator delete(self);
    return self;
}