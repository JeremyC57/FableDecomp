#pragma optimize("s",on)

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall CActiveFile_PreRead_0065e4d1(CActiveFile* self);
extern "C" void __cdecl CActiveFile_Free(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_PreRead_0065e4d1(self);
    void* p = self->buf;
    if (p)
        CActiveFile_Free(p);
}