// CTCTavern::GetClassTypeName @ 0x007bace0
// __fastcall member returning CCharString by value (hidden retptr on stack, this in ecx, ret 4).

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

struct CTCTavern {
    char pad;
    CCharString GetClassTypeName();
};

CCharString CTCTavern::GetClassTypeName()
{
    return CCharString((const char*)0x0123caf0, -1);
}