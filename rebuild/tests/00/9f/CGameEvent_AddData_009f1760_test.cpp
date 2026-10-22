#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CMemoryBuffer { unsigned char m_0[5]; unsigned char m_data[0x20]; unsigned char m_fill; void Copy(unsigned long length, const void* data) { std::memcpy(m_data + m_fill, data, length); m_fill += (unsigned char)length; } };

int main() {
    CMemoryBuffer b; std::memset(&b, 0, sizeof b); b.Copy(5, "hello"); b.Copy(6, " world"); if (b.m_fill != 11 || std::memcmp(b.m_data, "hello world", 11)) { std::printf("009f1760_TEST FAIL\n"); return 1; }
    std::printf("009f1760_TEST PASS\n"); return 0;
}