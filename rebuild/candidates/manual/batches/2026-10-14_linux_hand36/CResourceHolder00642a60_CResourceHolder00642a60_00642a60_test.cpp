#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
#include <string>
static std::string g_order; static int g_member = 0, g_holderBase = 0;
struct CMember_009f9f70 { unsigned char _body[0xc]; ~CMember_009f9f70() { ++g_member; g_order += 'm'; } };
struct CHolderBase_009f2e20 { virtual void v00() { } unsigned char _pad[0xc]; ~CHolderBase_009f2e20() { ++g_holderBase; g_order += 'h'; } };
struct CHolder_00642a60 : CHolderBase_009f2e20 { ~CHolder_00642a60() { } };
struct CBuffer_00642a60 { void* m_pBlock; char m_tag; CBuffer_00642a60() : m_pBlock(0) { } ~CBuffer_00642a60() { if (m_pBlock) { free(m_pBlock); g_order += m_tag; } } };
struct CResourceHolder_00642a60 { unsigned char _pad[8]; CBuffer_00642a60 m_bufferA; unsigned char _pad0c[8]; CHolder_00642a60 m_holder; CMember_009f9f70 m_member; CBuffer_00642a60 m_bufferB; CResourceHolder_00642a60() { m_bufferA.m_tag = 'A'; m_bufferB.m_tag = 'B'; } ~CResourceHolder_00642a60() { } };
int main() {
    int x1, x2; { CResourceHolder_00642a60 h; h.m_bufferA.m_pBlock = &x1; h.m_bufferB.m_pBlock = &x2; }
    if (g_freed != 2 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1 || g_member != 1 || g_holderBase != 1 || g_order != "BmhA") { std::printf("00642a60_TEST FAIL\n"); return 1; }
    std::printf("00642a60_TEST PASS\n"); return 0;
}
