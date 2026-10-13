// CShaderBankStateBlock::Set(const CBankStateBlock*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A61F40, oracle length 19 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; mov dl,[eax+8] ; mov [ecx+8],dl ; mov eax,[eax+4] ; mov [ecx+4],eax ; ret 4
// PDB CShaderBankStateBlock (12 bytes): +0x04 CStateBlock State. CStateBlock is a dword value
// (+0x04) plus a byte flag (+0x08). Copies the source block's state over this one's; the byte is
// copied before the dword, matching the retail store order.

struct CStateBlock {
    long          Value;      // +0x00 (state block id / render-state value)
    unsigned char Flag;       // +0x04 (valid / captured flag)
};

struct CBankStateBlock {
    void*       __vftable;    // +0x00
    CStateBlock State;        // +0x04
};

struct CShaderBankStateBlock {
    void*       __vftable;    // +0x00
    CStateBlock State;        // +0x04 (PDB)
    void Set(const CBankStateBlock* source);
};

void CShaderBankStateBlock::Set(const CBankStateBlock* source)
{
    this->State.Flag  = source->State.Flag;
    this->State.Value = source->State.Value;
}
