// CTCSolidToThing::AddThingNotSolidTo(const CThing* thing) -- append the thing to the not-solid-to list unless it is already there
// Leaf: AddThingNotSolidTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770FA0, length 78 bytes (28 callers).
// Callees: CTCSolidToThing::IsSolidToThing(const CThing*) @ 0x00770DE0 (true when the list is empty or does not hold the
// thing), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90,
// vector<CIntelligentPointer<CThing const> >::push_back(const T&) @ 0x007398B0, ~CBaseIntelligentPointer @ 0x00A01C10.
// Vtable 0x01237F1C = CIntelligentPointer (rtti-vtables.tsv): the inlined derived constructor's store.
// Layout: Ego_r.pdb CTCSolidToThing (0x1c bytes): NotSolidToThings (vector<CIntelligentPointer<CThing const> >) +0x0c.
// Shape: an 8-byte stack CIntelligentPointer built by its inline converting constructor (base constructor, own
// vtable, SetPItem), pushed by reference and destroyed through the base destructor (the compiler-generated derived
// destructor drops the vtable reset).
struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)thing); }
};

struct CThingPointerVector {                                // retail std::vector: first, last, end
    CIntelligentPointer* m_pFirst;
    CIntelligentPointer* m_pLast;
    CIntelligentPointer* m_pEnd;
    void push_back(const CIntelligentPointer& value);       // 0x007398B0
};

struct CTCSolidToThing {
    void*               __vftable;                          // +0x00
    CThing*             ParentThing;                        // +0x04
    bool                ToBeRemoved;                        // +0x08
    bool                IsInGlobalUpdateTCs;                // +0x09
    CThingPointerVector NotSolidToThings;                   // +0x0c
    bool IsSolidToThing(const CThing* thing) const;         // 0x00770DE0
    void AddThingNotSolidTo(const CThing* thing);
};

void CTCSolidToThing::AddThingNotSolidTo(const CThing* thing)
{
    if (IsSolidToThing(thing)) {
        CIntelligentPointer pointer(thing);
        NotSolidToThings.push_back(pointer);
    }
}