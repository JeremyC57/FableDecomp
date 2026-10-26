class CInputProcessManager { public: void* vtbl; };
// Leaf: ??1CInputProcessManager@

extern "C" void __fastcall inner_dtor_00b48e10(CInputProcessManager* self);
extern "C" void __fastcall base_dtor_00f36c90(CInputProcessManager* self);

void __fastcall DtorIPM_F(CInputProcessManager* self)
{
    self->vtbl = (void*)0x12a20fc;
    inner_dtor_00b48e10(self);
    base_dtor_00f36c90(self);
}