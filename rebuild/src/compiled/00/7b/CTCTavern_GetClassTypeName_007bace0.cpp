// CTCTavern::GetClassTypeName @ 0x007bace0
// __fastcall member returning CCharString by value (hidden retptr on stack, this in ecx, ret 4).

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

class CTCTavern { public:
    char pad;
    CCharString GetClassTypeName();
};

CCharString CTCTavern::GetClassTypeName()
{
    return CCharString((const char*)0x0123caf0, -1);
}