struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct CTCJackShield {
    CCharString GetClassTypeName();
};

CCharString CTCJackShield::GetClassTypeName()
{
    return CCharString((const char*)0x123a784, -1);
}