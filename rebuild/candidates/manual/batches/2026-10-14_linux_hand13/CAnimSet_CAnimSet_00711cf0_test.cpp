#include <cstdio>
#include <new>
#include <cstdlib>
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage; CRetailVector() : first(0), last(0), end_of_storage(0) {} };
struct CTransitionBlendAlpha { float Current; float Last; };
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    static node* Buynode() { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; return s; }
    CRetailList() : head(0) { head = Buynode(); }
};
struct SAnimListEntry { unsigned char _body[0x54]; };
struct CAnimSet {
    long UpdateMode; CRetailVector<short> Anims; CRetailVector<CTransitionBlendAlpha> InternalAlphas;
    float a, b, c; unsigned long FrameInternalAlphasUpdated; float d, e, f; float ChangeAlphaBy; void* PAnimator;
    unsigned long Flags, TypeFlags; long AnimDelay, Priority; unsigned char Type;
    unsigned char Looping : 1; unsigned char Finished : 1; unsigned char Forwards : 1; unsigned char HadUpdate : 1; unsigned char OwnedByMainAnim : 1;
    unsigned char _pad[2];
    CRetailList<SAnimListEntry> PendingEntries; CRetailList<SAnimListEntry> FinishedEntries;
    CAnimSet() : ChangeAlphaBy(0.0f), PAnimator(0), HadUpdate(0) {}
};
int main()
{
    if (sizeof(CAnimSet) != 0x5c || sizeof(CRetailList<SAnimListEntry>::node) != 0x5c) { std::printf("00711cf0_TEST FAIL\n"); return 1; }
    unsigned char raw[sizeof(CAnimSet)]; for (int i = 0; i < (int)sizeof raw; ++i) raw[i] = 0xff;
    CAnimSet* s = new (raw) CAnimSet();
    if (s->Anims.first || s->Anims.end_of_storage || s->InternalAlphas.last || s->ChangeAlphaBy != 0.0f || s->PAnimator || s->HadUpdate || !s->Looping || !s->OwnedByMainAnim) { std::printf("00711cf0_TEST FAIL\n"); return 1; }
    if (!s->PendingEntries.head || s->PendingEntries.head->next != s->PendingEntries.head || s->PendingEntries.head->prev != s->PendingEntries.head) { std::printf("00711cf0_TEST FAIL\n"); return 1; }
    if (!s->FinishedEntries.head || s->FinishedEntries.head == s->PendingEntries.head || s->FinishedEntries.head->next != s->FinishedEntries.head) { std::printf("00711cf0_TEST FAIL\n"); return 1; }
    std::free(s->PendingEntries.head); std::free(s->FinishedEntries.head);
    std::printf("00711cf0_TEST PASS\n");
    return 0;
}
