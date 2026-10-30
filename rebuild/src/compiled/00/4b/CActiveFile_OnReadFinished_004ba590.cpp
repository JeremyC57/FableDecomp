// CActiveFile::OnReadFinished @ 004ba590
// Leaf: OnReadFinished
struct CActiveFile;

// base-class read-complete handler (thiscall, this in ecx), base 0x4b8c00
void __fastcall BaseReadDone(CActiveFile* self);
// free cdecl helper, free 0xbfea14
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile {
    void* firstMember;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    BaseReadDone(self);
    void* p = self->firstMember;
    if (p != 0)
        free(p);
}