// Behaviour test for 006a8710 (model copy: PeekTC over a sorted interface map).
#include <cstdio>
#include <cstring>
class CTCBase { public: int tag; };
enum ETCInterfaceType { TC_DISCOVERY_K = 0x40 };
struct CInterfaceMap {
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const ETCInterfaceType& type) const
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
    SEntry* Find(long type) const
    {
        SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->type) ? last : it;
    }
};
class CThing {
public:
    unsigned int UsingComponents[9];
    CInterfaceMap Components;
    unsigned char Dead : 1;
    CInterfaceMap::SEntry entries[4];
    int n;
    CThing() : n(0) { std::memset(UsingComponents, 0, sizeof(UsingComponents)); Dead = 0; Components.first = Components.last = Components.end_of_storage = entries; }
    void Add(long type, CTCBase* tc)    // keep sorted by type
    {
        int i = n;
        while (i > 0 && entries[i - 1].type > type) { entries[i] = entries[i - 1]; --i; }
        entries[i].type = (ETCInterfaceType)type; entries[i].component = tc; ++n;
        Components.last = entries + n;
        UsingComponents[type >> 5] |= 1u << (type & 31);
    }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1u << (type & 31)))
            return true;
        return false;
    }
    template<class T> bool PeekTC(T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (T*)Components.Find(T::StatGetInterfaceType())->component;
            return true;
        }
        return false;
    }
};
inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->Dead)
        return true;
    return false;
}
class C3DVector { public: float X, Y, Z; };
class CTCCreatureNavigation : public CTCBase { public: static long StatGetInterfaceType() { return 7; } };
class CPosThing : public CThing { public: C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CThingFilter_IsNavLineOfSightToThing {
    C3DVector* Pos;
    CTCCreatureNavigation* PTCNav;
    CThingFilter_IsNavLineOfSightToThing(const CPosThing& thing) : Pos((C3DVector*)&thing.GetPosition())
    {
        thing.PeekTC(&PTCNav);
    }
};

int main()
{
    CPosThing a, b; CTCCreatureNavigation nav; CTCBase o;
    a.Add(7, &nav); a.Add(6, &o); b.Add(8, &o);
    int fail = 0;
    CThingFilter_IsNavLineOfSightToThing fa(a);
    if (fa.Pos != &a.pos || fa.PTCNav != &nav) fail = 1;
    CThingFilter_IsNavLineOfSightToThing fb(b);
    if (fb.Pos != &b.pos) fail = 1;
    if (fail) { std::printf("006a8710_TEST FAIL\n"); return 1; }
    std::printf("006a8710_TEST PASS\n");
    return 0;
}