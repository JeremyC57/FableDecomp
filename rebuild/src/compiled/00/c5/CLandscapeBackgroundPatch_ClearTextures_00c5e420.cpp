// CLandscapeBackgroundPatch::ClearTextures @ 0x00c5e420
// Leaf: ClearTextures

struct Tex;

// IAT [0x143fe30] = KERNEL32.dll!CloseHandle (__imp__CloseHandle@4); single arg, callee cleans stack
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);

class CLandscapeBackgroundPatch { public:
    void* vtbl;   // +0
    Tex*  tex;    // +4
};

void __fastcall ClearTextures(CLandscapeBackgroundPatch* self)
{
    Tex* t = self->tex;
    self->vtbl = (void*)0x12b90e8;
    if (t) {
        CloseHandle(t);
        self->tex = 0;
    }
}