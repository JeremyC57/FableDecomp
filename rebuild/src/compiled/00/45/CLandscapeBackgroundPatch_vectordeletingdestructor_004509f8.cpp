#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
struct CInner { int a; };
class CLandscapeBackgroundPatch { public:
    int f0;
    int Parent;
    void* f8;
    CInner inner;
    unsigned char _pad_0x10[0xa8];
    void* tex;
};

class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    (void)edx;
    ((CCharString*)self)->~CCharString();
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}