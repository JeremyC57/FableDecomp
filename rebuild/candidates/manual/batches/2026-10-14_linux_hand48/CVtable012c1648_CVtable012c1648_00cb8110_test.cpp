#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static char g_seq[64] = "";
static unsigned int g_size = 0; static void* MallocStub(unsigned int size) { std::strcat(g_seq, "m"); g_size = size; return std::malloc(size); }
struct CBase { virtual void v00() { } CBase() { std::strcat(g_seq, "B"); } };
struct SListNode { SListNode* m_pNext; SListNode* m_pPrev; unsigned char _payload[8]; };
struct CList { SListNode* m_pNode; CList() { m_pNode = 0; SListNode* node = (SListNode*)MallocStub(0x10); node->m_pNext = node; node->m_pPrev = node; m_pNode = node; } };
struct SMorphRecord { long m_a, m_b, m_c; unsigned char _padc; bool m_flag; unsigned char _pade[2]; SMorphRecord() : m_a(0), m_b(0), m_c(0), m_flag(false) { } };
struct SRecord16 { long m_a, m_b, m_c, m_d; SRecord16() : m_a(0), m_b(0), m_c(0), m_d(0) { } };
struct CObject : CBase { CList m_list; SMorphRecord m_morph08, m_morph18; long m_28; SRecord16 m_record2c; unsigned char _pad3c; bool m_3d; virtual void v00() { } CObject() : CBase() { m_3d = false; } };

int main() {
    unsigned char raw[sizeof(CObject)]; std::memset(raw, 0xff, sizeof raw); CObject& o = *new (raw) CObject; if (std::strcmp(g_seq, "Bm") || !o.m_list.m_pNode || o.m_list.m_pNode->m_pNext != o.m_list.m_pNode || o.m_list.m_pNode->m_pPrev != o.m_list.m_pNode || g_size != 0x10 || o.m_morph08.m_a || o.m_morph18.m_flag || o.m_28 != -1 || o.m_record2c.m_d || o.m_3d) { std::printf("00cb8110_TEST FAIL\n"); return 1; }
    std::printf("00cb8110_TEST PASS\n"); return 0;
}
