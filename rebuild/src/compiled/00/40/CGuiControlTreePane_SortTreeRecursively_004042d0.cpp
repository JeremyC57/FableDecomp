// List::Clear() -- free every node of a circular doubly-linked list and leave the sentinel empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x004042D0, 47 bytes. Callees: node free 0x00BFEA14.
//
// This is the STL-style list clear we use everywhere: the list is a single word pointing at a sentinel
// node whose next/prev form a ring with the real nodes. We walk from the sentinel's next, freeing each
// node after reading its successor, until we are back at the sentinel, then point the sentinel at
// itself so the list reads as empty. Nothing in the elements is destroyed here; this instantiation is
// for a list of trivially destructible nodes. The name CGuiControlTreePane::SortTreeRecursively on this
// address is a propagated label and does not describe the body; it sorts nothing.
//
// Matching: `this->head` is re-read on every loop test and after the loop (the free helper may alias
// it), so the head is not cached in a register; `cur`/`n` give the read-successor-then-free order.
struct Node { Node* next; Node* prev; };    // ring links
struct List { Node* head; void Clear(); };  // the sentinel node
// Returns a node to the engine heap.
extern "C" void __cdecl Free2(void* p);   // 0x00bfea14
void List::Clear() {
    Node* n = this->head->next;
    while (n != this->head) {
        // Read the successor before the node goes away.
        Node* cur = n;
        n = n->next;
        Free2(cur);
    }
    // Sentinel points at itself: empty list.
    this->head->next = this->head;
    this->head->prev = this->head;
}
