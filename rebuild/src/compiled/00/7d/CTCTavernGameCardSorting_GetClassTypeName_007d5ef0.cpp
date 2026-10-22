// CTCTavernGameCardSorting::GetClassTypeName  @ 007d5ef0
// Returns a CCharString by value built from a fixed string-literal address.

class CCharString {
public:
    CCharString(const char* str, int len);
    void* m_data;
};

class CTCTavernGameCardSorting {
public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameCardSorting::GetClassTypeName()
{
    return CCharString((const char*)0x0123d550, -1);
}