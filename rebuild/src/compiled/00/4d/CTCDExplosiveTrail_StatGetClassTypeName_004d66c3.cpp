struct Helper {
    void Do(const char* s, int n);
};
class CCharString { public: char* p; };
extern "C" CCharString* __fastcall StatGetClassTypeName_9(Helper* result)
{
    result->Do((const char*)0x123e6ec, -1);
    return (CCharString*)result;
}