// Behaviour test for 009109b0: final.cpp copied verbatim, the virtual type-flags getter, LowerBound, GetPItem and
// IsActionPointFree stubbed so every arm of the seat/ownership/free-point test is observable.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CIsSeatWithSpaceOwnedBy::operator()(CThing*) const -- the thing-pointer twin of 0x00945E70: the thing must not be in limbo; then a seat (type-flag bit 16 or 17), owned by POwner (its owned-entity component, kind 0x33, names POwner; with no POwner the seat must be unowned), whose user-action-point component (kind 0x2d) still has a free point
// Leaf: ??RCIsSeatWithSpaceOwnedBy@@QBE_NPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009109B0, 219 bytes, `ret 4` (the thing-pointer twin of the landed 0x00945E70; same body after the first test).
// Callees: the thing's virtual type-flags getter (slot 48, +0xc0, called twice), the const component-map LowerBound @
//   0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the owned-entity component's POwner),
//   CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0.
// Layout: Ego_r.pdb CIsSeatWithSpaceOwnedBy {POwner +0}; CTCOwnedEntity (packed) POwner +0x0d; retail CThing: component
//   bitset +0x20 (kinds 0x2d / 0x33 = bits 13 / 19 of the word at +0x24), map +0x44, InLimbo bit 5 of +0x91 (the landed
//   CIsNearCampfireWithSpace 0x00939A50 model and the landed item-pointer twin 0x00945E70).
// Shape: nested tests sharing one `xor al,al` exit; the two flag bits are `(unsigned char)(flags >> n) & 1` with a
//   second virtual call; the ownership test is a `mismatch` bool set in an if/else on POwner (the owned arm
//   `!owned || !ownedBit || owner != POwner`, the unowned arm `owned`) and tested once: VC7.1 threads it, so the owned
//   arm's compare jumps into the unowned arm's `jne`. The owner is read through a direct component-map Find (the mask was
//   already tested as ownedBit; the out-parameter GetInterface form pops edi early and compares from memory); the
//   free-point test returns through its own `mov al,1` exit.
class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* PData;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
struct CTCBase_Retail;
struct STCEntry_Retail {
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTCUserActionPointBase {
    bool IsActionPointFree();                      // 0x007FEBF0
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                            // component kind 0x33
    unsigned char           _base[0x0c];           // CTCBase
    bool                    SwitchableNavigationTCAdded;   // +0x0c
    CBaseIntelligentPointer POwner;                // +0x0d CIntelligentPointer<CThing>
};
#pragma pack(pop)
class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual unsigned long GetThingTypeFlags() const;   // slot 48 (+0xc0)
    unsigned char  _pad04[0x0c];
    unsigned char  m_kind;                         // +0x10 (5 = the seat's thing kind tested by the item-pointer twin)
    unsigned char  _pad11[0x0f];
    unsigned long  m_tcMask[8];                    // +0x20
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  Dead : 1;                       // +0x91
    unsigned char  _bits91 : 4;
    unsigned char  InLimbo : 1;                    // +0x91 bit 5
    unsigned char  _bits91b : 2;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CThing* GetOwnerOf(const CThing* thing)
{
    CTCOwnedEntity* owned = (CTCOwnedEntity*)thing->m_tcs.Find(0x33);
    return (CThing*)owned->POwner.GetPItem();
}
class CIsSeatWithSpaceOwnedBy {
public:
    CThing* POwner;                                // +0x00
    bool operator()(CThing* thing) const;
};

bool CIsSeatWithSpaceOwnedBy::operator()(CThing* thing) const
{
    if (!thing->InLimbo) {
        if (((unsigned char)(thing->GetThingTypeFlags() >> 16) & 1) || ((unsigned char)(thing->GetThingTypeFlags() >> 17) & 1)) {
            unsigned long ownedBit = thing->HasTC(0x33);
            bool owned = ownedBit != 0;
            bool mismatch;
            if (POwner)
                mismatch = !owned || !ownedBit || GetOwnerOf(thing) != POwner;
            else
                mismatch = owned;
            if (mismatch)
                return false;
            if (thing->HasTC(0x2d)) {
                CTCUserActionPointBase* points = (CTCUserActionPointBase*)thing->m_tcs.Find(0x2d);
                if (points->IsActionPointFree())
                    return true;
            }
        }
    }
    return false;
}
// ---- stubs ----
#define V(n) void CThing::v##n() { }
V(000) V(001) V(002) V(003) V(004) V(005) V(006) V(007) V(008) V(009) V(010) V(011) V(012) V(013) V(014) V(015)
V(016) V(017) V(018) V(019) V(020) V(021) V(022) V(023) V(024) V(025) V(026) V(027) V(028) V(029) V(030) V(031)
V(032) V(033) V(034) V(035) V(036) V(037) V(038) V(039) V(040) V(041) V(042) V(043) V(044) V(045) V(046) V(047)
static unsigned long g_flags = 0;
static bool g_free = false;
static char g_buf[128];
unsigned long CThing::GetThingTypeFlags() const { note("GetThingTypeFlags"); return g_flags; }
STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind) const
{
    _snprintf(g_buf, sizeof(g_buf), "LowerBound(%lu)", kind);
    note(g_buf);
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
    return it;
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { note("GetPItem"); return (CBaseObject*)PData; }
bool CTCUserActionPointBase::IsActionPointFree() { note("IsActionPointFree"); return g_free; }

static CTCOwnedEntity g_owned;
static CTCUserActionPointBase g_points;
static STCEntry_Retail g_entries[3];
static CThing* MakeThing(bool limbo, bool owned, bool points)
{
    CThing* t = new CThing();
    memset(t->m_tcMask, 0, sizeof(t->m_tcMask));
    t->InLimbo = limbo ? 1 : 0;
    t->Dead = 0;
    int n = 0;
    g_entries[n].m_kind = 0x10; g_entries[n].m_pTC = 0; ++n;        // an unrelated component first
    if (points) { g_entries[n].m_kind = 0x2d; g_entries[n].m_pTC = (CTCBase_Retail*)&g_points; ++n; t->m_tcMask[1] |= 1UL << 13; }
    if (owned)  { g_entries[n].m_kind = 0x33; g_entries[n].m_pTC = (CTCBase_Retail*)&g_owned; ++n; t->m_tcMask[1] |= 1UL << 19; }
    t->m_tcs.m_pFirst = g_entries; t->m_tcs.m_pLast = g_entries + n; t->m_tcs.m_pEnd = g_entries + 3;
    return t;
}
#define RUN(pred, thing) (g_log[0] = 0, (pred).operator()(thing))

int main()
{
    CThing* me = (CThing*)0x1234;
    CThing* other = (CThing*)0x5678;
    CIsSeatWithSpaceOwnedBy mine; mine.POwner = me;
    CIsSeatWithSpaceOwnedBy nobody; nobody.POwner = 0;
    bool r;

    // 1. in limbo: rejected before any call
    r = RUN(mine, MakeThing(true, true, true));
    CHECK(!r); CHECKLOG("");

    // 2. not a seat (neither flag bit): both virtual calls, rejected
    g_flags = 0;
    r = RUN(mine, MakeThing(false, true, true));
    CHECK(!r); CHECKLOG("GetThingTypeFlags;GetThingTypeFlags;");

    // 3. seat by bit 16 (one virtual call), POwner set but the seat is unowned: rejected without a lookup
    g_flags = 1UL << 16;
    r = RUN(mine, MakeThing(false, false, true));
    CHECK(!r); CHECKLOG("GetThingTypeFlags;");

    // 4. seat by bit 17 only (two calls), owned by someone else: rejected after the owner lookup
    g_flags = 1UL << 17; g_owned.POwner.PData = other;
    r = RUN(mine, MakeThing(false, true, true));
    CHECK(!r); CHECKLOG("GetThingTypeFlags;GetThingTypeFlags;LowerBound(51);GetPItem;");

    // 5. owned by POwner, no action-point component: rejected after the owner lookup
    g_owned.POwner.PData = me;
    r = RUN(mine, MakeThing(false, true, false));
    CHECK(!r); CHECKLOG("GetThingTypeFlags;GetThingTypeFlags;LowerBound(51);GetPItem;");

    // 6. owned by POwner, action points, a point is free: accepted
    g_free = true;
    r = RUN(mine, MakeThing(false, true, true));
    CHECK(r); CHECKLOG("GetThingTypeFlags;GetThingTypeFlags;LowerBound(51);GetPItem;LowerBound(45);IsActionPointFree;");

    // 7. same but no free point: rejected
    g_free = false;
    r = RUN(mine, MakeThing(false, true, true));
    CHECK(!r); CHECKLOG("GetThingTypeFlags;GetThingTypeFlags;LowerBound(51);GetPItem;LowerBound(45);IsActionPointFree;");

    // 8. no POwner wanted, seat is owned: rejected without a lookup
    g_flags = 1UL << 16;
    r = RUN(nobody, MakeThing(false, true, true));
    CHECK(!r); CHECKLOG("GetThingTypeFlags;");

    // 9. no POwner wanted, unowned seat with a free point: accepted
    g_free = true;
    r = RUN(nobody, MakeThing(false, false, true));
    CHECK(r); CHECKLOG("GetThingTypeFlags;LowerBound(45);IsActionPointFree;");

    // 10. no POwner wanted, unowned seat without action points: rejected
    r = RUN(nobody, MakeThing(false, false, false));
    CHECK(!r); CHECKLOG("GetThingTypeFlags;");

    if (fails == 0) printf("009109b0_TEST PASS\n");
    else printf("009109b0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}