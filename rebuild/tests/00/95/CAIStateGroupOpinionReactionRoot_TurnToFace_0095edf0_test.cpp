#include <cstdio>
class CThing { public: unsigned char flags91; unsigned long bits[9]; long compType; void* comp; CThing() : flags91(0), compType(-1), comp(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; } };
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
struct CPtrCopy : CBaseIntelligentPointer { CPtrCopy(const CBaseIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
struct CGroup { CBaseIntelligentPointer ThingReactingTo; long result; CThing* lookedAt; long flags; float speed; bool terminated; CGroup() : result(0), lookedAt(0), flags(0), speed(0), terminated(false) { }
    long AIAction_LookAtThing(long repeat, CPtrCopy thing, long f, float s) { lookedAt = thing.GetPItem(); flags = f; speed = s; return repeat == 1 ? result : -1; }
    void Terminate() { terminated = true; }
    bool TurnToFace() { if (AIAction_LookAtThing(1, ThingReactingTo, 0x21, 0.0027777f) != 2) { Terminate(); return false; } return true; }
};
int main()
{
    CThing t; CGroup g; g.ThingReactingTo.SetPItem(&t); g.result = 2;
    bool ok = g.TurnToFace() && g.lookedAt == &t && g.flags == 0x21 && g.speed > 0.00277f && g.speed < 0.00278f && !g.terminated;
    g.result = 4; ok = ok && !g.TurnToFace() && g.terminated;
    if (!ok) { std::printf("0095edf0_TEST FAIL\n"); return 1; }
    std::printf("0095edf0_TEST PASS\n"); return 0;
}