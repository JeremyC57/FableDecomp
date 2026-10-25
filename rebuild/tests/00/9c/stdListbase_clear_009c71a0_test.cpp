// Behaviour test for 0x009C71A0 _List_base<CFileInstaller::CActiveFile>::clear (destroy each active file, free nodes, relink sentinel).
#include <cstdio>
#include <cstdlib>

static int g_infoDeleted = 0;
static int g_compressorDtors = 0;
static int g_frees = 0;

struct Link { Link* next; Link* prev; };
struct Info { unsigned long refs; void (*deleter)(void*); void* data; };
static void Deleter(void* data) { ++*(int*)data; }

static void FreeCounted(void* p) { ++g_frees; std::free(p); }
static Link* NewHead() { Link* h = (Link*)std::malloc(sizeof(Link)); h->next = h->prev = h; return h; }
static void Append(Link* h, unsigned int size)
{
    Link* n = (Link*)std::malloc(size);
    n->next = h; n->prev = h->prev; h->prev->next = n; h->prev = n;
}
static void DestroyList(Link* h)
{
    Link* cur = h->next;
    while (cur != h) { Link* tmp = cur; cur = cur->next; FreeCounted(tmp); }
    h->next = h->prev = h;
    if (h) FreeCounted(h);
}

struct ActiveFile {
    void* infoData; Info* info; Link* pendingWrites; Link* pendingDecompression;
    ~ActiveFile()
    {
        DestroyList(pendingDecompression);
        DestroyList(pendingWrites);
        ++g_compressorDtors;
        if (info) {
            --info->refs;
            if (info->refs == 0) { info->deleter(info->data); delete info; }
        }
        infoData = 0; info = 0;
    }
};
struct Node { Link link; ActiveFile file; };

static void Clear(Link* head)
{
    Node* cur = (Node*)head->next;
    while (&cur->link != head) {
        Node* tmp = cur;
        cur = (Node*)cur->link.next;
        tmp->file.~ActiveFile();
        FreeCounted(tmp);
    }
    head->next = head; head->prev = head;
}

int main()
{
    int ok = 1;
    int deleted = 0;
    Info* shared = new Info; shared->refs = 2; shared->deleter = Deleter; shared->data = &deleted;
    Link* head = NewHead();
    for (int i = 0; i < 2; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->link.next = head; n->link.prev = head->prev; head->prev->next = &n->link; head->prev = &n->link;
        n->file.infoData = 0; n->file.info = shared;
        n->file.pendingWrites = NewHead(); Append(n->file.pendingWrites, 20);
        n->file.pendingDecompression = NewHead();
    }
    Clear(head);
    if (head->next != head || head->prev != head) ok = 0;
    if (deleted != 1 || g_compressorDtors != 2) ok = 0;
    if (g_frees != 2 + 2 * 3) ok = 0;                    // two nodes, per file: one write node and two sentinels
    std::free(head);
    if (!ok) { std::printf("009c71a0_TEST FAIL\n"); return 1; }
    std::printf("009c71a0_TEST PASS\n");
    return 0;
}