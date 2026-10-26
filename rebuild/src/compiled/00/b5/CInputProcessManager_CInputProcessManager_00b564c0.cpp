// Leaf: ??1CInputProcessManager@
class CVertexBufferWin32 {
public:
    void Uninitialise();   // 0x00A63200
};
extern "C" void __fastcall base_dtor_99a300(void* self);

class CInputProcessManager { public:
    void* vptr;
};

extern "C" void __fastcall DtorIPM_G(CInputProcessManager* self)
{
    self->vptr = (void*)0x129ba20;
    ((CVertexBufferWin32*)(self))->Uninitialise();
    base_dtor_99a300(self);
}