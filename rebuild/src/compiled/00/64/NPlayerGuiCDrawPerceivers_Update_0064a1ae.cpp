#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawPerceivers::Update() -- rebuild the perceiver summary: for each perceiving thing still alive, its perception component raises State to 2 (hostile perception) or 1 (aware) and counts it; each counted thing makes the allegiance 2 (an enemy of the hero) or else 1 (an ally); dead things are erased from the list
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A1AE, length 301 bytes (frame pointer).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetControlledCreature @ 0x00487DD0, IS_THING_ALIVE @ 0x0043DBF0 (the
// hero) and @ 0x00436380 (each thing) -- `__fastcall`, by reference, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, std::bitset::test
// @ 0x00410DE0, the interface map LowerBound const @ 0x004365B0, CTCPerceiveThing::IsAwareOfThing @ 0x00643B3B, AreThingsEnemies /
// AreThingsAllies @ 0x0076D2F0 / 0x0076D4F0 (`__fastcall` (hero, thing)), list::erase(iterator) @ 0x006550E2 (by-value iterator,
// hidden result).
// Layout: as the landed GetPerceivedThings 0x0064D3A5 (Things list +0x3c, PDB names State +0x28, Allegiance +0x2c, ThingCount +0x38;
// PlayerManager +8); the perception component's two flags +0x2d/+0x2e (descriptive).
// Shape: frame-pointer size TU; the hero and each thing are named locals bound by reference to IS_THING_ALIVE (kept in the frame and
// re-read for the relation tests); the count before the interface test is kept in a local; the component lookup is one ternary.
struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) const { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUncheckedConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing { };
struct CThingPlayerCreature : CThingCreatureBase { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0 / 0x00436380
void* FindRef_free_IS_THING_ALIVE_2();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    void* p = FindRef_free_IS_THING_ALIVE_2();
    if (!p)
        return false;
    *(void**)&thing = p;
    return false;
}
template <class T> bool __fastcall IS_THING_ALIVE_00436380(const T& thing)   // the 0x00436380 copy (same stand-in body)
{
    void* p = FindRef_free_IS_THING_ALIVE_2();
    if (!p)
        return false;
    *(void**)&thing = p;
    return false;
}
#pragma auto_inline(on)
struct CPlayer {
    CThingPlayerCreature* GetControlledCreature(); // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b);   // 0x0076D2F0
void* FindRef_free_AreThingsEnemies_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b)
{
    void* p = FindRef_free_AreThingsEnemies_1();
    if (!p)
        return false;
    *(void**)a = p;
    *(void**)b = p;
    return false;
}
#pragma auto_inline(on)
bool __fastcall AreThingsAllies(const CThing* a, const CThing* b);    // 0x0076D4F0
void* FindRef_free_AreThingsAllies_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall AreThingsAllies(const CThing* a, const CThing* b)
{
    void* p = FindRef_free_AreThingsAllies_0();
    if (!p)
        return false;
    *(void**)a = p;
    *(void**)b = p;
    return false;
}
#pragma auto_inline(on)

struct CTCPerceiveThing {                          // the thing's interface 0x34
    unsigned char _pad[0x2d];
    bool          PerceivesHostile;                // +0x2d
    bool          PerceivesSomething;              // +0x2e
    bool IsAwareOfThing();                         // 0x00643B3B
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    const CThing* GetPItem();                      // 0x00A01B50
};

struct CRetailThingPointerList {                   // VC6 Dinkumware list<CIntelligentPointer<const CThing>>: one sentinel pointer
    struct node { node* next; node* prev; CBaseIntelligentPointer value; };
    struct iterator {
        node* m_pNode;
        iterator() { }
        explicit iterator(node* n) : m_pNode(n) { }
        iterator(const iterator& o) : m_pNode(o.m_pNode) { }
        CBaseIntelligentPointer& operator*() const { return m_pNode->value; }
        iterator& operator++() { m_pNode = m_pNode->next; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    node* head;
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    iterator erase(iterator where);                // 0x006550E2
};

namespace NPlayerGui {

struct CDrawPerceivers {
    void*                   _vtable;               // +0x00
    void*                   PlayerGui;             // +0x04
    CPlayerManager*         PlayerManager;         // +0x08
    unsigned char           _pad0c[0x28 - 0x0c];
    long                    State;                 // +0x28
    long                    Allegiance;            // +0x2c
    unsigned char           _pad30[8];             // FrameInterp, ThrobScale
    long                    ThingCount;            // +0x38
    CRetailThingPointerList Things;                // +0x3c
    void Update();
};

void CDrawPerceivers::Update()
{
    State = 0;
    Allegiance = 0;
    ThingCount = 0;
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    for (CRetailThingPointerList::iterator it = Things.begin(); it != Things.end(); ) {
        const CThing* thing = (*it).GetPItem();
        if (IS_THING_ALIVE_00436380(thing)) {
            long countBefore = ThingCount;
            if (thing->HasInterface(0x34)) {
                CTCPerceiveThing* perceive = thing->GetInterfaceUncheckedConst<CTCPerceiveThing>(0x34);
                long level;
                if (perceive->PerceivesHostile)
                    level = 2;
                else if (perceive->IsAwareOfThing() || perceive->PerceivesSomething)
                    level = 1;
                else
                    goto counted;
                if (State < level)
                    State = level;
                ++ThingCount;
            }
        counted:
            if (ThingCount > countBefore) {
                if (Allegiance != 2 && AreThingsEnemies(hero, thing))
                    Allegiance = 2;
                if (Allegiance == 0 && AreThingsAllies(hero, thing))
                    Allegiance = 1;
            }
            ++it;
        } else
            it = Things.erase(it);
    }
}

}