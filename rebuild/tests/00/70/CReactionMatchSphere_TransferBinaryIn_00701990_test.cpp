// Behaviour test for 0x00701990: Sphere::TransferBinaryIn reads base, centre, then a 4-byte float via the chunk fast path or the overflow reader.
#include <cstdio>
#include <cstring>
struct Stream { unsigned int m_position; const unsigned char* m_pData; int m_remaining; int overflow; float ovValue; int calls;
  void ReadWithSrcChunkOverflow(unsigned char* b, int size) { ++overflow; std::memcpy(b, &ovValue, size); }
  void Read(void* buffer, int size) { long long np = (long long)(m_position + size); if (!(np >= 0 && np <= 0x7fffffff)) return;
    if (m_remaining >= size) { std::memcpy(buffer, m_pData, size); m_pData += size; m_remaining -= size; m_position += size; } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size); } };
struct M { int based, centred; float SqRadius;
  void TransferBinaryIn(Stream& s) { ++based; ++centred; float r = 0.0f; s.Read(&r, sizeof(r)); SqRadius = r; } };
int main()
{
    float v = 2.5f; unsigned char buf[4]; std::memcpy(buf, &v, 4);
    Stream s = {8, buf, 4, 0, 9.0f, 0}; M m = {0, 0, 0.0f}; m.TransferBinaryIn(s);
    bool ok = m.SqRadius == 2.5f && s.m_position == 12 && s.m_remaining == 0 && s.overflow == 0 && m.based == 1 && m.centred == 1;
    Stream t = {8, buf, 2, 0, 9.0f, 0}; M n = {0, 0, 0.0f}; n.TransferBinaryIn(t);
    ok = ok && n.SqRadius == 9.0f && t.overflow == 1;
    if (!ok) { std::printf("00701990_TEST FAIL\n"); return 1; }
    std::printf("00701990_TEST PASS\n");
    return 0;
}