// CAIStateGroup_MinionGuardFollowPlayer::IsRunnable() -- at most once per second: a village guard follows the hero
// when the hero is alive, the guard's village has no live guard-follower yet, and this guard is the nearest
// creature with the guard component (0xB4) to the hero; otherwise re-arm the one-second timer.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090C1A0, length 293 bytes (+ int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870 (x2), CWorld::GetCurrentPlayer @ 0x0049D9D0 (the world at +0x24 of the
// global @ 0x013B86A0), CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface: 0x23 = CTCVillager bit 3 of +0x24), GFGetPVillageInterface @ 0x007EE540 (x2; manifest
// "IS_THING_ALIVE<...>" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the village's +0xbd guard
// pointer), CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThing<CThingFilter_HasTC, CThingCompare_Nearest, CThingAICreature> @ 0x0065F744, own
// virtual slot 14 (+0x38, IsStillRunnable(bool)). Global: the frame-rate long @ 0x01375550.
// Shape: the frame gate is unsigned (`jbe`); the HasTC test is part of the alive condition and the villager is
// then looked up unconditionally; the nearest-thing compare holds a by-value copy of the hero position.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

class C3DVector;
class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
class CThingPlayerCreature : public CThing { };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern long g_framesPerSecond;                  // 0x01375550
class C3DVector { public:
    float x, y, z;
    C3DVector(const C3DVector& v) : x(v.x), y(v.y), z(v.z) { }
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                // 0x00487DC0
};
struct CWorldP {
    CPlayer* GetCurrentPlayer();                // 0x0049D9D0
};
struct CGameGlobals {
    unsigned char _pad[0x24];
    CWorldP* m_pWorld;                          // +0x24
};
extern CGameGlobals* g_pMainGameComponent;      // 0x013B86A0
struct CThingPos : CThing {
};
struct CTCVillager;
#pragma pack(push, 1)
struct CVillage {
    unsigned char _pad[0xbd];
    CBaseIntelligentPointer PGuardFollower;     // +0xbd (packed)
};
#pragma pack(pop)
CVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
class CThingFilter_HasTC { public:
    long m_type;
    CThingFilter_HasTC(long type) : m_type(type) { }
};
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
class CThingAICreature;
class CThingSearchTools { public:
    CThing** m_things;                          // candidate list (stand-in model)
    CThing** m_thingsEnd;
    bool GetPBestThing(const CThingFilter_HasTC& filter, const CThingCompare_Nearest& compare, CThingAICreature** out);  // 0x0065F744
};

struct CAIStateGroup_MinionGuardFollowPlayer : CAIStateGroupBase {
    unsigned char _pad14[4];
    unsigned long NextCheckFrame;               // +0x18
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    virtual bool IsRunnable();
};
struct CAIStateGroup_MinionGuardFollowPlayerV {
    V4(a) V4(b) V4(c)
    virtual void v12(); virtual void v13();
    virtual bool IsStillRunnable(bool starting);   // slot 14 (+0x38)
};

// GetPBestThing stand-in: the nearest candidate carrying the filter's interface.
#pragma auto_inline(off)
bool CThingSearchTools::GetPBestThing(const CThingFilter_HasTC& filter, const CThingCompare_Nearest& compare, CThingAICreature** out)
{
    CThing* best = 0;
    float bestDistance = 0.0f;
    for (CThing** it = m_things; it != m_thingsEnd; ++it) {
        CThing* thing = *it;
        if (!thing->HasTC(filter.m_type))
            continue;
        const C3DVector& pos = ((CThingPos*)thing)->GetPos();
        float dx = pos.x - compare.m_pos.x;
        float dy = pos.y - compare.m_pos.y;
        float dz = pos.z - compare.m_pos.z;
        float distance = dx * dx + dy * dy + dz * dz;
        if (!best || distance < bestDistance) {
            best = thing;
            bestDistance = distance;
        }
    }
    *out = (CThingAICreature*)best;
    return best != 0;
}
#pragma auto_inline(on)

bool CAIStateGroup_MinionGuardFollowPlayer::IsRunnable()
{
    if (NextCheckFrame > (unsigned long)GFGetCurrentFrame())
        return false;
    CThing* hero = g_pMainGameComponent->m_pWorld->GetCurrentPlayer()->GetPControlledCreature();
    if (hero && !(hero->m_flags91 & 1) && PBrain->PCreature->HasTC(0x23)) {
        CTCVillager* villager = (CTCVillager*)PBrain->PCreature->FindEntry(0x23)->component;
        if (GFGetPVillageInterface(villager)) {
            CVillage* village = GFGetPVillageInterface(villager);
            CThing* follower = village->PGuardFollower.GetPItem();
            if (!follower || (follower->m_flags91 & 1)) {
                CThingAICreature* nearest;
                PeekThingSearchTools()->GetPBestThing(CThingFilter_HasTC(0xb4),
                    CThingCompare_Nearest(((CThingPos*)hero)->GetPos()), &nearest);
                if ((CThing*)nearest == PBrain->PCreature
                        && ((CAIStateGroup_MinionGuardFollowPlayerV*)this)->IsStillRunnable(true))
                    return true;
            }
        }
    }
    long oneSecond = g_framesPerSecond;
    NextCheckFrame = GFGetCurrentFrame() + oneSecond;
    return false;
}