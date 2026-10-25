#include <cstdio>
#include <new>
static int g_dtor = 0;
class CSteeringGroup {
public:
    CSteeringGroup() : m_index(-1), m_pFirst(0) { }
    ~CSteeringGroup() { ++g_dtor; }
    virtual long* GetIndexSlot() { return &m_index; }
    long m_index; void* m_pFirst;
};
int main()
{
    CSteeringGroup* groups = new CSteeringGroup[3];
    bool ok = *groups[2].GetIndexSlot() == -1 && sizeof(CSteeringGroup) == 12;
    delete[] groups;
    ok = ok && g_dtor == 3;
    if (!ok) { std::printf("00c29eb0_TEST FAIL\n"); return 1; }
    std::printf("00c29eb0_TEST PASS\n");
    return 0;
}
