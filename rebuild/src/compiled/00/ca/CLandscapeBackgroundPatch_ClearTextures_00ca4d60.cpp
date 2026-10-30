class CLandscapeBackgroundPatch { public:
    char pad[0xc0];
    void* tex;   // +0xc0
};

// call [0x14404b0]: the IAT slot WINMM.dll!timeKillEvent (__imp__timeKillEvent@4), one arg, callee-cleaned
extern "C" __declspec(dllimport) unsigned int __stdcall timeKillEvent(unsigned int);

void __fastcall ClearTextures(CLandscapeBackgroundPatch* self)
{
    void* t = self->tex;
    if (t) {
        timeKillEvent((unsigned int)t);
        self->tex = 0;
    }
}