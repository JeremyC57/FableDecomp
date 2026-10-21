// CTCWound::~CTCWound() -- when the parent still has its texture-decal component, every remaining wound's four decal entries (wound/scar texture and bump) are deleted from it and the wound list is emptied from the back; then the list and the CTCBase base are destroyed
// Leaf: ??1CTCWound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757680, 217 bytes (+7 int3). __fastcall, `ret`. CTCWound vtable 0x012698B4 stored first.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; CTCTextureDecal::DeleteDecal(long) 0x0070A430;
//          free 0x00BFEA14 (the list node); list<CTCWound::CWound>::~list 0x00758B00 (manifest "CGuiControlTreePane::
//          SortTreeRecursively" is propagated); CTCBase::~CTCBase 0x00686830 (manifest "~CChunkedFileChunk" is propagated).
// Shape: interface 0xE9 (CTCTextureDecal, bitset word 7 bit 9 -> `test ch,2` on +0x3C) fetched through the out-parameter
//        form into edi (esi pushed only on that path); `while (!Wounds.empty())` re-reads the sentinel each pass, the back
//        wound is reached as `Head->Prev->Value` (the value pointer formed after the first field load), each non-zero decal id
//        is deleted and zeroed through one shared zero register (ebx), the node is unlinked (next, prev) and freed. The list
//        destructor and the base destructor run out of line on `this` kept in ebp.
// Layout: CTCWound (PDB, 0x1C with a 12-byte list): ParentThing +0x04, LastFrameChanged +0x0C, Wounds +0x10 (retail STLport
//         list = one sentinel pointer); CWound (0x44): WoundTextureDecalEntryID +0x2c, WoundBumpDecalEntryID +0x30,
//         ScarTextureDecalEntryID +0x34, ScarBumpDecalEntryID +0x38; retail CThing bitset +0x20, interface map +0x44.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (the other TU copy is 0x004365B0)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCTextureDecal {
    void DeleteDecal(long entryID);             // 0x0070A430
};
struct CWound {
    unsigned char _pad_0x00[0x2c];              // Desc (CEntry), Duration
    long WoundTextureDecalEntryID;              // +0x2c
    long WoundBumpDecalEntryID;                 // +0x30
    long ScarTextureDecalEntryID;               // +0x34
    long ScarBumpDecalEntryID;                  // +0x38
    long MorphIndex;                            // +0x3c
    bool Dead;                                  // +0x40
    bool HadUpdate;                             // +0x41
};
struct CWoundList {                             // STLport list<CTCWound::CWound>: the sentinel node pointer only
    struct Node { Node* Next; Node* Prev; CWound Value; };
    Node* Head;
    bool empty() const { return Head->Next == Head; }
    CWound& back() const { return Head->Prev->Value; }
    void pop_back()
    {
        Node* pos = Head->Prev;
        Node* next = pos->Next;
        Node* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
    }
    ~CWoundList();                              // 0x00758B00 (erases the remaining nodes, frees the sentinel)
};
struct CTCBase {                                // 0xc bytes
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
};
struct CTCWound : CTCBase {                     // vtable 0x012698B4
    unsigned long LastFrameChanged;             // +0x0c
    CWoundList    Wounds;                       // +0x10
    virtual ~CTCWound();
};
CTCWound::~CTCWound()
{
    CTCTextureDecal* decal;
    if (ParentThing->GetInterface(&decal, 0xe9)) {
        while (!Wounds.empty()) {
            CWound& wound = Wounds.back();
            if (wound.WoundTextureDecalEntryID) {
                decal->DeleteDecal(wound.WoundTextureDecalEntryID);
                wound.WoundTextureDecalEntryID = 0;
            }
            if (wound.WoundBumpDecalEntryID) {
                decal->DeleteDecal(wound.WoundBumpDecalEntryID);
                wound.WoundBumpDecalEntryID = 0;
            }
            if (wound.ScarTextureDecalEntryID) {
                decal->DeleteDecal(wound.ScarTextureDecalEntryID);
                wound.ScarTextureDecalEntryID = 0;
            }
            if (wound.ScarBumpDecalEntryID) {
                decal->DeleteDecal(wound.ScarBumpDecalEntryID);
                wound.ScarBumpDecalEntryID = 0;
            }
            Wounds.pop_back();
        }
    }
}
// ---- harness ----
#include <cstdio>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static long g_ids[32]; static int g_n = 0;
static const void* g_decalThis = 0;
static int g_baseDtors = 0, g_listDtors = 0;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
void CTCTextureDecal::DeleteDecal(long entryID) { if (g_n < 32) g_ids[g_n++] = entryID; g_decalThis = this; }
CTCBase::~CTCBase() { ++g_baseDtors; }
CWoundList::~CWoundList()
{
    ++g_listDtors;
    Node* n = Head->Next;
    while (n != Head) { Node* nx = n->Next; free(n); n = nx; }
    free(Head);
}
static void InitList(CWoundList& l)
{
    l.Head = (CWoundList::Node*)malloc(sizeof(CWoundList::Node));
    l.Head->Next = l.Head; l.Head->Prev = l.Head;
}
static void PushBack(CWoundList& l, long a, long b, long c, long d)
{
    CWoundList::Node* n = (CWoundList::Node*)malloc(sizeof(CWoundList::Node));
    n->Value.WoundTextureDecalEntryID = a; n->Value.WoundBumpDecalEntryID = b;
    n->Value.ScarTextureDecalEntryID = c; n->Value.ScarBumpDecalEntryID = d;
    n->Next = l.Head; n->Prev = l.Head->Prev;
    l.Head->Prev->Next = n; l.Head->Prev = n;
}
static CTCTextureDecal g_decal[4];
static CInterfaceMap::SEntry g_entries[2];
static CThing g_thing;
static CThing g_bare;
int main()
{
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0xe9; g_entries[1].component = (CTCBase*)&g_decal[1];
    g_thing.m_interfaces.first = g_entries; g_thing.m_interfaces.last = g_entries + 2;
    g_thing.m_presentBits[0xe9 >> 5] = 1UL << (0xe9 & 31);
    CTCWound* w = new CTCWound;
    w->ParentThing = &g_thing;
    InitList(w->Wounds);
    PushBack(w->Wounds, 1, 2, 3, 4);
    PushBack(w->Wounds, 5, 0, 7, 0);
    PushBack(w->Wounds, 0, 10, 0, 12);
    delete w;
    static const long order[] = { 10, 12, 5, 7, 1, 2, 3, 4 };
    CHECK(g_n == 8);
    for (int i = 0; i < 8 && i < g_n; ++i) CHECK(g_ids[i] == order[i]);
    CHECK(g_decalThis == &g_decal[1]);
    CHECK(g_baseDtors == 1 && g_listDtors == 1);
    // no decal component: nothing deleted, the list destructor frees the nodes
    g_n = 0;
    g_bare.m_interfaces.first = g_entries; g_bare.m_interfaces.last = g_entries + 2;
    w = new CTCWound;
    w->ParentThing = &g_bare;
    InitList(w->Wounds);
    PushBack(w->Wounds, 21, 22, 23, 24);
    delete w;
    CHECK(g_n == 0 && g_baseDtors == 2 && g_listDtors == 2);
    if (fails == 0) printf("00757680_TEST PASS\n");
    return fails;
}