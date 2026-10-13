// CClassFactory::DeleteAllClasses()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37130, oracle length 43 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
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

// Call-surface view of CClass (engine/CClass.h carries the layout only).
struct CClassCalls {
    virtual ~CClassCalls();     // slot 0: scalar deleting destructor, unlinks from the factory list
};

struct CClassListNode {
    CClassListNode* Next;       // +0x00
    CClassListNode* Prev;       // +0x04
    CClassCalls*    Value;      // +0x08
};

struct CClassFactory_Retail {
    void*           __vftable;         // +0x00
    unsigned char   ClassTypeInfo[12]; // +0x04 vector<CClassTypeInfo> (retail 12 bytes)
    unsigned long   MaxClassTypeID;    // +0x10 (PDB +0x14)
    unsigned long   NoAllocatedClasses;// +0x14 (PDB +0x18)
    CClassListNode* ClassList_Head;    // +0x18 (PDB list at +0x1C)
    void DeleteAllClasses();
};

void CClassFactory_Retail::DeleteAllClasses()
{
    while (this->ClassList_Head->Next != this->ClassList_Head) {
        CClassCalls* instance = this->ClassList_Head->Next->Value;
        if (instance)
            delete instance;
    }
}
