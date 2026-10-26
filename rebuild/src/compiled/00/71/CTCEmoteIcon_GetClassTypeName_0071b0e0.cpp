// CTCEmoteIcon::GetClassTypeName @ 0071b0e0
class CCharString { public:
    char* p;
    CCharString(const char* s, int n);
};

class CTCEmoteIcon {
public:
    CCharString GetClassTypeName();
};

CCharString CTCEmoteIcon::GetClassTypeName()
{
    return CCharString((const char*)0x123b314, -1);
}