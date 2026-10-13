#pragma optimize("s",on)
// CTCVillage::IsThereACratePutDownTask()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658E09, oracle length 47 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov eax,[edi+60h] ; mov esi,[eax] ; cmp esi,eax ; je F ; L: mov ecx,[esi+8] ; cmp byte ptr [ecx+18h],0 ; jne N ; mov eax,[ecx] ; call [eax+10h] ; test al,al ; jne T ; N: mov esi,[esi] ; cmp esi,[edi+60h] ; jne L ; F: xor al,al ; pop edi ; pop esi ; ret ; T: mov al,1 ; jmp <pop/ret>
// Walks CrateTasks (PDB +0xD0; retail +0x60, see the layout note) for a task that nobody has
// taken yet and that is a put-down task (virtual slot 4 of the task).
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.
//
// Lane feedback: retail-list iterator object (end() re-read each iteration, `cmp esi,[edi+60h]`) under
// #pragma optimize("s",on) (shared epilogue); the lander sweep applies the pragma. MATCH["s"].
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

struct CVillageTaskCarryCrateCalls {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual bool IsPutDownTask();          // slot 4 (+0x10): a put-crate-down (as opposed to pick-up) task
    unsigned char _pad_0x04[0x14];
    bool          Taken;                   // +0x18 (name inferred: already accepted by a villager)
};

struct CCountedPointer_CrateTask {
    CVillageTaskCarryCrateCalls* Data;     // +0x00
    void*                        Info;     // +0x04
};

// Retail CTCVillage view: the crate-task list (4 bytes) sits at +0x60 (PDB +0xD0).
struct CTCVillage_Retail {
    unsigned char                          _pad_0x00[0x60];
    CRetailList<CCountedPointer_CrateTask> CrateTasks;    // +0x60
    bool IsThereACratePutDownTask();
};

bool CTCVillage_Retail::IsThereACratePutDownTask()
{
    // Any crate task nobody has taken yet that is a put-down task? end() is re-read from
    // `this` every iteration (the virtual call could change the list).
    for (CRetailList<CCountedPointer_CrateTask>::iterator it = this->CrateTasks.begin();
         it != this->CrateTasks.end(); ++it) {
        CVillageTaskCarryCrateCalls* task = (*it).Data;
        if (!task->Taken && task->IsPutDownTask())
            return true;
    }
    return false;
}