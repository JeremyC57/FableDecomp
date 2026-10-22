struct CCharString {
    int d;
    CCharString(const char* lit, int len);
};

struct CTCTavernGameCoinGolf {
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameCoinGolf::GetClassTypeName() {
    return CCharString((const char*)0x123d538, -1);
}