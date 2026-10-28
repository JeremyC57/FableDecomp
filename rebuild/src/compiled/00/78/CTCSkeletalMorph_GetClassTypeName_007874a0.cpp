// CTCSkeletalMorph::GetClassTypeName @ 007874a0
// Retail: __fastcall, constructs CCharString(const char*, long) into the hidden
// return slot and returns it (ret 4 cleans the struct-return pointer).

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

extern const char kActionName_0123bffc[];

class CTCSkeletalMorph { public:
    CCharString GetClassTypeName();
};

CCharString CTCSkeletalMorph::GetClassTypeName()
{
    return CCharString(kActionName_0123bffc, -1);
}