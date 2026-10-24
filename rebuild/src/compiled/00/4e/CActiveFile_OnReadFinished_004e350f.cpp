#pragma optimize("s",on)
// Leaf: OnReadFinished
#include <stdlib.h>

struct CActiveFile;
extern "C" void __fastcall CActiveFile_helper_004e1bd2(CActiveFile* self);

struct CActiveFile {
    void* ptr;
};

extern "C" void __fastcall OnReadFinished_2(CActiveFile* self)
{
    CActiveFile_helper_004e1bd2(self);
    void* p = *(void**)self;
    if (p)
        free(p);
}