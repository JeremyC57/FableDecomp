// CTCDiscovery::GetClassTypeName @ 0x00750a40
// Value-return of a CCharString built from a string-literal pointer and -1.

struct CCharString {
    int d;
    CCharString(const char* s, int n);   // ctor is engine 0x0099ebf0 (extern)
};

struct CTCDiscovery {
    CCharString GetClassTypeName();
};

CCharString CTCDiscovery::GetClassTypeName() {
    return CCharString((const char*)0x123ad54, -1);
}