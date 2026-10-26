#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
struct Sub {
};

extern "C" void __fastcall Sub_dtor_0099eae0(Sub* self);

class CLandscapeBackgroundPatch { public:
    int pad0;
    Sub sub;
};

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    Sub_dtor_0099eae0(&self->sub);
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}