#include <cstdio>
struct CContextItem_Retail { unsigned char _pad[0x2c]; long m_priority; };
struct SContextEntry_Retail { long m_id; CContextItem_Retail* m_pItem; };
long __fastcall GetContextSensitivePriority_00581bde(long id) { return id * 10; }
struct CContextSensitivePriorityCompare {
    static __forceinline long GetPriority(const SContextEntry_Retail& entry)
    {
        if (entry.m_pItem)
            return entry.m_pItem->m_priority;
        return GetContextSensitivePriority_00581bde(entry.m_id);
    }
    bool operator()(const SContextEntry_Retail& left, const SContextEntry_Retail& right) const;
};
bool CContextSensitivePriorityCompare::operator()(const SContextEntry_Retail& left, const SContextEntry_Retail& right) const
{
    return GetPriority(left) > GetPriority(right);
}
int main()
{
    CContextItem_Retail item; item.m_priority = 25;
    SContextEntry_Retail a = { 3, 0 };        // priority 30
    SContextEntry_Retail b = { 0, &item };    // priority 25
    CContextSensitivePriorityCompare cmp;
    if (!cmp(a, b) || cmp(b, a) || cmp(a, a)) { std::printf("005928a8_TEST FAIL\n"); return 1; }
    std::printf("005928a8_TEST PASS\n");
    return 0;
}