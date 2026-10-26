// CTCWallMountEffects::GetClassTypeName @ 007dfab0
// __fastcall member returning CCharString by value (hidden retptr on stack, this in ecx, ret 4).
// Constructs a CCharString from the string literal at 0x123d734 with length -1.

class CCharString { public:
    char* p;
    CCharString(const char* s, int len);
};

class CTCWallMountEffects { public:
    CCharString GetClassTypeName();
};

CCharString CTCWallMountEffects::GetClassTypeName()
{
    return CCharString((const char*)0x123d734, -1);
}