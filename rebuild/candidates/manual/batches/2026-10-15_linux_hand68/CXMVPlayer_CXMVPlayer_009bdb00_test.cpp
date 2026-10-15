// Behaviour test for CXMVPlayer::CXMVPlayer(bool, unsigned long) (0x009BDB00): null threaded-file pointer and an empty
// subtitle list whose allocated sentinel links to itself; the arguments are not stored.
#include <cstdio>
#include <cstdlib>
static int g_baseCalls = 0;
struct CBaseClassNonCopyable { CBaseClassNonCopyable() { ++g_baseCalls; } virtual ~CBaseClassNonCopyable() { } };
struct CCountedPointer { void* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct SNode { SNode* Next; SNode* Prev; unsigned char SubTitle[0x1c]; };
struct CList {
    SNode* m_pNode;
    CList() : m_pNode(0) { SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->Next = n; n->Prev = n; m_pNode = n; }
};
struct CXMVPlayer : CBaseClassNonCopyable {
    CCountedPointer PThreadedFile; long FilePosition; bool AllowSkipping; long LocalXMVPlayerRefCount; CList SubTitles;
    CXMVPlayer(bool, unsigned long) { }
};
int main()
{
    CXMVPlayer p(true, 30);
    SNode* n = p.SubTitles.m_pNode;
    bool ok = g_baseCalls == 1 && p.PThreadedFile.Data == 0 && p.PThreadedFile.Info == 0 && n != 0 && n->Next == n && n->Prev == n;
    std::free(n);
    if (!ok) { std::printf("009bdb00_TEST FAIL\n"); return 1; }
    std::printf("009bdb00_TEST PASS\n");
    return 0;
}
