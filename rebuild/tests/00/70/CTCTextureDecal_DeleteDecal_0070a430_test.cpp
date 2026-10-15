#include <cstdio>
#include <stdlib.h>
// Behaviour test for CTCTextureDecal::DeleteDecal (0x0070A430).
struct CDecalEntry { long m_allocEntryID; unsigned char _rest[0x14]; };
struct SDecalNode { SDecalNode* Next; SDecalNode* Prev; CDecalEntry Entry; };
struct CRetailList_DecalEntry {
    SDecalNode* Node;
    SDecalNode* Begin() { return Node->Next; }
    SDecalNode* End() { return Node; }
    SDecalNode* Erase(SDecalNode* pos)
    {
        SDecalNode* next = pos->Next;
        SDecalNode* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
        return next;
    }
};
struct CThing { int requests; long last; void ForwardToGraphicAppearance(long r) { ++requests; last = r; } };
struct CTCTextureDecal {
    CThing* m_pParentThing;
    CRetailList_DecalEntry Decals;
    bool Dirty;
    void DeleteDecal(long id);
};
void CTCTextureDecal::DeleteDecal(long id)
{
    bool found = false;
    for (SDecalNode* it = Decals.Begin(); it != Decals.End(); ) {
        if (it->Entry.m_allocEntryID == id) {
            it = Decals.Erase(it);
            found = true;
        } else {
            it = it->Next;
        }
    }
    if (found) {
        Dirty = true;
        m_pParentThing->ForwardToGraphicAppearance(4);
    }
}
static int Count(SDecalNode* s) { int c = 0; for (SDecalNode* n = s->Next; n != s; n = n->Next) ++c; return c; }
int main()
{
    SDecalNode* s = (SDecalNode*)malloc(sizeof(SDecalNode)); s->Next = s->Prev = s;
    long ids[4] = { 3, 8, 3, 3 };
    for (int i = 0; i < 4; ++i) {
        SDecalNode* n = (SDecalNode*)malloc(sizeof(SDecalNode)); n->Entry.m_allocEntryID = ids[i];
        n->Prev = s->Prev; n->Next = s; s->Prev->Next = n; s->Prev = n;
    }
    CThing thing; thing.requests = 0; thing.last = 0;
    CTCTextureDecal d; d.m_pParentThing = &thing; d.Decals.Node = s; d.Dirty = false;
    d.DeleteDecal(42);
    if (d.Dirty || thing.requests != 0 || Count(s) != 4) { std::printf("0070a430_TEST FAIL\n"); return 1; }
    d.DeleteDecal(3);
    if (!d.Dirty || thing.requests != 1 || thing.last != 4 || Count(s) != 1 || s->Next->Entry.m_allocEntryID != 8 || s->Prev != s->Next)
    { std::printf("0070a430_TEST FAIL\n"); return 1; }
    std::printf("0070a430_TEST PASS\n");
    return 0;
}