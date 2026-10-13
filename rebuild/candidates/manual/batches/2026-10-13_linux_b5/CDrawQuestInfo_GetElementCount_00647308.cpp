// CDrawQuestInfo::GetElementCount() -- walks the element list (+0x40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00647308, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov edx,[ecx+40h] ; mov ecx,[edx] ; xor eax,eax ; jmp T ; L: mov ecx,[ecx] ; inc eax ; T: cmp ecx,edx ; jne L ; ret
// Counts the nodes of the std::list at +0x40 (retail list: sentinel pointer only). The manifest leaf
// `GetElementByIndex` takes an index; this body takes none and returns the walk length, so it is
// named for what it does. With no call inside the loop the sentinel is kept in edx.
struct CQuestInfoElement;
// Retail std::list (4 bytes: sentinel pointer) with an iterator OBJECT; begin()/end() are
// re-read from `this` on every use (see docs/pipeline/GOTCHAS.md).
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p; explicit iterator(node* q) : p(q) {}
        T& operator*() const { return p->value; }
        iterator& operator++() { p = p->next; return *this; }
        bool operator!=(const iterator& o) const { return p != o.p; }
        bool operator==(const iterator& o) const { return p == o.p; }
    };
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    bool empty() const { return head->next == head; }
    T& front() { return *begin(); }
};

struct CDrawQuestInfo_Retail {
    unsigned char                   _pad_0x00[0x40];
    CRetailList<CQuestInfoElement*> Elements;   // +0x40
    long GetElementCount();
};

long CDrawQuestInfo_Retail::GetElementCount()
{
    long count = 0;
    for (CRetailList<CQuestInfoElement*>::iterator it = this->Elements.begin(); it != this->Elements.end(); ++it)
        ++count;
    return count;
}
