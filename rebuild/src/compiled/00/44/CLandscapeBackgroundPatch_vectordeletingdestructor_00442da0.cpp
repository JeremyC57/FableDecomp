struct CLandscapeBackgroundPatch { void* vftable; };
// Leaf: `vector_deleting_destructor'
extern void __fastcall CLandscapeBackgroundPatch_dtor_00441cb0(CLandscapeBackgroundPatch* self);
extern void __cdecl op_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CLandscapeBackgroundPatch_dtor_00441cb0(self);
    if (flags & 1)
        op_delete(self);
    return self;
}