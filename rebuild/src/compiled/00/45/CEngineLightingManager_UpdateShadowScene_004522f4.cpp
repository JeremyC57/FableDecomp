#pragma optimize("s",on)
// Leaf: UpdateShadowScene
// CEngineLightingManager::UpdateShadowScene @ 004522f4
struct CShadowArg;

struct CShadowSink {
    void Consume(CShadowArg* a); // thiscall
};

class CEngineLightingManager { public:
    char pad0[0x18];
    int  mode;      // +0x18
    char pad1c[0x24-0x1c];
    void* obj24;    // +0x24
    void* obj28;    // +0x28 (CShadowSink*)
};

struct CLocalHolder {
    int v; // [ebp-4]
    void Handle(void* arg, void* obj); // thiscall (push arg then push obj)
};

extern "C" void __stdcall EnterShadowScope(void* tag);

// Literal (NF413, 2026-11-25): HEAD pushed the address of an extern int data symbol that reloc_bindings.tsv bound to 0x0122D70E, retail's one pooled ""
//   (00 at 0x0122D70E, NUL-preceded); it is now that literal.
void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, CShadowArg** ppArg)
{
    (void)edx;
    EnterShadowScope((void*)"");
    CLocalHolder local;
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Handle(self->obj24, ppArg);
        break;
    case 3:
        ((CShadowSink*)self->obj28)->Consume(*ppArg);
        break;
    }
}