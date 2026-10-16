// CThingPhysical::CThingPhysical(unsigned long type) -- physical thing: the CThing base, an empty health-over-time list, null last/original creator pointers, CanComeBetweenCameraAndObservedThing = -1
// Leaf: ??0CThingPhysical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005296B0, length 106 bytes (+ int3 padding).
// Label correction: the catalog's "CTCDPhysicalObstruction::CTCDPhysicalObstruction" is propagated and wrong: the vtable stored,
// 0x01244E84, is CThingPhysical's (rtti-vtables.tsv), the base call is CThing::CThing(ulong) @ 0x004C9030, and the two pointer
// vtables 0x01244DCC are CIntelligentPointer<CThingPhysical>.
// Callees: CThing::CThing(unsigned long) @ 0x004C9030, malloc @ 0x00BFEA0E (the 0x10-byte list sentinel),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (x2).
// Layout: Ego_r.pdb CThingPhysical with the retail CThing 0x10 bytes shorter and a 4-byte list: HealthOverTimeEffects +0x98,
// Scale +0x9C, PLastCreator +0xA0, POriginalCreator +0xA8, MaxHealth +0xB0, Health +0xB4, CanComeBetweenCameraAndObservedThing
// +0xB8 (the flag byte after it is left unset).
// Shape: the inline list ctor nulls its head before malloc and links the node to itself before storing it.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E

struct CThing {
    virtual void v00();
    unsigned char _body[0x98 - 0x04];
    CThing(unsigned long type);                 // 0x004C9030
};
struct CRetailList_node0x10 {                   // list<CThingPhysical::CHealthOverTimeHelper>
    struct node { node* next; node* prev; unsigned char value[8]; };
    node* head;
    CRetailList_node0x10() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CIntelligentPointerPhysical : CBaseIntelligentPointer {  // vtable 0x01244DCC
    virtual void v01();
    CIntelligentPointerPhysical() {}
};
struct CThingPhysical : CThing {
    CRetailList_node0x10 HealthOverTimeEffects; // +0x98
    float Scale;                                // +0x9c
    CIntelligentPointerPhysical PLastCreator;   // +0xa0
    CIntelligentPointerPhysical POriginalCreator;   // +0xa8
    float MaxHealth;                            // +0xb0
    float Health;                               // +0xb4
    long CanComeBetweenCameraAndObservedThing;  // +0xb8
    unsigned char Flags;                        // +0xbc Damageable, Killable, ...
    virtual void v01();
    CThingPhysical(unsigned long type);
};

CThingPhysical::CThingPhysical(unsigned long type)
    : CThing(type)
    , CanComeBetweenCameraAndObservedThing(-1)
{
}