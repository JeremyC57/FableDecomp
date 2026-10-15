// CConversation::AddPersonToConversation(CThing* person) -- append an intelligent pointer to the person unless the conversation already holds one
// Grade: DIFFER(94v100) -- lane gap (GOTCHAS wave 24l, std::find tag over a live parameter slot): retail enregisters `person` in ebx, stores it back to its own argument slot for the const-ref value and homes the empty iterator-tag temporary in that same slot (`lea eax,[esp+18h]` twice); the lane binds the value to the slot directly (no ebx, `sub esp,0Ch`) and gives the tag its own frame byte (`lea eax,[esp+0Bh]`).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E5790, 100 bytes (112 with padding).
// Callees: SGI/STLport __find(first, last, const CThing*&, const random_access_iterator_tag&) 0x006E66E0
//          (/Gr __fastcall, unrolled by four over 8-byte elements), CBaseIntelligentPointer::CBaseIntelligentPointer
//          0x00A01B10, ::SetPItem 0x00A01B90, ::~CBaseIntelligentPointer 0x00A01C10, the people vector's
//          push_back(const CIntelligentPointer<CThing>&) 0x004AD520. CIntelligentPointer<CThing> vtable 0x01238C6C.
//
// Shape: the people are a retail vector<CIntelligentPointer<CThing>> at +8 (first +8, last +0xc); end()
// is read once into a local before the find and compared with its result. The value and the empty
// iterator tag are both bound to const references: the tag temporary is homed in the dead `person`
// argument slot, and the person itself (enregistered in ebx) is stored back to that same slot for
// the value reference. The pushed element is a temporary CIntelligentPointer built as in the landed
// CThingGameObject::OnCreate (base constructor, derived vtable, SetPItem), destroyed through the base.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

CIntelligentPointerThing* __fastcall __find(CIntelligentPointerThing* first, CIntelligentPointerThing* last,
                                            CThing* const& value, const random_access_iterator_tag& tag);   // 0x006E66E0

struct CPeopleVector {                              // retail vector<CIntelligentPointer<CThing>>
    CIntelligentPointerThing* first;
    CIntelligentPointerThing* last;
    CIntelligentPointerThing* end_of_storage;
    CIntelligentPointerThing* begin() { return first; }
    CIntelligentPointerThing* end() { return last; }
    void push_back(const CIntelligentPointerThing& value);   // 0x004AD520
};

struct CConversation {
    unsigned char _pad[8];
    CPeopleVector m_People;                         // +0x08
    void AddPersonToConversation(CThing* person);
};

void CConversation::AddPersonToConversation(CThing* person)
{
    CIntelligentPointerThing* end = m_People.end();
    if (__find(m_People.begin(), end, person, random_access_iterator_tag()) == end)
        m_People.push_back(CIntelligentPointerThing(person));
}
