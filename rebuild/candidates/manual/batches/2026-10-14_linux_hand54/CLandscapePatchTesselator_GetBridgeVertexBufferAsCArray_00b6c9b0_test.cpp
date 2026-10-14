#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SV { float x, y, z, nx, ny, nz; }; template<class T> struct CVec { T* m_first; T* m_last; T* m_end; long size() const { return m_last - m_first; } };
struct CTess { unsigned char _pad[0x14]; CVec<SV> m_v; SV* Get() { CVec<SV>* v = &m_v; return v->size() != 0 ? v->m_first : (SV*)v; } };

int main() {
    CTess t; SV buf[3]; t.m_v.m_first = buf; t.m_v.m_last = buf + 2; t.m_v.m_end = buf + 3; if (t.Get() != buf) { std::printf("00b6c9b0_TEST FAIL\n"); return 1; } t.m_v.m_last = buf; if ((void*)t.Get() != (void*)&t.m_v) { std::printf("00b6c9b0_TEST FAIL\n"); return 1; }
    std::printf("00b6c9b0_TEST PASS\n"); return 0;
}
