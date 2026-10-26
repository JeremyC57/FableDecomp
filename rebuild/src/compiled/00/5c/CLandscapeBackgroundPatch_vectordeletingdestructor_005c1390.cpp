#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
typedef unsigned int uint;

struct Sub;
class CLandscapeBackgroundPatch { public:
    char pad[8];
    Sub* sub;
};

extern void __fastcall Sub_dtor(void* subself);

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, uint flags)
{
    Sub_dtor((char*)self + 8);
    if (flags & 1)
        operator delete(self);
    return self;
}