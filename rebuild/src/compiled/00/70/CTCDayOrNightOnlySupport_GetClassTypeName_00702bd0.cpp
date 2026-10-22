struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct CTCDayOrNightOnlySupport {
    CCharString GetClassTypeName();
};

CCharString CTCDayOrNightOnlySupport::GetClassTypeName()
{
    return CCharString((const char*)0x123c158, -1);
}