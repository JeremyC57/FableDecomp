struct CSubField { char data[4]; };
struct CTransferSink {
    void Xfer(CSubField* p);
};
class CSpecialAbilitiesDrainLifeDataDef { public:
    char pad0[0x28];
    CSubField sub;
    void Transfer(CTransferSink* sink);
};
void CSpecialAbilitiesDrainLifeDataDef::Transfer(CTransferSink* sink)
{
    sink->Xfer(&this->sub);
}