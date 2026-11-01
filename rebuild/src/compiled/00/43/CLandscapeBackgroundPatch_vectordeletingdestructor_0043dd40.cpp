class CLandscapeBackgroundPatch { public: void* vftable; };
// Leaf: `vector_deleting_destructor'
class CWideString { public:
    ~CWideString();   // 0x0099B510 (Ego_r.pdb ??1CWideString@@QAE@XZ)
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    ((CWideString*)self)->~CWideString();
    if (flags & 1)
        operator delete(self);
    return self;
}