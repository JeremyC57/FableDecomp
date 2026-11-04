struct CInner;
// Leaf: ?CConsoleVariable_scalar_deleting_destructor@@YIPAXPAUCConsoleVariable@@PAXI@Z
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};

struct CConsoleVariable {
    unsigned int vftable;
    char inner[0xb4];
    void* tex;
};

void* __fastcall CConsoleVariable_scalar_deleting_destructor(CConsoleVariable* variable, void* edx, unsigned int flags)
{
    (void)edx;
    variable->vftable = 0x122e5b0;
    ((CCharString*)((CInner*)(&variable->inner)))->~CCharString();
    if (flags & 1)
        operator delete(variable);
    return variable;
}