#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_set = 0; static void* g_setItem[4];
struct CEventBase_NEntityEvents { virtual void v00() { } long m_eventType; CEventBase_NEntityEvents() : m_eventType(0) { } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } void SetPItem(void* item) { m_pItem = item; g_setItem[g_set++] = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(void* item) : CBaseIntelligentPointer() { SetPItem(item); } };
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; CIVCountedPointer() : Object(0) { } CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); } } };
#pragma pack(push, 1)
namespace NEntityEvents { struct CEventVillagerInteraction : CEventBase_NEntityEvents { unsigned char m_flag; CIntelligentPointer m_villager; unsigned char _pad11[3]; CIVCountedPointer m_data; unsigned char m_state; unsigned char _pad19[3]; long m_sentinel; long m_a; virtual void v00() { }
    CEventVillagerInteraction(void* villager, const CIVCountedPointer& data, long a) : CEventBase_NEntityEvents(), m_flag(0), m_villager(villager), m_data(data), m_state(0), m_sentinel(0x7fffffff), m_a(a) { } }; }
#pragma pack(pop)
int main() {
    CIVCountedPointeeBase obj; obj.IVRefCount = 1; CIVCountedPointer data; data.Object = &obj; int v;
    { NEntityEvents::CEventVillagerInteraction e(&v, data, 42); if (e.m_eventType != 0 || e.m_flag != 0 || e.m_villager.m_pItem != &v || e.m_data.Object != &obj || obj.IVRefCount != 2 || e.m_state != 0 || e.m_sentinel != 0x7fffffff || e.m_a != 42) { std::printf("00982320_TEST FAIL\n"); return 1; } }
    if (obj.IVRefCount != 1) { std::printf("00982320_TEST FAIL\n"); return 1; } data.Object = 0;
    std::printf("00982320_TEST PASS\n"); return 0;
}