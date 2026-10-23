struct CLandscapeBackgroundPatch { void* vftable; };
extern void __fastcall CLandscapeBackgroundPatch_dtor_00441ee0(CLandscapeBackgroundPatch* self);
extern void __cdecl operator_delete_stub(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_pad, unsigned int flags)
{
    (void)edx_pad;
    CLandscapeBackgroundPatch_dtor_00441ee0(self);
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}