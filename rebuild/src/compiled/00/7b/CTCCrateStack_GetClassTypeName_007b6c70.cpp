// CTCCrateStack::GetClassTypeName @ 007b6c70
// __fastcall returning CCharString by value from a string literal at 0x123c8d0.

struct CCharString {
    char* p;
    // ctor(const char*, int len) at the call target
    CCharString(const char* s, int len);
};

struct CTCCrateStack {
    CCharString GetClassTypeName();
};

CCharString CTCCrateStack::GetClassTypeName()
{
    return CCharString((const char*)0x123c8d0, -1);
}