// CTCBandit::GetClassTypeName @ 0076d960
// Retail: __fastcall, constructs CCharString(const char*, int) into the hidden
// return slot and returns it (ret 4 cleans the struct-return pointer).

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

extern const char kActionName[];

struct CTCBandit {
    CCharString GetClassTypeName();
};

CCharString CTCBandit::GetClassTypeName()
{
    return CCharString(kActionName, -1);
}