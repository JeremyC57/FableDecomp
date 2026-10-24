#include <cstdio>
#include <cstdlib>
#include <new>
struct CCameraTag { long CameraPositionPerson, CameraPosition, CameraFocusPerson, CameraFocusPosition, CameraZoomType, CameraView; };
struct SEntry {
    float first; CCameraTag second;
    SEntry() : first(0.0f) { second.CameraPositionPerson = second.CameraPosition = second.CameraFocusPerson = 0; second.CameraFocusPosition = second.CameraZoomType = second.CameraView = 0; }
    SEntry(const SEntry& p) : first(p.first), second(p.second) { }
};
static SEntry* UninitializedCopy(const SEntry* first, const SEntry* last, SEntry* result)
{
    for (; first != last; ++first, ++result)
        new (result) SEntry(*first);
    return result;
}
struct CVector {
    SEntry* m_start; SEntry* m_finish; SEntry* m_end;
    unsigned long size() const { return (unsigned long)(m_finish - m_start); }
    CVector() : m_start(0), m_finish(0), m_end(0) { }
    CVector(const CVector& x) : m_start(0), m_finish(0), m_end(0)
    {
        unsigned long n = x.size();
        m_start = n != 0 ? static_cast<SEntry*>(std::malloc(n * sizeof(SEntry))) : 0;
        m_finish = m_start;
        m_end = m_start + n;
        m_finish = UninitializedCopy(x.m_start, x.m_finish, m_start);
    }
};
int main()
{
    bool ok = true;
    SEntry src[5];
    for (int i = 0; i < 5; ++i) { src[i].first = 0.5f * i; src[i].second.CameraView = 100 + i; src[i].second.CameraPositionPerson = -i; }
    CVector a; a.m_start = src; a.m_finish = src + 5; a.m_end = src + 5;
    CVector b(a);
    if (b.size() != 5 || b.m_end != b.m_finish) ok = false;
    for (int i = 0; ok && i < 5; ++i)
        if (b.m_start[i].first != 0.5f * i || b.m_start[i].second.CameraView != 100 + i || b.m_start[i].second.CameraPositionPerson != -i) ok = false;
    CVector e;
    CVector c(e);
    if (c.m_start != 0 || c.m_finish != 0 || c.m_end != 0) ok = false;
    std::free(b.m_start);
    if (!ok) { std::printf("004d08a0_TEST FAIL\n"); return 1; }
    std::printf("004d08a0_TEST PASS\n");
    return 0;
}