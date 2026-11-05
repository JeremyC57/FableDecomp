// Leaf: ?CFontBankStateBlock_GetDefault@@YGXPAUCFontBankStateBlock@@@Z
struct CFontBankStateBlock {
    int   f0;
    int   f4;
    int   f8;
    char  fc;
};

void __stdcall CFontBankStateBlock_GetDefault(CFontBankStateBlock* block)
{
    block->f4 = 0;
    block->f8 = 0x190;
    block->fc = 0;
}