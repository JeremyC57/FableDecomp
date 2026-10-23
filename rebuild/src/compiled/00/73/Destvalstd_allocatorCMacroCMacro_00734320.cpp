// _Dest_val<std::allocator<CMacro>,CMacro> : destroy chain forwarder.
extern "C" void __fastcall sub_A_00739030(void* self);
extern "C" void __fastcall sub_B_00739140(void* self);
extern "C" void __fastcall sub_C_00739250(void* self);
extern "C" void __fastcall sub_D(void* self);

void __fastcall Dest_val_allocator(void* self)
{
    sub_A_00739030(self);
    sub_B_00739140(self);
    sub_C_00739250(self);
    sub_D(self);
}