#pragma optimize("s",on)
// Leaf: OnReadFinished
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall CActiveFile_PreRead_0065fe38(CActiveFile* self);

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_PreRead_0065fe38(self);
    void* p = self->buf;
    if (p)
        free(p);
}