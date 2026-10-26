class CCharString { public:
    int d;
    CCharString* ctor(const char* lit, int len);
};

CCharString* __fastcall StatGetClassTypeName(CCharString* thisret) {
    thisret->ctor((const char*)0x123a5cc, -1);
    return thisret;
}