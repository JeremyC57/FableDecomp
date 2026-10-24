// CGuiVarTransferBool::TransferToGui @ 006bb1a0
// Leaf: TransferToGui

struct GuiTarget;

// helper @ 0x6bb140 : __fastcall(GuiTarget* ecx, int edx, bool val)
extern void __fastcall GuiSetBool_006bb140(GuiTarget* self, int edx, bool val);

struct CGuiVarTransferBool {
    GuiTarget* target;   // +0x0  -> helper ecx
    int        arg2;     // +0x4  -> helper edx
    int        pad8;     // +0x8
    bool       value;    // +0xc  bool arg pushed
    char       flag;     // +0xd  cleared to 0
};

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    GuiSetBool_006bb140(self->target, self->arg2, self->value);
    self->flag = 0;
}