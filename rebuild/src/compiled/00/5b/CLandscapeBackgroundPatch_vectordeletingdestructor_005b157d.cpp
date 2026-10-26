#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall Sub_Dtor_0099b510(void* p);

class CLandscapeBackgroundPatch { public:
    int f0;
    int Parent;
    void* f8;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    Sub_Dtor_0099b510((char*)self + 4);
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}