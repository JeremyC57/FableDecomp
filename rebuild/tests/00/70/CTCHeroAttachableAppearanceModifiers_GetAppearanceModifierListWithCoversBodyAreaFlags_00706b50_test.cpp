// Behaviour test for 0x00706B50 CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierListWithCoversBodyAreaFlags:
// walks AppearanceModifiers[type] in order and returns (as a list) the def indices whose thing-object def resolves, has
// an appearance-modifier sub-def, and whose CoversBodyAreaFlags intersect flags
#include <cstdio>

namespace model {

struct CAppearanceModifierDef { int CoversBodyAreaFlags; };
struct CThingObjectDef { const CAppearanceModifierDef* Modifier; };
struct CModifierEntry { int DefIndex; int Extra; };

const int NDEFS = 8;
CAppearanceModifierDef g_modifiers[NDEFS];
CThingObjectDef g_things[NDEFS];
bool g_exists[NDEFS];

bool GetDef(int index, const CThingObjectDef** out)
{
    if (index < 0 || index >= NDEFS || !g_exists[index]) return false;
    *out = &g_things[index];
    return true;
}
bool GetSubDef(const CThingObjectDef* def, const CAppearanceModifierDef** out)
{
    if (!def->Modifier) return false;
    *out = def->Modifier;
    return true;
}

// The modifier lists and the expected result live at namespace scope: under the lane's VC7.1 (/Od) a brace-initialised local array after a
// `for` loop makes cl.exe spin.
CModifierEntry hats[6] = { { 0, 0 }, { 4, 0 }, { 5, 0 }, { 3, 0 }, { 1, 0 }, { 7, 0 } };
CModifierEntry none[1] = { { 2, 0 } };
const int wantBit0[2] = { 0, 4 };

struct ListNode { ListNode* Next; ListNode* Prev; int Data; };
struct List {                                         // std::list<long>: a circular list around a header node
    ListNode Header;
    unsigned int Count;
    List() : Count(0) { Header.Next = Header.Prev = &Header; }
    void push_back(int v)
    {
        ListNode* n = new ListNode;
        n->Data = v;
        n->Next = &Header;
        n->Prev = Header.Prev;
        Header.Prev->Next = n;
        Header.Prev = n;
        ++Count;
    }
    ~List() { for (ListNode* n = Header.Next; n != &Header; ) { ListNode* x = n->Next; delete n; n = x; } }
};

struct Hero {
    const CModifierEntry* Types[3];
    int Sizes[3];
    void GetList(int flags, int type, List& result) const
    {
        const CModifierEntry* modifiers = Types[type];
        for (const CModifierEntry* it = modifiers; it != modifiers + Sizes[type]; ++it) {
            const CThingObjectDef* def = 0;
            const CAppearanceModifierDef* modifierDef = 0;
            if (GetDef(it->DefIndex, &def) && GetSubDef(def, &modifierDef) && (modifierDef->CoversBodyAreaFlags & flags))
                result.push_back(it->DefIndex);
        }
    }
};

int run()
{
    for (int i = 0; i < NDEFS; ++i) {
        g_exists[i] = i != 5;                             // def 5 does not resolve
        g_modifiers[i].CoversBodyAreaFlags = 1 << (i % 4);
        g_things[i].Modifier = i == 3 ? 0 : &g_modifiers[i];   // def 3 has no modifier sub-def
    }
    Hero h;
    h.Types[0] = none; h.Sizes[0] = 0;
    h.Types[1] = hats; h.Sizes[1] = 6;
    h.Types[2] = none; h.Sizes[2] = 1;
    int bad = 0;
    {
        List r;
        h.GetList(1, 1, r);                               // flag bit 0: defs 0 and 4 (5 unresolved, 3 has no sub-def)
        ListNode* n = r.Header.Next;
        for (int i = 0; i < 2; ++i, n = n->Next) if (n == &r.Header || n->Data != wantBit0[i]) bad = 1;
        if (r.Count != 2 || n != &r.Header) bad = 2;
    }
    {
        List r;
        h.GetList(0x8 | 0x2, 1, r);                       // bits 1 and 3: defs 1 and 7, in vector order
        if (r.Count != 2 || r.Header.Next->Data != 1 || r.Header.Prev->Data != 7) bad = 3;
    }
    {
        List r, e;
        h.GetList(0xF, 0, e);                             // empty type
        h.GetList(0x1, 2, r);                             // def 2 covers bit 2 only
        if (e.Count != 0 || r.Count != 0 || e.Header.Next != &e.Header) bad = 4;
    }
    return bad;
}

}

// (the checks run inside namespace model: a `using namespace model;` in main followed by a for loop and a later
// declaration makes the lane's VC7.1 cl.exe spin)
int main()
{
    if (model::run()) { std::printf("00706b50_TEST FAIL\n"); return 1; }
    std::printf("00706b50_TEST PASS\n");
    return 0;
}