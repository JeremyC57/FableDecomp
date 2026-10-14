#include <cstdio>
static int g_dtors = 0;
struct CThing; struct CBaseObject;
struct CBaseIntelligentPointer { void* m_pItem; void* m_pInfo; CBaseObject* GetPItem() { return (CBaseObject*)m_pItem; } void SetPItem(CBaseObject* item) { m_pItem = item; } ~CBaseIntelligentPointer() { ++g_dtors; } };
struct CIntelligentPointer_CThing : CBaseIntelligentPointer { CIntelligentPointer_CThing(CThing* thing) { m_pItem = thing; m_pInfo = 0; } };
#pragma pack(push, 1)
struct CTCTargetingBase_Retail { unsigned char _pad_0x00[0x25]; CBaseIntelligentPointer PTargetedThing; void SetTargetedThing(CThing* thing) { CIntelligentPointer_CThing pointer(thing); PTargetedThing.SetPItem(pointer.GetPItem()); } };
#pragma pack(pop)

int main()
{
    int thing; CTCTargetingBase_Retail tc; tc.PTargetedThing.m_pItem = 0;
    tc.SetTargetedThing((CThing*)&thing);
    if (tc.PTargetedThing.m_pItem != &thing || g_dtors != 1) { std::printf("004ddab4_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&tc.PTargetedThing - (unsigned char*)&tc != 0x25) { std::printf("004ddab4_TEST FAIL\n"); return 1; }
    std::printf("004ddab4_TEST PASS\n");
    return 0;
}
