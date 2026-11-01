#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};

class CLandscapeBackgroundPatch { public:
    int f0;
    int Parent;
    void* f8;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    ((CCharString*)((char*)self + 4))->~CCharString();
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}