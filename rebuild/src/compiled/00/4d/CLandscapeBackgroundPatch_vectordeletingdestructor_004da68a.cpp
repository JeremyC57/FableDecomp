#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
struct Sub {
};

class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};

class CLandscapeBackgroundPatch { public:
    int pad0;
    Sub sub;
};

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    ((CCharString*)(&self->sub))->~CCharString();
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}