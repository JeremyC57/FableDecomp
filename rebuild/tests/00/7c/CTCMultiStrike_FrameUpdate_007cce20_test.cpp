// Behaviour test for 007cce20: src/007cce20.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCMultiStrike::FrameUpdate()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCMultiStrike::FrameUpdate() -- once the strike has started, count the delay down; when it runs out hit the target creature with the owner's melee hit (and tell its entity events about the special-ability hit), count the hit, remove the multi-strike from the owner after the def's last hit, and restart the delay from the def
// Leaf: ?FrameUpdate@CTCMultiStrike@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CCE20, 299 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget x3, PWeapon), CHitParameters() 0x007A3BC0 (label
//   "CWound_Constructor" is a forge autoname) / ~CHitParameters 0x00789358, CThingGameObject::GetMeleeHitParameters
//   0x00838BC0, the target's virtual OnHit(CHitParameters&) (slot 65, +0x104), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCEntityEvents 0x42), operator new 0x00BFEA1A (0x18),
//   NEntityEvents::CEventHitBySpecialAbility(const CHitParameters&, long) 0x00811830 (ability 9),
//   CTCEntityEvents::AddEvent 0x008113C0, CThing::RemoveTC(long) 0x004C9840 (0x87), _ftol 0x00BFEA70.
// Globals: g_framesPerSecond 0x01375550 (`fild`).
// Layout: Ego_r.pdb CTCMultiStrike (PDef +0x0c, SpellLevel +0x10, StrikeStarted +0x14, DelayTimer +0x18, NumHits +0x1c,
//   PHitLocation +0x20, PWeapon +0x24, PointOfHit +0x2c, HitDirection +0x38, PTarget +0x44); CSpecialAbilitiesMultiStrikeDef
//   with 12-byte retail vectors (DelayTimeSecs +0x6c, NumHits +0x78); CHitParameters 0x60 bytes, MoveBack +0x22; CThing
//   ThingTypeFlags +0x6c bit 3, interface bitset +0x20 (0x42 = word 2 bit 2), map +0x44.
// Shape: the owner is read into a local first; the target is a named third GetPItem before the hit parameters; the
//   entity-events component is a named local (retail does the lookup before the new); the delay is the int frame rate
//   times the def's seconds, truncated.
class CTCBase;
class CThing;
namespace NHitLocations { class CHitLocation; }

class C3DVector {
public:
    float X, Y, Z;
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x22];
    bool MoveBack;                                  // +0x22
    unsigned char _pad23[0x60 - 0x23];
    CHitParameters();                               // 0x007A3BC0
    ~CHitParameters();                              // 0x00789358
};

class CEventBase {
public:
    virtual ~CEventBase();
};
namespace NEntityEvents {
class CEventHitBySpecialAbility : public CEventBase {   // 0x18 bytes, vtable 0x012723F0
public:
    unsigned char _pad04[0x18 - 0x04];
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);   // 0x00811830
};
}
class CTCEntityEvents {                             // interface 0x42
public:
    void AddEvent(CEventBase* event);               // 0x008113C0
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void OnHit(CHitParameters& hit);        // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                 // +0x20 bitset<274>
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                   // +0x6C (bit 3: game object)
    void RemoveTC(long interfaceType);              // 0x004C9840
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CThingGameObject : public CThing {
public:
    void GetMeleeHitParameters(NHitLocations::CHitLocation* hitLocation, CThing& hitter, CThing* weapon,
                               const C3DVector& position, const C3DVector& direction, CHitParameters& out) const;   // 0x00838BC0
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

template <class T> class CStdVector {               // std::vector, 12 bytes in retail
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](long i) const { return _M_start[i]; }
};
struct CSpecialAbilitiesMultiStrikeDef {
    unsigned char      _pad00[0x6c];
    CStdVector<float>  DelayTimeSecs;               // +0x6c
    CStdVector<long>   NumHits;                     // +0x78
};

extern long g_framesPerSecond;                      // 0x01375550

class CTCMultiStrike {
public:
    virtual void v00();
    CThing*                          ParentThing;   // +0x04 (CTCBase)
    unsigned char                    _pad08[0x0c - 0x08];
    CSpecialAbilitiesMultiStrikeDef* PDef;          // +0x0c (CDefPointer)
    long                             SpellLevel;    // +0x10
    bool                             StrikeStarted; // +0x14
    long                             DelayTimer;    // +0x18
    long                             NumHits;       // +0x1c
    NHitLocations::CHitLocation*     PHitLocation;  // +0x20
    CIntelligentPointer<CThing>      PWeapon;       // +0x24
    C3DVector                        PointOfHit;    // +0x2c
    C3DVector                        HitDirection;  // +0x38
    CIntelligentPointer<CThing>      PTarget;       // +0x44
    void FrameUpdate();
};

void CTCMultiStrike::FrameUpdate()
{
    CThing* owner = ParentThing;
    if (StrikeStarted && --DelayTimer == 0) {
        if (PTarget.GetPItem() && (PTarget.GetPItem()->ThingTypeFlags & 8)) {
            CThing* target = PTarget.GetPItem();
            CHitParameters hit;
            hit.MoveBack = false;
            ((CThingGameObject*)target)->GetMeleeHitParameters(PHitLocation, *owner, PWeapon.GetPItem(), PointOfHit, HitDirection, hit);
            target->OnHit(hit);
            if (target->HasTC(0x42)) {
                CTCEntityEvents* events = target->GetTC<CTCEntityEvents>(0x42);
                events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 9));
            }
        }
        ++NumHits;
        if (NumHits == PDef->NumHits[SpellLevel])
            owner->RemoveTC(0x87);
        DelayTimer = (long)(g_framesPerSecond * PDef->DelayTimeSecs[SpellLevel]);
    }
}

// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
void CThing::v000() {}
void CThing::v001() {}
void CThing::v002() {}
void CThing::v003() {}
void CThing::v004() {}
void CThing::v005() {}
void CThing::v006() {}
void CThing::v007() {}
void CThing::v008() {}
void CThing::v009() {}
void CThing::v010() {}
void CThing::v011() {}
void CThing::v012() {}
void CThing::v013() {}
void CThing::v014() {}
void CThing::v015() {}
void CThing::v016() {}
void CThing::v017() {}
void CThing::v018() {}
void CThing::v019() {}
void CThing::v020() {}
void CThing::v021() {}
void CThing::v022() {}
void CThing::v023() {}
void CThing::v024() {}
void CThing::v025() {}
void CThing::v026() {}
void CThing::v027() {}
void CThing::v028() {}
void CThing::v029() {}
void CThing::v030() {}
void CThing::v031() {}
void CThing::v032() {}
void CThing::v033() {}
void CThing::v034() {}
void CThing::v035() {}
void CThing::v036() {}
void CThing::v037() {}
void CThing::v038() {}
void CThing::v039() {}
void CThing::v040() {}
void CThing::v041() {}
void CThing::v042() {}
void CThing::v043() {}
void CThing::v044() {}
void CThing::v045() {}
void CThing::v046() {}
void CThing::v047() {}
void CThing::v048() {}
void CThing::v049() {}
void CThing::v050() {}
void CThing::v051() {}
void CThing::v052() {}
void CThing::v053() {}
void CThing::v054() {}
void CThing::v055() {}
void CThing::v056() {}
void CThing::v057() {}
void CThing::v058() {}
void CThing::v059() {}
void CThing::v060() {}
void CThing::v061() {}
void CThing::v062() {}
void CThing::v063() {}
void CThing::v064() {}

static int g_hits = 0; static const CHitParameters* g_onHit = 0; static bool g_moveBackAtHit = true;
void CThing::OnHit(CHitParameters& hit) { Log("OnHit"); g_onHit = &hit; g_moveBackAtHit = hit.MoveBack; }
static long g_removed = -1; static CThing* g_removedFrom = 0;
void CThing::RemoveTC(long interfaceType) { Log("RemoveTC"); g_removed = interfaceType; g_removedFrom = this; }
static int g_params = 0;
CHitParameters::CHitParameters() { Log("Params"); MoveBack = true; ++g_params; }
CHitParameters::~CHitParameters() { --g_params; }
CEventBase::~CEventBase() {}
static const CHitParameters* g_evParams = 0; static long g_evAbility = -1;
NEntityEvents::CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& params, long ability)
{ Log("Event"); g_evParams = &params; g_evAbility = ability; }
static CEventBase* g_added = 0; static CTCEntityEvents* g_addedTo = 0;
void CTCEntityEvents::AddEvent(CEventBase* event) { Log("AddEvent"); g_added = event; g_addedTo = this; }
static const NHitLocations::CHitLocation* g_loc = 0; static CThing* g_hitter = 0; static CThing* g_weapon = 0;
static const C3DVector* g_pos = 0; static const C3DVector* g_dir = 0; static const CHitParameters* g_out = 0; static const CThing* g_meleeThis = 0;
void CThingGameObject::GetMeleeHitParameters(NHitLocations::CHitLocation* hitLocation, CThing& hitter, CThing* weapon,
                                             const C3DVector& position, const C3DVector& direction, CHitParameters& out) const
{ Log("Melee"); g_loc = hitLocation; g_hitter = &hitter; g_weapon = weapon; g_pos = &position; g_dir = &direction; g_out = &out; g_meleeThis = this; }
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CTCMultiStrike::v00() {}
long g_framesPerSecond = 30;

static void Reset() { g_log[0] = 0; g_step = 0; g_removed = -1; g_removedFrom = 0; g_added = 0; g_onHit = 0; }

int main()
{
    static CThing owner, target, weapon;
    static CTCEntityEvents events;
    static CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x42; entries[1].component = (CTCBase*)&events;
    memset(target.m_presentBits, 0, sizeof target.m_presentBits);
    target.m_interfaces.first = entries; target.m_interfaces.last = entries + 2;
    target.ThingTypeFlags = 8;
    float delays[3] = { 0.5f, 1.0f, 2.0f };
    long hits[3] = { 3, 4, 5 };
    static CSpecialAbilitiesMultiStrikeDef def;
    def.DelayTimeSecs._M_start = delays; def.NumHits._M_start = hits;
    CTCMultiStrike ms; memset(&ms, 0, sizeof ms);
    ms.ParentThing = &owner; ms.PDef = &def; ms.SpellLevel = 1;
    ms.PHitLocation = (NHitLocations::CHitLocation*)0x1234;
    ms.PWeapon.m_pItem = &weapon; ms.PTarget.m_pItem = &target;
    // 1: not started: nothing happens
    {
        Reset(); ms.StrikeStarted = false; ms.DelayTimer = 1;
        ms.FrameUpdate();
        CHECK(g_log[0] == 0 && ms.DelayTimer == 1);
    }
    // 2: started, delay not yet run out: only the countdown
    {
        Reset(); ms.StrikeStarted = true; ms.DelayTimer = 3;
        ms.FrameUpdate();
        CHECK(g_log[0] == 0 && ms.DelayTimer == 2 && ms.NumHits == 0);
    }
    // 3: delay runs out on a game object without entity events: hit, count, restart the delay (30 * 1.0)
    {
        Reset(); ms.DelayTimer = 1; ms.NumHits = 0;
        ms.FrameUpdate();
        CHECK(strcmp(g_log, "1:Params;2:Melee;3:OnHit;") == 0);
        CHECK(g_meleeThis == &target && g_loc == ms.PHitLocation && g_hitter == &owner && g_weapon == &weapon);
        CHECK(g_pos == &ms.PointOfHit && g_dir == &ms.HitDirection && g_out == g_onHit && !g_moveBackAtHit);
        CHECK(ms.NumHits == 1 && ms.DelayTimer == 30 && g_params == 0);
    }
    // 4: with entity events, last hit (NumHits reaches 4): event added, multi-strike removed from the owner
    {
        Reset(); target.m_presentBits[2] = 1UL << 2; ms.DelayTimer = 1; ms.NumHits = 3; ms.SpellLevel = 2; hits[2] = 4;
        ms.FrameUpdate();
        CHECK(strcmp(g_log, "1:Params;2:Melee;3:OnHit;4:Event;5:AddEvent;6:RemoveTC;") == 0);
        CHECK(g_addedTo == &events && g_added != 0 && g_evParams == g_onHit && g_evAbility == 9);
        CHECK(g_removed == 0x87 && g_removedFrom == &owner);
        CHECK(ms.NumHits == 4 && ms.DelayTimer == 60 && g_params == 0);
        delete g_added;
    }
    // 5: target not a game object: no hit, still counts
    {
        Reset(); target.ThingTypeFlags = 0; ms.DelayTimer = 1; ms.NumHits = 0; ms.SpellLevel = 0;
        ms.FrameUpdate();
        CHECK(g_log[0] == 0 && ms.NumHits == 1 && ms.DelayTimer == 15);
    }
    // 6: no target
    {
        Reset(); ms.PTarget.m_pItem = 0; ms.DelayTimer = 1; ms.NumHits = 2;
        ms.FrameUpdate();
        CHECK(strcmp(g_log, "1:RemoveTC;") == 0 && ms.NumHits == 3 && ms.DelayTimer == 15);
    }
    if (fails == 0) printf("007cce20_TEST PASS\n");
    return fails;
}