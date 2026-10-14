#include <cstdio>
#include <cstring>
#include <new>
static int g_strings = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
struct CResource { virtual void v00() {} unsigned char _body[0x18]; CResource() {} };
struct __declspec(novtable) CDefinitionBase : CResource { long m_defIndex; long m_reserved; unsigned char m_flags : 3; CDefinitionBase() : CResource(), m_defIndex(0), m_flags(0) { } };
struct CAIScratchpadDef : CDefinitionBase { unsigned char _pad0[0x3c]; CCharString Action; bool FaceHeroForAction; CCharString TargetName; unsigned char _pad1[0x1c]; CCharString TableName; CCharString SeatName; virtual void v00() {} CAIScratchpadDef(); };
CAIScratchpadDef::CAIScratchpadDef() : CDefinitionBase(), Action(), TargetName(), TableName(), SeatName() { }
int main()
{
    unsigned char raw[sizeof(CAIScratchpadDef)]; std::memset(raw, 0xFF, sizeof(raw));
    CAIScratchpadDef* d = new (raw) CAIScratchpadDef;
    if (d->m_defIndex != 0 || d->m_flags != 0 || d->Action.m_pData != 0 || d->TargetName.m_pData != 0 || d->TableName.m_pData != 0 || d->SeatName.m_pData != 0 || g_strings != 4) { std::printf("007abb30_TEST FAIL\n"); return 1; }
    std::printf("007abb30_TEST PASS\n");
    return 0;
}
