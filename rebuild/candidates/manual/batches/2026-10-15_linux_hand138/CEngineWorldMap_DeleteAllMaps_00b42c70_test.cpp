// Behaviour test for 0x00b42c70 CEngineWorldMap::DeleteAllMaps
#include <cstdio>
#include <cstring>
struct CEngineMap;
static int g_deleted[8]; static int g_nDeleted = 0; static int g_inserts = 0;
template <class T> struct CArray {
    T* m_pBegin; T* m_pEnd; T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; }
    static T* CopyDown(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd) return dest;
        unsigned long bytes = (unsigned long)((char*)srcEnd - (char*)src);
        return (T*)((char*)std::memmove(dest, src, bytes) + bytes);
    }
    void InsertN(T* where, unsigned long count, const T& value) { ++g_inserts; }
    void Resize(unsigned long n, const T& value = T())
    {
        if (n < Size()) m_pEnd = CopyDown(m_pEnd, m_pEnd, m_pBegin + n);
        else InsertN(m_pEnd, n - Size(), value);
    }
};
struct CEngineWorldMap {
    short* Map; CArray<CEngineMap*> MapList;
    void DeleteMap(long index) { g_deleted[g_nDeleted++] = index; }
    void DeleteAllMaps()
    {
        for (unsigned long i = 0; i < MapList.Size(); ++i) DeleteMap(i);
        MapList.Resize(0);
        if (Map) { delete Map; Map = 0; }
    }
};
int main()
{
    CEngineMap* maps[3] = { 0, 0, 0 };
    CEngineWorldMap w; w.Map = new short; w.MapList.m_pBegin = maps; w.MapList.m_pEnd = w.MapList.m_pCapacity = maps + 3;
    w.DeleteAllMaps();
    bool ok = g_nDeleted == 3 && g_deleted[0] == 0 && g_deleted[2] == 2 && w.MapList.Size() == 0 && w.Map == 0 && g_inserts == 0;
    w.DeleteAllMaps();   // empty list: the resize goes through the (zero-count) insert path
    ok = ok && g_nDeleted == 3 && g_inserts == 1;
    if (!ok) { std::printf("00b42c70_TEST FAIL\n"); return 1; }
    std::printf("00b42c70_TEST PASS\n");
    return 0;
}
