#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;

// helper called with this in ecx (thiscall modelled as fastcall + dummy edx-less: no stack args)
extern "C" void __fastcall helper_4dd641(CActiveFile* self);
// cdecl free-style function taking one pointer arg (cleaned by caller)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    void* payload;   // offset 0
};

void __fastcall OnReadFinished_15(CActiveFile* self)
{
    helper_4dd641(self);
    void* p = self->payload;
    if (p) {
        free(p);
    }
}