struct CTransferContext {
    int Xfer(void* p);
};

class CSpecialAbilitiesForcePushDataDef { public:
    unsigned char _tail[0x2c];
    void Transfer(CTransferContext* ctx);
};

void CSpecialAbilitiesForcePushDataDef::Transfer(CTransferContext* ctx)
{
    ctx->Xfer(this->_tail + 0x28);
}