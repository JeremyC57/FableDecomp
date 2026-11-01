struct CInner;
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};

class CConsoleInputBase { public:
    unsigned int vftable;
    char inner[0xb4];
    void* tex;
};

void* __fastcall CConsoleInputBase_scalar_deleting_destructor(CConsoleInputBase* input, void* edx, unsigned int flags)
{
    (void)edx;
    input->vftable = 0x122e5b0;
    ((CCharString*)((CInner*)(&input->inner)))->~CCharString();
    if (flags & 1)
        operator delete(input);
    return input;
}