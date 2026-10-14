#include <cstdio>
struct CThing { unsigned char flags91; unsigned long bits[9]; long compType; void* comp; CThing() : flags91(0), compType(-1), comp(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; } };
struct CBaseIntelligentPointer {
    CThing* item; int sets;
    CBaseIntelligentPointer() : item(0), sets(0) { }
    CThing* GetPItem() const { return item; }
    void SetPItem(CThing* p) { item = p; ++sets; }
};
// the inlined GetInterface: presence bit, then the single-entry map
template<class T> bool GetInterface(CThing* t, T** out, long type)
{
    if (t->bits[type >> 5] & (1UL << (type & 31))) { *out = (t->compType == type) ? (T*)t->comp : 0; return true; }
    return false;
}
static void GiveTC(CThing* t, long type, void* comp) { t->bits[type >> 5] |= 1UL << (type & 31); t->compType = type; t->comp = comp; }
struct CTCNymph { bool active; CBaseIntelligentPointer target; };
struct CGroup { CThing* creature; CThing* asked; CGroup(CThing* c) : creature(c), asked(0) { }
    bool IsTargetInLineOfSight(CThing* t) { asked = t; return t != 0 && t->flags91 == 7; }
    bool IsRunnable() { CTCNymph* p; if (GetInterface(creature, &p, 0xF5)) { if (p->active) return IsTargetInLineOfSight(p->target.GetPItem()); } return false; } };
int main()
{
    CThing c, t; t.flags91 = 7; CTCNymph n; n.active = false; n.target.SetPItem(&t); CGroup g(&c);
    bool ok = !g.IsRunnable() && g.asked == 0;
    GiveTC(&c, 0xF5, &n); ok = ok && !g.IsRunnable() && g.asked == 0;
    n.active = true; ok = ok && g.IsRunnable() && g.asked == &t;
    t.flags91 = 0; ok = ok && !g.IsRunnable();
    if (!ok) { std::printf("0091c640_TEST FAIL\n"); return 1; }
    std::printf("0091c640_TEST PASS\n"); return 0;
}
