// source_cpp
// Leaf: UpdateShadowScene
#pragma optimize("s",on)

struct CLocalSink {
    int slot;
    void Emit(int obj24, int* param);
};
class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};

class CEngineLightingManager { public:
    char pad0[0x18];
    int mode;          // +0x18
    char pad1c[0x24-0x1c];
    CDataOutputStream* obj24; // +0x24
    CDataOutputStream* obj28; // +0x28
};

extern "C" void __stdcall LM_Prologue(int cookie);

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* /*edx*/, int* param)
{
    CLocalSink sink;
    LM_Prologue(0x122d70e);
    sink.slot = 0;
    switch (self->mode) {
    case 2:
        sink.Emit((int)self->obj24, param);
        break;
    case 3:
        self->obj28->WriteSLONG(*param);
        break;
    }
}