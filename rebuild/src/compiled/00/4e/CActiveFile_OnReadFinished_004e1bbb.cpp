#pragma optimize("s",on)
// Leaf: OnReadFinished
#include <stdlib.h>

struct CActiveFile {
    void* buffer;
};

extern void __fastcall CActiveFile_Helper_004dfb5e(CActiveFile* self);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper_004dfb5e(self);
    void* p = self->buffer;
    if (p)
        free(p);
}