// CTCFamilyMember::GetClassTypeName @ 0x00778be0
// __fastcall CCharString GetActionName(CTCFamilyMember* this)
// Retail: builds a CCharString from literal ptr 0x123b918 with len -1 and returns it.
// this (ecx) is unused; the return-buffer becomes ecx for the ctor call.

class CCharString {
public:
    // ctor(const char*, int) -> the call target 0x99ebf0
    CCharString(const char* s, int n);
    CCharString(const CCharString&);
    ~CCharString();
private:
    char* m_p;
};

class CTCFamilyMember {
public:
    CCharString GetClassTypeName();
};

CCharString CTCFamilyMember::GetClassTypeName()
{
    return CCharString((const char*)0x0123b918, -1);
}