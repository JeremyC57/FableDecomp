#pragma optimize("s",on)
class CDefString;
class CPersistContext { public:
    template <class T> void Transfer(T& value);
};
class CCoopSpiritDef { public:
    unsigned char _tail[0x3c];
    void CCoopSpiritDef_Transfer(CPersistContext& ctx);
};

void CCoopSpiritDef::CCoopSpiritDef_Transfer(CPersistContext& ctx)
{
    ctx.Transfer(*(CDefString*)(this->_tail + 0x28));
    ctx.Transfer(*(CDefString*)(this->_tail + 0x2c));
    ctx.Transfer(*(CDefString*)(this->_tail + 0x30));
    ctx.Transfer(*(float*)(this->_tail + 0x38));
    ctx.Transfer(*(CDefString*)(this->_tail + 0x34));
}