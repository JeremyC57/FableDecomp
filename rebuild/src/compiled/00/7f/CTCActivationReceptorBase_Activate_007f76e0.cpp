class CTCActivationReceptorBase { public:
    char pad0[0x0c];
    unsigned char Activated;   // +0x0c
    char pad1[0x20 - 0x0d];
    unsigned char f20;   // +0x20
    char pad2[0x28 - 0x21];
    void* f28;           // +0x28
};

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

void __fastcall Activate(CTCActivationReceptorBase* receptor)
{
    receptor->Activated = 1;
    if (receptor->f20)
        receptor->f28 = ((void*)CWorld::GetFrame());
}