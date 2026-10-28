// CTCActionUseBuyOrSteal::GetClassTypeName @ 0x0072a710
// __fastcall, returns CCharString by value (hidden struct-return ptr on stack).
// Retail ignores `this`, constructs a CCharString("literal", -1) in the return slot.

class CCharString {
public:
    CCharString(const char* s, long n);
    void* p;
};

class CTCActionUseBuyOrSteal {
public:
    CCharString GetClassTypeName();
};

CCharString CTCActionUseBuyOrSteal::GetClassTypeName()
{
    return CCharString((const char*)0x123a154, -1);
}