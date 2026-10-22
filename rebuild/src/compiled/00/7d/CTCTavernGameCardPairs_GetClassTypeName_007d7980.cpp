// CTCTavernGameCardPairs::GetClassTypeName  @ 007d7980
// __fastcall, returns CCharString by value.

class CCharString {
public:
    CCharString(const char* s, int n);
    char* m_data;
};

class CTCTavernGameCardPairs {
public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameCardPairs::GetClassTypeName()
{
    return CCharString((const char*)0x123d56c, -1);
}