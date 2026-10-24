// Behaviour test for 0x00942B30 GetDummyObjectRecursive: the thing's own appearance first, then every live owned thing,
// depth first; dead or empty pointers are skipped; false when nothing has the dummy.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* text; };
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: int axis; };

class CTCGraphicAppearance {
public:
    const char* dummy;                          // the one dummy object this appearance carries
    float x;
    bool GetDummyObject(const CCharString& name, long, unsigned long, C3DVector& pos, CRightHandedSet& set, float) const
    {
        if (std::strcmp(name.text, dummy) != 0) return false;
        pos.X = x;
        set.axis = 1;
        return true;
    }
};

class CThing;
class CIntelligentPointerThing { public: CThing* item; CThing* GetPItem() const { return item; } };

struct Node { Node* next; Node* prev; CIntelligentPointerThing data; };

class CTCThingOwner {
public:
    Node sentinel;                              // STLport list header
    Node nodes[4];
    int count;
    void Init() { sentinel.next = sentinel.prev = &sentinel; count = 0; }
    void Add(CThing* t)
    {
        Node* n = &nodes[count++];
        n->data.item = t;
        n->next = &sentinel;
        n->prev = sentinel.prev;
        sentinel.prev->next = n;
        sentinel.prev = n;
    }
};

class CThing {
public:
    CTCGraphicAppearance* PTCGraphicAppearance;
    CTCThingOwner* owner;                       // stands for the interface-0x21 lookup
    bool Dead;
    CThing() : PTCGraphicAppearance(0), owner(0), Dead(false) { }
};

inline bool IS_THING_ALIVE(const CIntelligentPointerThing& p) { return p.GetPItem() != 0 && !p.GetPItem()->Dead; }

bool GetDummyObjectRecursive(const CThing* thing, const CCharString& name, long index, unsigned long flags,
                             C3DVector& pos, CRightHandedSet& set)
{
    if (thing->PTCGraphicAppearance && thing->PTCGraphicAppearance->GetDummyObject(name, index, flags, pos, set, 1.0f))
        return true;
    const CTCThingOwner* owner = thing->owner;
    if (owner) {
        for (const Node* it = owner->sentinel.next; it != &owner->sentinel; it = it->next) {
            if (IS_THING_ALIVE(it->data) && GetDummyObjectRecursive(it->data.GetPItem(), name, index, flags, pos, set))
                return true;
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CTCGraphicAppearance rootApp = { "hand", 1.0f };
    CTCGraphicAppearance swordApp = { "tip", 2.0f };
    CTCGraphicAppearance gemApp = { "gem", 3.0f };
    CThing root, sword, gem, ghost;
    CTCThingOwner rootOwner, swordOwner;
    rootOwner.Init(); swordOwner.Init();
    root.PTCGraphicAppearance = &rootApp;
    root.owner = &rootOwner;
    sword.PTCGraphicAppearance = &swordApp;
    sword.owner = &swordOwner;
    gem.PTCGraphicAppearance = &gemApp;
    ghost.PTCGraphicAppearance = &gemApp;
    ghost.Dead = true;
    rootOwner.Add(0);
    rootOwner.Add(&sword);
    swordOwner.Add(&gem);
    C3DVector pos = { 0, 0, 0 };
    CRightHandedSet set = { 0 };
    CCharString hand = { "hand" }, tip = { "tip" }, gemName = { "gem" }, none = { "none" };
    if (!GetDummyObjectRecursive(&root, hand, 0, 0, pos, set) || pos.X != 1.0f) ok = 0;
    if (!GetDummyObjectRecursive(&root, tip, 0, 0, pos, set) || pos.X != 2.0f) ok = 0;
    if (!GetDummyObjectRecursive(&root, gemName, 0, 0, pos, set) || pos.X != 3.0f) ok = 0;   // two levels down
    if (GetDummyObjectRecursive(&root, none, 0, 0, pos, set)) ok = 0;
    gem.Dead = true;
    if (GetDummyObjectRecursive(&root, gemName, 0, 0, pos, set)) ok = 0;                      // dead things are skipped
    rootOwner.Add(&ghost);
    if (GetDummyObjectRecursive(&root, gemName, 0, 0, pos, set)) ok = 0;
    if (!ok) { std::printf("00942b30_TEST FAIL\n"); return 1; }
    std::printf("00942b30_TEST PASS\n");
    return 0;
}