class CEngineLightingManager { public:
// Leaf: UpdateShadowScene
    char pad[0x18];
    int  mode;        // +0x18
    char pad2[0x24-0x1c];
    void* obj24;      // +0x24
    void* obj28;      // +0x28
};

class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};
struct CLocal {
    int v;
    void Do(void* a, int* b); // this=[ebp-4], two stack args
};

extern "C" void __stdcall LightScope(const char* tag);

#pragma optimize("y", off)
#pragma optimize("s", on)
void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* arg)
{
    LightScope((const char*)0x122d70e);
    CLocal local;
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Do(self->obj24, arg);
        break;
    case 3:
        ((CDataOutputStream*)self->obj28)->WriteSLONG(*arg);
        break;
    }
}