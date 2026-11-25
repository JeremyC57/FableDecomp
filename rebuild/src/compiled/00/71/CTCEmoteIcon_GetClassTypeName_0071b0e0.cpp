// CTCEmoteIcon::GetClassTypeName @ 0071b0e0
class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

class CTCEmoteIcon {
public:
    CCharString GetClassTypeName();
};

CCharString CTCEmoteIcon::GetClassTypeName()
{
    return CCharString("CTCEmoteIcon", -1);
}