#include <cstdio>
class CContainmentVolume { public: void* __vftable; void* ContainmentPlanes; unsigned long Size; void* MemoryPool; };
struct CDataOutputStream_Calls {
    long m_long; const void* m_data; long m_size;
    void WriteLong(long value) { m_long = value; }
    void WriteBuffer(const void* data, long size) { m_data = data; m_size = size; }
};
struct CContainmentVolume_Methods : CContainmentVolume {
    void SaveBinary(CDataOutputStream_Calls& stream) const;
};
void CContainmentVolume_Methods::SaveBinary(CDataOutputStream_Calls& stream) const
{
    stream.WriteLong(Size);
    stream.WriteBuffer(ContainmentPlanes, Size * 16);
}
int main()
{
    float planes[3 * 4];
    CContainmentVolume_Methods v; v.ContainmentPlanes = planes; v.Size = 3;
    CDataOutputStream_Calls s = { -1, 0, -1 };
    v.SaveBinary(s);
    if (s.m_long != 3 || s.m_data != planes || s.m_size != 48) { std::printf("00a75400_TEST FAIL\n"); return 1; }
    std::printf("00a75400_TEST PASS\n");
    return 0;
}