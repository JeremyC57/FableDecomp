class CCharString { public:
    int d;
    CCharString(const char* lit, long len);
};

class CTCTavernGameCoinGolf { public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameCoinGolf::GetClassTypeName() {
    return CCharString("CTCTavernGameCoinGolf", -1);
}