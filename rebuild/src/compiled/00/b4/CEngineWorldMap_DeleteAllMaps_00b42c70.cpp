// CEngineWorldMap::DeleteAllMaps() -- delete every engine map by index, shrink the map list to nothing and free the map grid
// Leaf: DeleteAllMaps
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B42C70, 154 bytes (+ int3 padding).
// Label: the manifest calls this "DeleteMap" too (the one-argument DeleteMap(long) is 0x00B42B10, which this body calls
//   for each index); renamed after what it does.
// Callees: CEngineWorldMap::DeleteMap(long) @ 0x00B42B10, memmove @ 0x00BFEAE6, the map list's InsertN(where, count,
//   const value&) @ 0x00B43F80 (manifest "std_vector_insert_ENavigatorType", propagated), operator delete @ 0x00BFE9BC.
// Layout (retail): Map (the short* grid) +0x0c, MapList (CArray<CEngineMap*>: begin, end, capacity) +0x20.
// Shape: the map count is re-derived every pass (unsigned compare); then `MapList.resize(0)` -- Dinkumware resize with
//   the default value `T()` materialised in the `push ecx` frame word, `if (n < size()) erase(begin() + n, end()); else
//   InsertN(end(), n - size(), value)` with the copy-down helper as an early-out (`cmp ecx,ecx ; jne`, GOTCHAS wave 17,
//   as the landed 0x005697C0); last `if (Map) { delete Map; Map = 0; }`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);

struct CEngineMap;

template <class T> struct CArray {               // retail CArray: begin, end, capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; }
    static T* CopyDown(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (unsigned long)((char*)srcEnd - (char*)src);
        return (T*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void InsertN(T* where, unsigned long count, const T& value);   // 0x00B43F80
    void Resize(unsigned long n, const T& value = T())
    {
        if (n < Size())
            m_pEnd = CopyDown(m_pEnd, m_pEnd, m_pBegin + n);   // erase(begin() + n, end())
        else
            InsertN(m_pEnd, n - Size(), value);
    }
};

struct CEngineWorldMap {
    unsigned char        _pad0[0x0c];
    short*               Map;                    // +0x0c
    unsigned char        _pad10[0x20 - 0x10];
    CArray<CEngineMap*>  MapList;                // +0x20
    void DeleteMap(long index);                  // 0x00B42B10
    void DeleteAllMaps();
};

void CEngineWorldMap::DeleteAllMaps()
{
    for (unsigned long i = 0; i < MapList.Size(); ++i)
        DeleteMap(i);
    MapList.Resize(0);
    if (Map) {
        delete Map;
        Map = 0;
    }
}