class CCharString { public:
    // real member ctor => this in ECX, (lit,-1) pushed, callee-cleaned.
    CCharString(const char* lit, int len);
    int dummy;
};

class CTCBuyableHouse { public:
    CCharString GetClassTypeName();
};

CCharString CTCBuyableHouse::GetClassTypeName()
{
    return CCharString((const char*)0x123a5ec, -1);
}