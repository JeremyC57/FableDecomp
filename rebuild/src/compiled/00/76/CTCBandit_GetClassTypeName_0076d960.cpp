// CTCBandit::GetClassTypeName @ 0076d960
// Retail: __fastcall, constructs CCharString(const char*, long) into the hidden
// return slot and returns it (ret 4 cleans the struct-return pointer).

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

extern const char kActionName[];

struct CTCBandit {
    CCharString GetClassTypeName();
};

CCharString CTCBandit::GetClassTypeName()
{
    return CCharString(kActionName, -1);
}