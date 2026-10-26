// CTCUserActionPointBase::IsActionPointFree() -- the user list at +0x108 is non-empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661F20, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+108h] ; mov edx,[eax] ; xor ecx,ecx ; cmp edx,eax ; setne cl ; mov al,cl ; ret
// True when the std::list at +0x108 has at least one node (sentinel->next != sentinel). The
// register-width `setne cl ; mov al,cl` is the plain `return a != b;` bool form.
class CThing;
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

struct CTCUserActionPointBase_Retail {
    unsigned char        _pad_0x00[0x108];
    CRetailList<CThing*> Users;             // +0x108 (name from use)
    bool IsActionPointFree();
};

bool CTCUserActionPointBase_Retail::IsActionPointFree()
{
    return !this->Users.empty();
}