// CTCDiscovery::GetClassTypeName @ 0x00750a40
// Value-return of a CCharString built from a string-literal pointer and -1.

class CCharString { public:
    int d;
    CCharString(const char* s, long n);   // ctor is engine 0x0099ebf0 (extern)
};

class CTCDiscovery { public:
    CCharString GetClassTypeName();
};

CCharString CTCDiscovery::GetClassTypeName() {
    return CCharString((const char*)0x123ad54, -1);
}