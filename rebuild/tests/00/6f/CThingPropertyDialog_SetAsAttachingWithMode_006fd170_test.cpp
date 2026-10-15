#include <cstdio>
struct SElement {
    float x, y, z;
    SElement() : x(0.0f), y(0.0f), z(0.0f) {}
};
struct SArrayHolder {
    char* m_pData;
    SArrayHolder() : m_pData(0) {}
    ~SArrayHolder() { delete[] m_pData; }
};
struct CThingPropertyDialog {
    SElement m_store[8];
    unsigned long m_size;
    void Resize(unsigned long count, const SElement& value)
    {
        while (m_size < count) m_store[m_size++] = value;
        m_size = count;
    }
    void SetAsAttachingWithMode(unsigned long count);
};
void CThingPropertyDialog::SetAsAttachingWithMode(unsigned long count)
{
    SArrayHolder holder;
    Resize(count, SElement());
}
int main()
{
    int bad = 0, i;
    CThingPropertyDialog v; v.m_size = 0;
    for (i = 0; i < 8; ++i) { v.m_store[i].x = 9.0f; v.m_store[i].y = 9.0f; v.m_store[i].z = 9.0f; }
    v.SetAsAttachingWithMode(3);
    if (v.m_size != 3) bad = 1;
    for (i = 0; i < 3; ++i) if (v.m_store[i].x != 0.0f || v.m_store[i].y != 0.0f || v.m_store[i].z != 0.0f) bad = 2;
    if (v.m_store[3].x != 9.0f) bad = 3;
    v.SetAsAttachingWithMode(1);
    if (v.m_size != 1) bad = 4;
    if (bad) { std::printf("006fd170_TEST FAIL\n"); return 1; }
    std::printf("006fd170_TEST PASS\n");
    return 0;
}