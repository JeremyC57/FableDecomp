class CLandscapeBackgroundPatch { public: void* vftable; };
// Leaf: ?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z
extern void __fastcall CLandscapeBackgroundPatch_dtor_00441cb0(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CLandscapeBackgroundPatch_dtor_00441cb0(self);
    if (flags & 1)
        operator delete(self);
    return self;
}