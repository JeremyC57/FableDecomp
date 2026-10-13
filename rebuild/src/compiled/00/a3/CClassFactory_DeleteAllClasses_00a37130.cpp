// CClassFactory::DeleteAllClasses()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37130, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+18h] ; cmp [eax],eax ; je R ; L: mov eax,[esi+18h] ; mov ecx,[eax] ; mov ecx,[ecx+8] ; test ecx,ecx ; je N ; mov edx,[ecx] ; push 1 ; call [edx] ; N: mov eax,[esi+18h] ; cmp [eax],eax ; jne L ; R: pop esi ; ret
// engine/CClassFactory.h (PDB): +0x1C std::list<CClass*> ClassList; the vector before it is 4
// bytes smaller in retail, so the list head pointer is at +0x18. Deletes the front class until the
// list is empty: `delete` on a class with a virtual destructor calls vtable slot 0 (the scalar
// deleting destructor) with flag 1, and that destructor unlinks the class from this list.
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.
//
// Lane feedback: the per-iteration reload of the list head and of begin() is the retail-list iterator
// object (`front()` = `*begin()`); raw node pointers CSE the loads. MATCH.
// Retail std::list (4 bytes: the sentinel-node pointer only) with an iterator OBJECT; node =
// {next, prev, value}. begin()/end() are re-read from `this` on every use (no CSE through the
// iterator temporary), which is the retail instruction order for every list walk seen so far.
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p;
        explicit iterator(node* q) : p(q) {}
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

struct CClassCalls {
    virtual ~CClassCalls();     // slot 0: scalar deleting destructor, unlinks from the factory list
};

// Retail CClassFactory view (the vector at +0x04 is 12 bytes, so the list head is at +0x18).
struct CClassFactory_Retail {
    void*                     __vftable;          // +0x00
    unsigned char             ClassTypeInfo[12];  // +0x04 vector<CClassTypeInfo> (retail 12 bytes)
    unsigned long             MaxClassTypeID;     // +0x10 (PDB +0x14)
    unsigned long             NoAllocatedClasses; // +0x14 (PDB +0x18)
    CRetailList<CClassCalls*> ClassList;          // +0x18 (PDB list at +0x1C)
    void DeleteAllClasses();
};

void CClassFactory_Retail::DeleteAllClasses()
{
    // Each CClass destructor removes itself from the list, so deleting the front until the
    // list is empty drains it. `delete` on a null front is the compiler's own null check.
    while (!this->ClassList.empty())
        delete this->ClassList.front();
}