// behaviour test for 0x00E299F0 CScriptRecordSet_00e299f0(a, b): base first, m_40 = b, m_44 = a, two empty self-linked maps
#include <cstdio>
#include <cstdlib>
static int g_baseBuilt = 0;
struct Node { char color; Node* parent; Node* left; Node* right; };
struct CMap {
    Node* header; unsigned int count;
    CMap() : header(0) { header = (Node*)std::malloc(sizeof(Node)); count = 0; header->color = 0; header->parent = 0; header->left = header; header->right = header; }
    ~CMap() { std::free(header); }
};
struct CBase { CBase() { ++g_baseBuilt; } virtual ~CBase() {} };
struct CMid : CBase { int m_40; int m_44; CMid(int a, int b) : m_40(b), m_44(a) {} };
struct CScriptRecordSet : CMid { int m_48; CMap m_map4c; CMap m_map58; CScriptRecordSet(int a, int b) : CMid(a, b) {} };
int main()
{
    CScriptRecordSet s(5, 6);
    if (g_baseBuilt != 1 || s.m_40 != 6 || s.m_44 != 5 || s.m_map4c.count != 0 || s.m_map4c.header->left != s.m_map4c.header ||
        s.m_map58.header->right != s.m_map58.header || s.m_map58.header->parent != 0) { std::printf("00e299f0_TEST FAIL\n"); return 1; }
    std::printf("00e299f0_TEST PASS\n");
    return 0;
}