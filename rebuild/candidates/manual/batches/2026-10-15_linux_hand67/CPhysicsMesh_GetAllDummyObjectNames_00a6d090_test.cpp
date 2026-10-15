#include <cstdio>
struct CCharString { const char* m_pData; };
struct SRecord { CCharString m_name; unsigned char _rest[0x34]; };
struct CCharStringVector {
    CCharString m_items[16]; unsigned long m_count; unsigned long m_capacity;
    void reserve(unsigned long count) { if (count > m_capacity) m_capacity = count; }
    void push_back(const CCharString& s) { m_items[m_count++] = s; }
};
struct CAddName {
    CCharStringVector* m_pNames;
    CAddName(CCharStringVector& names) : m_pNames(&names) { }
    void operator()(const SRecord& d) { m_pNames->push_back(d.m_name); }
};
CAddName ForEach_AddName(const SRecord* first, const SRecord* last, CAddName f)
{ for (; first != last; ++first) f(*first); return f; }
struct CPhysicsMesh {
    SRecord* m_pFirst; SRecord* m_pLast;
    void GetAllDummyObjectNames(CCharStringVector& names) const
    {
        names.reserve(m_pLast - m_pFirst);
        ForEach_AddName(m_pFirst, m_pLast, CAddName(names));
    }
};
int main()
{
    SRecord d[3]; d[0].m_name.m_pData = "a"; d[1].m_name.m_pData = "b"; d[2].m_name.m_pData = "c";
    CPhysicsMesh m; m.m_pFirst = d; m.m_pLast = d + 3;
    CCharStringVector v; v.m_count = 1; v.m_capacity = 1; v.m_items[0].m_pData = "x";
    m.GetAllDummyObjectNames(v);
    if (v.m_count != 4 || v.m_capacity != 3 || v.m_items[1].m_pData != d[0].m_name.m_pData || v.m_items[3].m_pData != d[2].m_name.m_pData) { std::printf("00a6d090_TEST FAIL\n"); return 1; }
    m.m_pLast = d; m.GetAllDummyObjectNames(v);
    if (v.m_count != 4) { std::printf("00a6d090_TEST FAIL\n"); return 1; }
    std::printf("00a6d090_TEST PASS\n");
    return 0;
}
