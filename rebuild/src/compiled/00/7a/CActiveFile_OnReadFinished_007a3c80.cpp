// CActiveFile::OnReadFinished @ 007a3c80
// Leaf: OnReadFinished

struct CActiveFile {
    void* field0;
};

// base-class method (this-only), at 0x7a3910
void __fastcall BaseReadDone_007a3910(CActiveFile* self);

// free cdecl helper at 0xbfea14
void FreeHelper(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    BaseReadDone_007a3910(self);
    void* p = self->field0;
    if (p)
        FreeHelper(p);
}