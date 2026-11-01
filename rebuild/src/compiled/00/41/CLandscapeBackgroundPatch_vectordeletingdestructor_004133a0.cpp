class CLandscapeBackgroundPatch;
// Leaf: `vector_deleting_destructor'
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx_pad*/, unsigned int flags)
{
    ((CCharString*)self)->~CCharString();
    if (flags & 1) {
        operator delete((void*)self);
    }
    return (void*)self;
}