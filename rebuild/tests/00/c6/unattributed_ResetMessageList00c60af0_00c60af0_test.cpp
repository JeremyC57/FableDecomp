#include <cstdio>
#include <cstring>
struct CMessageNode { int id; };
static CMessageNode g_node; static int g_cleared = 0, g_posted = 0;
struct CMessageOwner { unsigned char _pad[0x3b4]; unsigned long m_state; unsigned char _pad3b8[0x0c]; CMessageNode* m_pMessages;
    void ClearMessages() { ++g_cleared; m_pMessages = 0; }
    CMessageNode* PostMessage() { ++g_posted; m_pMessages = &g_node; return &g_node; }
    CMessageNode* ResetMessageList_00c60af0(CMessageNode** ppHead) { CMessageNode* pPosted = 0; ClearMessages(); if (!(m_state & 0x80000000)) pPosted = PostMessage(); *ppHead = m_pMessages; return pPosted; } };
int main()
{
    CMessageOwner o; std::memset(&o, 0, sizeof(o)); CMessageNode* head = (CMessageNode*)1;
    o.m_state = 0xffffffff; CMessageNode* r1 = o.ResetMessageList_00c60af0(&head); CMessageNode* h1 = head;
    o.m_state = 3; CMessageNode* r2 = o.ResetMessageList_00c60af0(&head);
    if (r1 != 0 || h1 != 0 || r2 != &g_node || head != &g_node || g_cleared != 2 || g_posted != 1 || (char*)&o.m_pMessages - (char*)&o != 0x3c4) { std::printf("00c60af0_TEST FAIL\n"); return 1; }
    std::printf("00c60af0_TEST PASS\n");
    return 0;
}