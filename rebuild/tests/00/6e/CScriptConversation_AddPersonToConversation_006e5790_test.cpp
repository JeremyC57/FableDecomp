// Behaviour test for 006e5790: src/006e5790.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CScriptConversation::AddPersonToConversation(CThing& person)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CScriptConversation::AddPersonToConversation(CThing& person) -- append an intelligent pointer to the person unless the conversation already holds one
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E5790, 100 bytes (112 with padding).
// Leaf: AddPersonToConversation
// Label correction: the catalog "CConversation::AddPersonToConversation" (forge-autoname) has the wrong class and parameter.
// Ego_r.pdb names `?AddPersonToConversation@CScriptConversation@@QAEXAAVCThing@@@Z` (the person by reference), and the
// PDB CScriptConversation keeps PeopleInConversation (vector<CIntelligentPointer<CThing>>) at +8, as retail does.
// Layout: the shared engine/CScriptConversation.h gives PeopleInConversation the PDB's 16-byte Dinkumware vector. Retail's
// is the 12-byte STLport one: its push_back 0x004AD520 tests [this+4] (finish) against [this+8] (end of storage), and
// this function passes `lea edi,[ecx+8]` as that `this`. So the class is modelled locally up to the vector.
// Callees: SGI/STLport __find(first, last, CThing* const&, const random_access_iterator_tag&) 0x006E66E0
//          (/Gr __fastcall, unrolled by four over 8-byte elements), CBaseIntelligentPointer::CBaseIntelligentPointer
//          0x00A01B10, ::SetPItem 0x00A01B90, ::~CBaseIntelligentPointer 0x00A01C10, the people vector's
//          push_back(const CIntelligentPointer<CThing>&) 0x004AD520. CIntelligentPointer<CThing> vtable 0x01238C6C.
//
// Shape: end() is read once into a local before the find and compared with its result. `&person` is an rvalue, so
// binding it to find's `CThing* const&` makes a temporary. VC7.1 homes that temporary in the reference argument's own
// slot: it stores ebx back to [esp+20h], and the empty iterator tag shares the slot. This needs __find and push_back as
// same-TU noinline stand-ins (both take addresses by const reference) that forward to the retail addresses. Without
// them the tag gets its own frame word. The pushed element is a temporary CIntelligentPointer built as in the landed
// CThingGameObject::OnCreate (base constructor, derived vtable, SetPItem), destroyed through the base.
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

CIntelligentPointerThing* Find_006e66e0(CIntelligentPointerThing* first, CIntelligentPointerThing* last, CThing* const* value);
#pragma auto_inline(off)
__declspec(noinline) CIntelligentPointerThing* __fastcall __find(CIntelligentPointerThing* first, CIntelligentPointerThing* last,
                                            CThing* const& value, const random_access_iterator_tag&)   // 0x006E66E0 stand-in
{
    return Find_006e66e0(first, last, &value);
}
#pragma auto_inline(on)

struct CPeopleVector {                              // retail vector<CIntelligentPointer<CThing>>
    CIntelligentPointerThing* first;
    CIntelligentPointerThing* last;
    CIntelligentPointerThing* end_of_storage;
    CIntelligentPointerThing* begin() { return first; }
    CIntelligentPointerThing* end() { return last; }
    void push_back(const CIntelligentPointerThing& value);
};

void PushBack_004ad520(CPeopleVector* v, const CIntelligentPointerThing* value);
#pragma auto_inline(off)
__declspec(noinline) void CPeopleVector::push_back(const CIntelligentPointerThing& value)   // 0x004AD520 stand-in
{
    PushBack_004ad520(this, &value);
}
#pragma auto_inline(on)

class CScriptConversationManager;

class CScriptConversation { public:
    CScriptConversationManager* Manager;            // +0x00
    long                        ConversationIndex;  // +0x04
    CPeopleVector               PeopleInConversation;   // +0x08 (retail STLport, 12 bytes)
    void AddPersonToConversation(CThing& person);
};

void CScriptConversation::AddPersonToConversation(CThing& person)
{
    CIntelligentPointerThing* end = PeopleInConversation.end();
    if (__find(PeopleInConversation.begin(), end, &person, random_access_iterator_tag()) == end)
        PeopleInConversation.push_back(CIntelligentPointerThing(&person));
}

// ---- stubs and driver ----
class CThing { public: int id; };
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_finds = 0, g_pushes = 0;
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { ++g_ctors; PItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { ++g_sets; PItem = item; }
static CThing* g_lastValue = 0;
CIntelligentPointerThing* Find_006e66e0(CIntelligentPointerThing* first, CIntelligentPointerThing* last, CThing* const* value)
{
    ++g_finds; g_lastValue = *value;
    for (; first != last; ++first) if (first->PItem == *value) break;
    return first;
}
static CThing* g_pushed[8];
void PushBack_004ad520(CPeopleVector* v, const CIntelligentPointerThing* value)
{
    g_pushed[g_pushes++] = value->PItem;
    // model the retail append (the element copy is a raw 8-byte store here)
    *(void**)v->last = *(void* const*)value; v->last->PItem = value->PItem; ++v->last;
}
int main()
{
    CThing a = { 1 }, b = { 2 };
    static unsigned char raw[4 * sizeof(CIntelligentPointerThing)];
    CIntelligentPointerThing* store = (CIntelligentPointerThing*)raw;
    CScriptConversation c;
    c.PeopleInConversation.first = store; c.PeopleInConversation.last = store; c.PeopleInConversation.end_of_storage = store + 4;
    c.AddPersonToConversation(a);
    CHECK(g_finds == 1 && g_lastValue == &a && g_pushes == 1 && g_pushed[0] == &a);
    CHECK(g_ctors == 1 && g_sets == 1 && g_dtors == 1 && c.PeopleInConversation.last == store + 1 && store[0].PItem == &a);
    c.AddPersonToConversation(a);                        // already present: no push
    CHECK(g_finds == 2 && g_pushes == 1 && g_ctors == 1 && g_dtors == 1);
    c.AddPersonToConversation(b);
    CHECK(g_finds == 3 && g_lastValue == &b && g_pushes == 2 && g_pushed[1] == &b && store[1].PItem == &b && g_dtors == 2);
    if (fails == 0) printf("006e5790_TEST PASS\n");
    return fails;
}
