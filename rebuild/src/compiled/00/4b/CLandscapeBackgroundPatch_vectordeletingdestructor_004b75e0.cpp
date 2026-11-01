class CLandscapeBackgroundPatch { public:
// Leaf: `vector_deleting_destructor'
    void* vftable;
};

class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void operator delete(void*);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    ((CCharString*)self)->~CCharString();
    if (flags & 1)
        operator delete(self);
    return self;
}