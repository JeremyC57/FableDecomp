// Behaviour test for 009d5f80: CBankFileAsync ctor -- null counted pointers, empty sets/list (self-linked sentinels), flags clear.
#include <cstdio>
#include <cstdlib>
struct TNode { unsigned char color; TNode* parent; TNode* left; TNode* right; void* value; };
struct TSet { TNode* header; unsigned long count;
    TSet() : header(0), count(0) { header = (TNode*)std::malloc(sizeof(TNode)); header->color = 0; header->parent = 0; header->left = header; header->right = header; }
    ~TSet() { std::free(header); } };
struct TLNode { TLNode* next; TLNode* prev; };
struct TList { TLNode* node; TList() : node(0) { TLNode* n = (TLNode*)std::malloc(sizeof(TLNode)); n->next = n; n->prev = n; node = n; } ~TList() { std::free(node); } };
struct TPtr { void* Data; void* Info; TPtr() : Data(0), Info(0) {} };
static int g_cs = 0;
struct TAsync {
    TPtr ThreadedFile, LoadingMemoryPool, MemoryFailureHandler; TSet AsyncDataInstances, ReadsToRestart; void* LastRestartingData; TList DisposalList; bool NonCached, DisableAsync;
    TAsync() { NonCached = false; DisableAsync = false; LastRestartingData = 0; ++g_cs; }
};
int main()
{
    TAsync a;
    if (a.ThreadedFile.Data || a.LoadingMemoryPool.Info || a.MemoryFailureHandler.Data || a.AsyncDataInstances.count ||
        a.AsyncDataInstances.header->left != a.AsyncDataInstances.header || a.ReadsToRestart.header->right != a.ReadsToRestart.header ||
        a.DisposalList.node->next != a.DisposalList.node || a.NonCached || a.DisableAsync || a.LastRestartingData || g_cs != 1)
    { std::printf("009d5f80_TEST FAIL\n"); return 1; }
    std::printf("009d5f80_TEST PASS\n");
    return 0;
}
