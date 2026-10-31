// Behaviour test for CMemoryAllocatorVariableSize::RemoveFromTreeFixup (0x00A64910): red-black delete fixup.
// Builds a tree, blackens a leaf path imbalance, runs the fixup and checks the red-black invariants.
#include <cstdio>
enum { RB_RED = 0, RB_BLACK = 1 };
struct Area { Area* PRightChild; Area* PParentArea; Area* PLeftChild; unsigned char Colour; int key; };
struct Info { bool LeftChild; Area* PParent; unsigned char Colour; bool Valid; };
struct Alloc {
    Area* PTreeRoot;
    void RotateRight(Area* node) { Area* c = node->PRightChild; node->PRightChild = c->PLeftChild; if (c->PLeftChild) c->PLeftChild->PParentArea = node;
        c->PParentArea = node->PParentArea; if (!node->PParentArea) PTreeRoot = c; else if (node == node->PParentArea->PLeftChild) node->PParentArea->PLeftChild = c; else node->PParentArea->PRightChild = c;
        c->PLeftChild = node; node->PParentArea = c; }
    void RotateLeft(Area* node) { Area* c = node->PLeftChild; node->PLeftChild = c->PRightChild; if (c->PRightChild) c->PRightChild->PParentArea = node;
        c->PParentArea = node->PParentArea; if (!node->PParentArea) PTreeRoot = c; else if (node == node->PParentArea->PLeftChild) node->PParentArea->PLeftChild = c; else node->PParentArea->PRightChild = c;
        c->PRightChild = node; node->PParentArea = c; }
    static bool IsBlack(const Area* a) { return a == 0 || a->Colour == RB_BLACK; }
    void Fixup(Area* area, const Info* info)
    {
        while ((area != 0 && area != PTreeRoot && area->Colour == RB_BLACK) || (info != 0 && info->Valid && info->PParent != 0 && info->Colour == RB_BLACK)) {
            Area* parent = area ? area->PParentArea : info->PParent;
            if ((area != 0 && area == parent->PLeftChild) || (info != 0 && info->LeftChild)) {
                Area* s = parent->PRightChild;
                if (s->Colour == RB_RED) { s->Colour = RB_BLACK; parent->Colour = RB_RED; RotateRight(parent); s = parent->PRightChild; if (s == 0) { area = PTreeRoot; info = 0; continue; } }
                Area* n = s->PLeftChild;
                if (IsBlack(n) && IsBlack(s->PRightChild)) { s->Colour = RB_RED; area = parent; }
                else { if (IsBlack(s->PRightChild)) { if (n) n->Colour = RB_BLACK; s->Colour = RB_RED; RotateLeft(s); s = parent->PRightChild; }
                       s->Colour = parent->Colour; parent->Colour = RB_BLACK; if (s->PRightChild) s->PRightChild->Colour = RB_BLACK; RotateRight(parent); area = PTreeRoot; }
            } else {
                Area* s = parent->PLeftChild;
                if (s->Colour == RB_RED) { s->Colour = RB_BLACK; parent->Colour = RB_RED; RotateLeft(parent); s = parent->PLeftChild; if (s == 0) { area = PTreeRoot; info = 0; continue; } }
                Area* n = s->PRightChild;
                if (IsBlack(n) && IsBlack(s->PLeftChild)) { s->Colour = RB_RED; area = parent; }
                else { if (IsBlack(s->PLeftChild)) { if (n) n->Colour = RB_BLACK; s->Colour = RB_RED; RotateRight(s); s = parent->PLeftChild; }
                       s->Colour = parent->Colour; parent->Colour = RB_BLACK; if (s->PLeftChild) s->PLeftChild->Colour = RB_BLACK; RotateLeft(parent); area = PTreeRoot; }
            }
            info = 0;
        }
        if (area) area->Colour = RB_BLACK;
    }
};
static int BlackHeight(const Area* a, bool* ok)
{
    if (!a) return 1;
    if (a->Colour == RB_RED && ((a->PLeftChild && a->PLeftChild->Colour == RB_RED) || (a->PRightChild && a->PRightChild->Colour == RB_RED))) *ok = false;
    int l = BlackHeight(a->PLeftChild, ok), r = BlackHeight(a->PRightChild, ok);
    if (l != r) *ok = false;
    return l + (a->Colour == RB_BLACK ? 1 : 0);
}
static void Link(Area* p, Area* l, Area* r) { p->PLeftChild = l; p->PRightChild = r; if (l) l->PParentArea = p; if (r) r->PParentArea = p; }
int main()
{
    // Root B(10) with left B(5) and right B(15) whose children are red 12/20. Delete the black leaf 5 -> null left child of 10.
    Area n10 = {0,0,0,RB_BLACK,10}, n15 = {0,0,0,RB_BLACK,15}, n12 = {0,0,0,RB_RED,12}, n20 = {0,0,0,RB_RED,20};
    Link(&n10, 0, &n15); Link(&n15, &n12, &n20);
    Alloc al; al.PTreeRoot = &n10;
    Info info = { true, &n10, RB_BLACK, true };
    al.Fixup(0, &info);
    bool ok = true; BlackHeight(al.PTreeRoot, &ok);
    if (!ok || al.PTreeRoot->Colour != RB_BLACK || al.PTreeRoot->PParentArea != 0) { std::printf("00a64910_TEST FAIL\n"); return 1; }
    // Mirror case: root B(10), left B(5) with red children 3/7, right leaf removed.
    Area m10 = {0,0,0,RB_BLACK,10}, m5 = {0,0,0,RB_BLACK,5}, m3 = {0,0,0,RB_RED,3}, m7 = {0,0,0,RB_RED,7};
    Link(&m10, &m5, 0); Link(&m5, &m3, &m7);
    Alloc a2; a2.PTreeRoot = &m10;
    Info info2 = { false, &m10, RB_BLACK, true };
    a2.Fixup(0, &info2);
    ok = true; BlackHeight(a2.PTreeRoot, &ok);
    if (!ok || a2.PTreeRoot->Colour != RB_BLACK) { std::printf("00a64910_TEST FAIL\n"); return 1; }
    std::printf("00a64910_TEST PASS\n");
    return 0;
}