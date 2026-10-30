// CContainerRewardHeroDef::Copy  0x004e249b

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

// member object at +0x28 with a Copy(const T&) method -> 0x4e24ba
struct Member28 {
    int pad[4];
    void Copy(const Member28& src);
};

class CContainerRewardHeroDef { public:
    char pad[0x28];
    Member28 m28;   // at +0x28
    void Copy(const CDefClassBase& src);
};

void CContainerRewardHeroDef::Copy(const CDefClassBase& src)
{
    ((CDefClassBase*)(this))->operator=((const CDefClassBase&)(src));
    const Member28* srcm = (const Member28*)((const char*)&src + 0x28);
    this->m28.Copy(*srcm);
}