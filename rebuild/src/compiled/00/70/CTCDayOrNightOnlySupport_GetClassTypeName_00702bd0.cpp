class CCharString { public:
    CCharString(const char* lit, long len);
    int dummy;
};

class CTCDayOrNightOnlySupport { public:
    CCharString GetClassTypeName();
};

CCharString CTCDayOrNightOnlySupport::GetClassTypeName()
{
    return CCharString("CTCDayOrNightOnlySupport", -1);
}