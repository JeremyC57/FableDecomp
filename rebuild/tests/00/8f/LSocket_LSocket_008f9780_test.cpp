#include <cstdio>
#include <cstdlib>
static int g_closed, g_nullDeletes;
struct CMallocBuffer { void* Data; ~CMallocBuffer() { if (Data) std::free(Data); } };
struct LAddress { CMallocBuffer Name; };
struct CCharVector { CMallocBuffer Start; };
class LSocket;
struct CSocketListNode { CSocketListNode* Next; CSocketListNode* Prev; LSocket* Value; };
struct CSocketList
{
    CSocketListNode* Node;
    void erase(CSocketListNode* pos)
    {
        CSocketListNode* next = pos->Next;
        CSocketListNode* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        std::free(pos);
    }
};
static CSocketList g_Sockets;
static void NullDelete(void* p) { if (p == 0) ++g_nullDeletes; }
class LSocket
{
public:
    LAddress RemoteAddress, LocalAddress;
    unsigned int Socket;
    CCharVector SendOutputQueue, InternalPacketBuffer;
    void* DebugFile;
    bool Close() { ++g_closed; return true; }
    ~LSocket();
};
LSocket::~LSocket()
{
    if (Socket != (unsigned int)-1)
        Close();
    CSocketListNode* head = g_Sockets.Node;
    CSocketListNode* it = head->Next;
    while (it != head && it->Value != this)
        it = it->Next;
    g_Sockets.erase(it);
    if (!DebugFile)
        NullDelete(DebugFile);
}
static CSocketListNode* Push(CSocketListNode* head, LSocket* s)
{
    CSocketListNode* n = (CSocketListNode*)std::malloc(sizeof(CSocketListNode));
    n->Value = s; n->Next = head; n->Prev = head->Prev; head->Prev->Next = n; head->Prev = n;
    return n;
}
int main()
{
    CSocketListNode head; head.Next = head.Prev = &head; g_Sockets.Node = &head;
    LSocket* a = new LSocket; LSocket* b = new LSocket;
    LSocket* socks[2] = { a, b };
    for (int i = 0; i < 2; ++i)
    {
        LSocket* s = socks[i];
        s->RemoteAddress.Name.Data = std::malloc(4); s->LocalAddress.Name.Data = 0;
        s->SendOutputQueue.Start.Data = std::malloc(8); s->InternalPacketBuffer.Start.Data = 0;
        Push(&head, s);
    }
    a->Socket = 5; a->DebugFile = 0;
    b->Socket = (unsigned int)-1; b->DebugFile = &head;
    delete a;
    if (g_closed != 1 || g_nullDeletes != 1 || head.Next->Value != b || head.Next->Next != &head) { std::printf("008f9780_TEST FAIL\n"); return 1; }
    delete b;
    if (g_closed != 1 || g_nullDeletes != 1 || head.Next != &head || head.Prev != &head) { std::printf("008f9780_TEST FAIL\n"); return 1; }
    std::printf("008f9780_TEST PASS\n");
    return 0;
}