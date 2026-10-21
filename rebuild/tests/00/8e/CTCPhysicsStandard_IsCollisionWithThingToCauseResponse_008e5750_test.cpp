// CTCPhysicsStandard::IsCollisionWithThingToCauseResponse(CTCPhysicsBase* other) -- a thing-thing collision causes a physical response only when this component collides with things (two-way or one-way) and the other does too; a parent with a collision-filter component (interface 0x4D) gets the final say for the other's thing
// Leaf: IsCollisionWithThingToCauseResponse
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E5750, 127 bytes.
// Callees: virtual slots 57 (+0xe4, GetCollidesWithThings: the override 0x00724C90 reads +0x84 bit 7) and 58 (+0xe8,
//   GetCollidesWithThingsOneWay: 0x00724CA0 reads +0x85 bit 0); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0
//   (this TU's copy; inlined GetTC, interface 0x4D = bit 13 of CThing+0x28); the filter component's test @ 0x00770DE0
//   (bool (CThing*), tail jump; catalog "ContainerFindElement" is propagated, named descriptively).
// Layout: CTCBase ParentThing +0x04; CThing component bitset +0x20, interface map +0x44.
// Shape: nested ifs with one shared `xor al,al` exit; `HasTC(k)` as the raw masked word, the filter a named local so the
//   final call tail-jumps. GetTC picks the end/hit entry with ONE ternary, so the component is loaded straight into ecx
//   before the argument (an if-assign pick joins it through eax and moves it to ecx).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CTCCollisionFilter {                     // the interface-0x4D component
    bool ShouldCollideWith(CThing* other);      // 0x00770DE0
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056();
    virtual bool GetCollidesWithThings();   // slot 57 (+0xe4)
    virtual bool GetCollidesWithThingsOneWay();   // slot 58 (+0xe8)
    CThing* ParentThing;                        // +0x04
    CThing* GetParentThing_rb() const { return ParentThing; }
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    bool IsCollisionWithThingToCauseResponse(CTCPhysicsBase* other);
};

bool CTCPhysicsStandard::IsCollisionWithThingToCauseResponse(CTCPhysicsBase* other)
{
    if (GetCollidesWithThings() || GetCollidesWithThingsOneWay()) {
        if (other->GetCollidesWithThings()) {
            if (ParentThing->HasTC(0x4d)) {
                CTCCollisionFilter* filter = (CTCCollisionFilter*)ParentThing->GetTC(0x4d);
                return filter->ShouldCollideWith(other->GetParentThing_rb());
            }
            return true;
        }
    }
    return false;
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static bool g_filterResult = false; static CThing* g_filterOther = 0; static const void* g_filterThis = 0; static int g_filterCalls = 0;
bool CTCCollisionFilter::ShouldCollideWith(CThing* other) { ++g_filterCalls; g_filterOther = other; g_filterThis = this; return g_filterResult; }
struct CPhys : CTCPhysicsStandard {
    bool twoWay, oneWay;
    virtual bool GetCollidesWithThings() { return twoWay; }
    virtual bool GetCollidesWithThingsOneWay() { return oneWay; }
};
#define STUB(n) void CTCPhysicsBase::n() { }
STUB(v000) STUB(v001) STUB(v002) STUB(v003) STUB(v004) STUB(v005) STUB(v006) STUB(v007) STUB(v008) STUB(v009)
STUB(v010) STUB(v011) STUB(v012) STUB(v013) STUB(v014) STUB(v015) STUB(v016) STUB(v017) STUB(v018) STUB(v019)
STUB(v020) STUB(v021) STUB(v022) STUB(v023) STUB(v024) STUB(v025) STUB(v026) STUB(v027) STUB(v028) STUB(v029)
STUB(v030) STUB(v031) STUB(v032) STUB(v033) STUB(v034) STUB(v035) STUB(v036) STUB(v037) STUB(v038) STUB(v039)
STUB(v040) STUB(v041) STUB(v042) STUB(v043) STUB(v044) STUB(v045) STUB(v046) STUB(v047) STUB(v048) STUB(v049)
STUB(v050) STUB(v051) STUB(v052) STUB(v053) STUB(v054) STUB(v055) STUB(v056)
bool CTCPhysicsBase::GetCollidesWithThings() { return false; }
bool CTCPhysicsBase::GetCollidesWithThingsOneWay() { return false; }
static CThing g_self, g_otherThing;
static CTCCollisionFilter g_filter;
static CInterfaceMap::SEntry g_entries[1];
static void SetBit(CThing& t, long k, bool on)
{
    if (on) t.m_presentBits[k >> 5] |= 1UL << (k & 31);
    else t.m_presentBits[k >> 5] &= ~(1UL << (k & 31));
}
int main()
{
    g_entries[0].type = 0x4d; g_entries[0].component = (CTCBase*)&g_filter;
    g_self.m_interfaces.first = g_entries; g_self.m_interfaces.last = g_entries + 1;
    CPhys me, other;
    me.ParentThing = &g_self; other.ParentThing = &g_otherThing;
    me.twoWay = false; me.oneWay = false; other.twoWay = true; other.oneWay = false;
    CHECK(!me.IsCollisionWithThingToCauseResponse(&other));          // this does not collide with things
    me.oneWay = true;
    CHECK(me.IsCollisionWithThingToCauseResponse(&other));           // one-way, no filter
    other.twoWay = false;
    CHECK(!me.IsCollisionWithThingToCauseResponse(&other));          // the other does not collide
    other.twoWay = true; me.oneWay = false; me.twoWay = true;
    SetBit(g_self, 0x4d, true); g_filterResult = false;
    CHECK(!me.IsCollisionWithThingToCauseResponse(&other));          // filter refuses
    CHECK(g_filterCalls == 1 && g_filterOther == &g_otherThing && g_filterThis == &g_filter);
    g_filterResult = true;
    CHECK(me.IsCollisionWithThingToCauseResponse(&other));           // filter accepts
    CHECK(g_filterCalls == 2);
    if (fails == 0) printf("008e5750_TEST PASS\n");
    return fails;
}