#pragma optimize("s",on)
extern void __fastcall Sub_Dtor_0099b510(void* p);
extern void __cdecl Op_Delete(void* p);

struct CLandscapeBackgroundPatch {
    int f0;
    int Parent;
    void* f8;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    Sub_Dtor_0099b510((char*)self + 4);
    if (flags & 1) {
        Op_Delete(self);
    }
    return self;
}