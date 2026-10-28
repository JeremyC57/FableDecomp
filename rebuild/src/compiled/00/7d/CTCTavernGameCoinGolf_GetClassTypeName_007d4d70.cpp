class CCharString { public:
    int d;
    CCharString(const char* lit, long len);
};

class CTCTavernGameCoinGolf { public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameCoinGolf::GetClassTypeName() {
    return CCharString((const char*)0x123d538, -1);
}