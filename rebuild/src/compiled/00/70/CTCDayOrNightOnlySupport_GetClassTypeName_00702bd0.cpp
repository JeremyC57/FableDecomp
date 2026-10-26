class CCharString { public:
    CCharString(const char* lit, int len);
    int dummy;
};

class CTCDayOrNightOnlySupport { public:
    CCharString GetClassTypeName();
};

CCharString CTCDayOrNightOnlySupport::GetClassTypeName()
{
    return CCharString((const char*)0x123c158, -1);
}