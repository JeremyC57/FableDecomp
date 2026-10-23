#pragma optimize("s",on)
struct CPersistContext {
    void TransferA_0045228f(void* field);
    void TransferB(void* field);
};
struct CCoopSpiritDef {
    unsigned char _tail[0x3c];
    void CCoopSpiritDef_Transfer(CPersistContext& ctx);
};

void CCoopSpiritDef::CCoopSpiritDef_Transfer(CPersistContext& ctx)
{
    ctx.TransferA_0045228f(this->_tail + 0x28);
    ctx.TransferA_0045228f(this->_tail + 0x2c);
    ctx.TransferA_0045228f(this->_tail + 0x30);
    ctx.TransferB(this->_tail + 0x38);
    ctx.TransferA_0045228f(this->_tail + 0x34);
}