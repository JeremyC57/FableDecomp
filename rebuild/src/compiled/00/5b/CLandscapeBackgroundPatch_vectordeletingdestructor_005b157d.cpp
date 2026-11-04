#pragma optimize("s",on)
// Leaf: ?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z
class CWideString { public:
    ~CWideString();   // 0x0099B510 (Ego_r.pdb ??1CWideString@@QAE@XZ)
};

class CLandscapeBackgroundPatch { public:
    int f0;
    int Parent;
    void* f8;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    ((CWideString*)((char*)self + 4))->~CWideString();
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}