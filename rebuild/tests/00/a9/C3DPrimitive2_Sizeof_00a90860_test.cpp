// Behaviour test for 0x00A90860 C3DPrimitive2::Sizeof
#include <cstdio>
struct CSized { unsigned long n; unsigned long Sizeof() const { return n; } };
template<class T> struct CArray { T* m_begin; T* m_end; T* m_capacity; unsigned long Size() const { return (unsigned long)(m_end - m_begin); } };
template<class T> struct scoped_ptr { T* m_p; };
template<class T> struct CCountedPointer { T* Data; void* Info; };
inline unsigned long GetSizeof(const unsigned short&) { return 2; }
template<class T> inline unsigned long GetSizeof(const scoped_ptr<T>& p) { return p.m_p ? p.m_p->Sizeof() + 4 : 4; }
template<class T> inline unsigned long GetSizeof(const CCountedPointer<T>& p) { return p.Data ? p.Data->Sizeof() + 8 : 8; }
template<class T> inline unsigned long GetSizeof(const CArray<T>& a)
{
    unsigned long total = 12 + a.Size() * 4;
    for (const T* p = a.m_begin; p != a.m_end; ++p) total += GetSizeof(*p);
    return total;
}
struct C3DPrimitive2 {
    scoped_ptr<CSized> Data, VertexBuffer, IndexBuffer; CArray<unsigned short> Partitioned; CCountedPointer<CSized> PartitionTree;
    unsigned long Sizeof() const
    {
        unsigned long size = 0xa0 + GetSizeof(Data);
        size += GetSizeof(VertexBuffer);
        size += GetSizeof(IndexBuffer);
        size += GetSizeof(Partitioned);
        return size + GetSizeof(PartitionTree);
    }
};
int main()
{
    CSized d = { 100 }, ib = { 50 }, tree = { 7 };
    unsigned short idx[3] = { 1, 2, 3 };
    C3DPrimitive2 p; p.Data.m_p = &d; p.VertexBuffer.m_p = 0; p.IndexBuffer.m_p = &ib;
    p.Partitioned.m_begin = idx; p.Partitioned.m_end = idx + 3; p.Partitioned.m_capacity = idx + 3;
    p.PartitionTree.Data = &tree; p.PartitionTree.Info = 0;
    unsigned long want = 0xa0 + 104 + 4 + 54 + (12 + 12 + 6) + 15;
    bool ok = p.Sizeof() == want;
    p.Partitioned.m_end = idx; p.PartitionTree.Data = 0; p.Data.m_p = 0; p.IndexBuffer.m_p = 0;
    ok = ok && p.Sizeof() == 0xa0 + 4 + 4 + 4 + 12 + 8;
    if (!ok) { std::printf("00a90860_TEST FAIL\n"); return 1; }
    std::printf("00a90860_TEST PASS\n");
    return 0;
}