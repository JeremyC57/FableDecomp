// CTCCrateStack::GetClassTypeName @ 007b6c70
// __fastcall returning CCharString by value from a string literal at 0x123c8d0.

class CCharString { public:
    char* p;
    // ctor(const char*, int len) at the call target
    CCharString(const char* s, long len);
};

class CTCCrateStack { public:
    CCharString GetClassTypeName();
};

CCharString CTCCrateStack::GetClassTypeName()
{
    return CCharString((const char*)0x123c8d0, -1);
}