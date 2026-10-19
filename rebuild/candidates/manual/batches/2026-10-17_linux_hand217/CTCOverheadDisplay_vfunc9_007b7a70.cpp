// CTCOverheadDisplay::vfunc_9() -- advances and removes expired overhead display entries in three lists
// Retail: 0x007b7a70, 240 bytes; each list is a circular doubly-linked sentinel list.
struct COverheadNode { COverheadNode* next; COverheadNode* prev; long frames; };
struct CTCOverheadDisplay { virtual void v00(); unsigned char pad[8]; COverheadNode* first; COverheadNode* second; COverheadNode* third; void vfunc_9(); };
extern float g_OverheadExpiry; // 0x013bae68
extern void ReleaseNode(COverheadNode*); // 0x00bfea14
void CTCOverheadDisplay::vfunc_9()
{
    COverheadNode* n = first->next;
    while (n != first) {
        --n->frames;
        if ((float)n->frames <= -g_OverheadExpiry) {
            COverheadNode* next=n->next; COverheadNode* prev=n->prev;
            prev->next=next; next->prev=prev; ReleaseNode(n); n=next;
        } else n=n->next;
    }
    n = second->next;
    while (n != second) {
        --n->frames;
        if ((float)n->frames <= -g_OverheadExpiry) {
            COverheadNode* next=n->next; COverheadNode* prev=n->prev;
            prev->next=next; next->prev=prev; ReleaseNode(n); n=next;
        } else n=n->next;
    }
    n = third->next;
    while (n != third) {
        --n->frames;
        if ((float)n->frames <= -g_OverheadExpiry) {
            COverheadNode* next=n->next; COverheadNode* prev=n->prev;
            prev->next=next; next->prev=prev; ReleaseNode(n); n=next;
        } else n=n->next;
    }
}
