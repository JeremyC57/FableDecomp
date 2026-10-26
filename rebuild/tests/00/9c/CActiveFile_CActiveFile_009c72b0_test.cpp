#include <cstdio>
#include <cstdlib>
class CCPPointerInfo { public: long RefCount; };
struct CCountedPointerFileInfo {
    void* Data; CCPPointerInfo* Info;
    CCountedPointerFileInfo(void* d, CCPPointerInfo* i) : Data(d), Info(i) { }
    CCountedPointerFileInfo(const CCountedPointerFileInfo& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};
struct Node { Node* next; Node* prev; int value; };
struct List {
    Node* head;
    List() { head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head; }
    void push_back(int v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->value = v; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    List(const List& x)
    {
        head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head;
        for (Node* p = x.head->next; p != x.head; p = p->next) push_back(p->value);
    }
};
struct CActiveFile {
    unsigned long Handles[7];
    CCountedPointerFileInfo PFileInfo;
    unsigned long MinWriteSize, MinReadSize;
    double StartTime;
    List PendingWrites, PendingDecompression;
    unsigned long DecompressSrcPos, DecompressDstUsed;
    bool DecompressFinish;
    CActiveFile(CCPPointerInfo* info) : PFileInfo(0, info) { }
    CActiveFile(const CActiveFile& o)
        : PFileInfo(o.PFileInfo), MinWriteSize(o.MinWriteSize), MinReadSize(o.MinReadSize), StartTime(o.StartTime),
          PendingWrites(o.PendingWrites), PendingDecompression(o.PendingDecompression),
          DecompressSrcPos(o.DecompressSrcPos), DecompressDstUsed(o.DecompressDstUsed), DecompressFinish(o.DecompressFinish)
    { for (int i = 0; i < 7; ++i) Handles[i] = o.Handles[i]; }
};
int main()
{
    CCPPointerInfo info = { 1 };
    CActiveFile a(&info);
    for (int i = 0; i < 7; ++i) a.Handles[i] = 100 + i;
    a.MinWriteSize = 5; a.MinReadSize = 6; a.StartTime = 2.5; a.DecompressSrcPos = 9; a.DecompressDstUsed = 10; a.DecompressFinish = true;
    a.PendingWrites.push_back(1); a.PendingWrites.push_back(2); a.PendingDecompression.push_back(7);
    CActiveFile b(a);
    if (info.RefCount != 2 || b.Handles[6] != 106 || b.StartTime != 2.5 || !b.DecompressFinish || b.MinReadSize != 6) { std::printf("009c72b0_TEST FAIL\n"); return 1; }
    if (b.PendingWrites.head == a.PendingWrites.head || b.PendingWrites.head->next->value != 1 || b.PendingWrites.head->prev->value != 2
        || b.PendingDecompression.head->next->value != 7 || b.PendingDecompression.head->next->next != b.PendingDecompression.head) { std::printf("009c72b0_TEST FAIL\n"); return 1; }
    CActiveFile c(&info); CActiveFile d(c);   // empty lists stay self-linked
    if (d.PendingWrites.head->next != d.PendingWrites.head || info.RefCount != 3) { std::printf("009c72b0_TEST FAIL\n"); return 1; }
    std::printf("009c72b0_TEST PASS\n");
    return 0;
}