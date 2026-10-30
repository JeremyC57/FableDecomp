#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
class CHitLocationsDef { public:
    unsigned char _tail[0x34];
};
struct CSub {
    void SubCopy(const CSub* src);
};

void __fastcall CHitLocationsDef_Copy(CHitLocationsDef* self, int /*edx*/, const CHitLocationsDef* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    ((CSub*)((unsigned char*)self + 0x28))->SubCopy((const CSub*)((const unsigned char*)src + 0x28));
}