class CCharString { public:
    CCharString(const char* lit, int len);
    int dummy;
};

class CTCJackShield { public:
    CCharString GetClassTypeName();
};

CCharString CTCJackShield::GetClassTypeName()
{
    return CCharString((const char*)0x123a784, -1);
}