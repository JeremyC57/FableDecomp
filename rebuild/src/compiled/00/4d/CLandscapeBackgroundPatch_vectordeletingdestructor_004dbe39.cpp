#pragma optimize("s",on)
typedef unsigned int uint;

struct Sub;
struct CLandscapeBackgroundPatch {
    char pad[8];
    Sub* sub;
};

extern void __fastcall Sub_dtor_00a01c10(void* subself);
extern void __cdecl op_delete(void* p);

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, uint flags)
{
    Sub_dtor_00a01c10((char*)self + 8);
    if (flags & 1)
        op_delete(self);
    return self;
}