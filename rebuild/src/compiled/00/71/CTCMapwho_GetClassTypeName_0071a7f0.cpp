// CTCMapwho::GetClassTypeName @ 0071a7f0
struct CCharString {
    char* storage_;
    CCharString(const char* text, long length);
};

struct CTCMapwho {
    int dummy;
    CCharString GetClassTypeName();
};

CCharString CTCMapwho::GetClassTypeName()
{
    return CCharString("ACTION_DO_CREATURE_ACTION", -1);
}