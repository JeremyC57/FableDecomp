struct Helper {
    void Do(const char* s, int n);
};
class CCharString { public: char* p; };
// Literal (NF413, 2026-11-25): HEAD cast the retail address (const char*)0x0123E6EC; it is now the literal spelled from retail's bytes
//   at 0x0123E6EC (narrow "CTCDExplosiveTrail", NUL-terminated).
extern "C" CCharString* __fastcall StatGetClassTypeName_9(Helper* result)
{
    result->Do("CTCDExplosiveTrail", -1);
    return (CCharString*)result;
}