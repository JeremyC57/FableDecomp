// Behaviour test for 00929b10: src/00929b10.cpp copied verbatim, out-of-line callees stubbed below.
#include <cstdio>
#include <cstring>
#include <cmath>
// ---- source (verbatim) ----
// CAIStateGroup_MinionGuardBribed::Update() -- hold the main player's character; while the state stays valid and the hero lives, look away from the hero (at the point mirrored through the creature) and idle, whistling (WHISTLE_GUARD_BRIBED) at random intervals; afterwards stop the whistle, clear the village's bribe frame and Terminate()
// Grade: RELOCATION_MATCH (retry A 11a) -- the mirrored look point is a named C3DVector local (materialised, Z-first), the player character goes through an inline GetPCharacter() accessor (lea ecx,[eax+34h]), the first delay is an unsigned long added last (frame + min + delay) and the repeat target is `next = frame + delay; next += min;`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929B10, length 785 bytes (+ int3 padding), plain `ret` (virtual), `and esp,-8` frame.
// Callees: GetPPlayerManager @ 0x008FCF80, CPlayerManager::GetMainPlayer const @ 0x00449960, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, GetPWorld @ 0x008FCFC0, GetPWorldSeed
// @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), CWorld::GetWorldFrame @ 0x0049D870 (static), the state's virtual slot 11
// (+0x2c, still valid), CThing::GetPosition @ 0x004C73D0, AIAction_LookAtPos @ 0x0097D580, AIAction_DoNothing @ 0x0097E200,
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the sound component's PlaySound @ 0x0066BD00 /
// StopSample @ 0x0066ADD0, CVectorMap::LowerBound @ 0x0040F020, CTCVillageMember::GetPVillage @ 0x007EE540, Terminate @ 0x008FCF40.
// Data: whistle delay ranges @ 0x0138CB98/0x0138CB9C (first) and 0x0138CBA0/0x0138CBA4 (repeat); g_InvalidSoundHandle @ 0x0129D25C;
// "WHISTLE_GUARD_BRIBED" @ 0x01291A00; 0.0027777f turn speed (immediate).
// Layout: local model (PBrain +4; brain PCreature +0x18; player +0x34 CIntelligentPointer to the character; presence bits +0x20,
// interface map +0x44; interface 0x10 = sound component, 0x23 = CTCVillageMember; village bribe frame +0xA8).
// Shape: the random delays are a static CWorld random called through GetPWorld() (the discarded call); the in-loop sound lookup
// inlines the vector-map lower_bound, the after-loop one calls it.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    static __forceinline SEntry* LowerBoundInline(SEntry* first, SEntry* end, long key)
    {
        long len = end - first;
        SEntry* it = first;
        while (len > 0) {
            long half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < key) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceInline(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            CInterfaceMap::SEntry* end = m_interfaces.last;
            CInterfaceMap::SEntry* it = CInterfaceMap::LowerBoundInline(m_interfaces.first, end, interfaceType);
            *ppOut = (T*)((it == end || interfaceType < it->type) ? end : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingPlayerCreature : CThing { };
struct CTCSoundInfo {                           // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};
struct CVillage {
    unsigned char _pad[0xa8];
    long BribedFrame;                           // +0xA8
};
struct CTCVillageMember {                       // interface 0x23
    CVillage* GetPVillage();                    // 0x007EE540
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerPlayerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    __forceinline CIntelligentPointerPlayerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CPlayer {
    unsigned char _pad[0x34];
    CBaseIntelligentPointer PCharacter;         // +0x34
    const CBaseIntelligentPointer& GetPCharacter() const { return PCharacter; }
};
struct CPlayerManager {
    const CPlayer& GetMainPlayer() const;       // 0x00449960
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
    static __forceinline unsigned long Random(unsigned long range, unsigned long* seed)
    {
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};
extern unsigned long g_InvalidSoundHandle;      // 0x0129D25C
extern long g_BribedWhistleFirstMin;            // 0x0138CB98
extern long g_BribedWhistleFirstMax;            // 0x0138CB9C
extern long g_BribedWhistleRepeatMin;           // 0x0138CBA0
extern long g_BribedWhistleRepeatMax;           // 0x0138CBA4
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
#define VF(n) virtual void v##n();
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10)
    virtual bool IsStillValid();                // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    CThing* GetCreature() const { return PBrain->PCreature; }
    const CPlayerManager* GetPPlayerManager() const;    // 0x008FCF80
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_MinionGuardBribed : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_MinionGuardBribed::Update()
{
    CIntelligentPointerPlayerCreature hero(GetPPlayerManager()->GetMainPlayer().GetPCharacter().GetPItem());
    unsigned long delay = GetPWorld()->Random(g_BribedWhistleFirstMax - g_BribedWhistleFirstMin, GetPWorldSeed());
    long nextWhistle = CWorld::GetWorldFrame() + g_BribedWhistleFirstMin + delay;
    unsigned long sample = g_InvalidSoundHandle;
    while (IsStillValid() && IS_THING_ALIVE(hero)) {
        C3DVector heroPos = hero.GetPItem()->GetPosition();
        const C3DVector& here = GetCreature()->GetPosition();
        C3DVector lookPos = here * 2.0f - heroPos;
        if (AIAction_LookAtPos(1, lookPos, 0x11, 0.0027777f) != 2)
            break;
        if (AIAction_DoNothing() != 4)
            break;
        if (GetPWorld()->GetWorldFrame() >= nextWhistle) {
            CTCSoundInfo* sound;
            if (GetCreature()->GetInterfaceInline(&sound, 0x10))
                sample = sound->PlaySound("WHISTLE_GUARD_BRIBED", 0, 0, 0, -1);
            delay = GetPWorld()->Random(g_BribedWhistleRepeatMax - g_BribedWhistleRepeatMin, GetPWorldSeed());
            nextWhistle = CWorld::GetWorldFrame() + delay;
            nextWhistle += g_BribedWhistleRepeatMin;
        }
    }
    if (sample != g_InvalidSoundHandle) {
        CTCSoundInfo* sound;
        if (GetCreature()->GetInterface(&sound, 0x10))
            sound->StopSample(sample);
    }
    CTCVillageMember* member;
    if (GetCreature()->GetInterface(&member, 0x23) && member->GetPVillage())
        member->GetPVillage()->BribedFrame = 0;
    Terminate();
}

// ---- stubs ----
void CAIStateGroupBase::v0() {}
void CAIStateGroupBase::v1() {}
void CAIStateGroupBase::v2() {}
void CAIStateGroupBase::v3() {}
void CAIStateGroupBase::v4() {}
void CAIStateGroupBase::v5() {}
void CAIStateGroupBase::v6() {}
void CAIStateGroupBase::v7() {}
void CAIStateGroupBase::v8() {}
void CAIStateGroupBase::v9() {}
void CAIStateGroupBase::v10() {}

void CIntelligentPointerPlayerCreature::v00() {}
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(const CThing* item) { m_pItem = (CThing*)item; }
CCharString::CCharString(const char*, long) : m_pData(0) {}
CCharString::~CCharString() {}

static double heroBuf[0x40], creatureBuf[0x40];
static CThing* g_hero = (CThing*)heroBuf;
static CThing* g_creature = (CThing*)creatureBuf;
static C3DVector g_heroPos, g_herePos;
const C3DVector& CThing::GetPosition() const { return this == g_hero ? g_heroPos : g_herePos; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) {
    SEntry* it = first; while (it != last && it->type < *type) ++it; return it;
}
static int g_play, g_stop; static unsigned long g_stopped;
unsigned long CTCSoundInfo::PlaySound(const CCharString&, long, long, long, long) { return 1000 + g_play++; }
void CTCSoundInfo::StopSample(unsigned long id) { g_stop++; g_stopped = id; }
static CVillage g_village; static bool g_hasVillage = true;
CVillage* CTCVillageMember::GetPVillage() { return g_hasVillage ? &g_village : 0; }

static double playerBuf[0x10];
static CPlayerManager g_pm;
const CPlayer& CPlayerManager::GetMainPlayer() const { return *(const CPlayer*)playerBuf; }
const CPlayerManager* CAIStateGroupBase::GetPPlayerManager() const { return &g_pm; }
static CWorld g_world; static unsigned long g_seed;
CWorld* CAIStateGroupBase::GetPWorld() { return &g_world; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return &g_seed; }
unsigned long __fastcall GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static long g_frame;
long CWorld::GetWorldFrame() { return g_frame; }
unsigned long g_InvalidSoundHandle = 0xffffffff;
long g_BribedWhistleFirstMin = 100, g_BribedWhistleFirstMax = 200, g_BribedWhistleRepeatMin = 50, g_BribedWhistleRepeatMax = 80;
static int g_valid, g_terms, g_looks, g_badLook; static long g_whistleFrames[16];
bool CAIStateGroupBase::IsStillValid() { return g_valid-- > 0; }
void CAIStateGroupBase::Terminate() { g_terms++; }
long CAIStateGroupBase::AIAction_LookAtPos(long, C3DVector pos, long flags, float) {
    g_looks++;
    if (std::fabs(pos.X - (2 * g_herePos.X - g_heroPos.X)) > 1e-4f || std::fabs(pos.Y - (2 * g_herePos.Y - g_heroPos.Y)) > 1e-4f
        || std::fabs(pos.Z - (2 * g_herePos.Z - g_heroPos.Z)) > 1e-4f || flags != 0x11) g_badLook++;
    return 2;
}
long CAIStateGroupBase::AIAction_DoNothing() { g_frame += 30; return 4; }

static int fails = 0;
static void check(bool c, const char* m) { if (!c) { std::printf("FAIL %s\n", m); fails++; } }
static unsigned long lcg(unsigned long& s) { s = s * 0x24A1u + 0x24DFu; s = (s >> 13) | (s << 19); return s; }
static char soundTC, memberTC;

int main()
{
    ((CPlayer*)playerBuf)->PCharacter.m_pItem = g_hero;
    CAIBrain brain; std::memset(&brain, 0, sizeof brain); brain.PCreature = g_creature;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)&soundTC }, { 0x23, (CTCBase*)&memberTC } };
    g_creature->m_interfaces.first = entries; g_creature->m_interfaces.last = entries + 2;
    g_creature->m_presentBits[0] |= 1UL << 0x10; g_creature->m_presentBits[1] |= 1UL << (0x23 & 31);
    g_heroPos.X = 1; g_heroPos.Y = 2; g_heroPos.Z = 3; g_herePos.X = 5; g_herePos.Y = -1; g_herePos.Z = 0.5f;
    CAIStateGroup_MinionGuardBribed g; g.PBrain = &brain;

    // 20 iterations, 30 frames each: whistles at the expected frames, sample stopped, village frame cleared
    g_seed = 1234; g_frame = 0; g_valid = 20; g_village.BribedFrame = 99;
    unsigned long s = 1234; long next = 100 + (long)(lcg(s) % 100); int expectedWhistles = 0; long f = 0;
    for (int i = 0; i < 20; ++i) { f += 30; if (f >= next) { expectedWhistles++; next = f + (long)(lcg(s) % 30) + 50; } }
    g.Update();
    check(g_looks == 20 && g_badLook == 0, "look at mirrored point");
    check(g_play == expectedWhistles && expectedWhistles > 0, "whistles");
    check(g_stop == 1 && g_stopped == (unsigned long)(1000 + g_play - 1), "stop last sample");
    check(g_village.BribedFrame == 0 && g_terms == 1, "village + terminate");
    check(g_seed == s, "seed");

    // dead hero: no looks, no sample to stop, still terminates
    g_play = g_stop = 0; g_looks = 0; g_terms = 0; g_valid = 5; g_hero->m_flags91 = 1; g_village.BribedFrame = 7;
    g.Update();
    check(g_looks == 0 && g_play == 0 && g_stop == 0 && g_village.BribedFrame == 0 && g_terms == 1, "dead hero");
    // no village: frame left alone
    g_hero->m_flags91 = 0; g_valid = 0; g_hasVillage = false; g_village.BribedFrame = 7; g_terms = 0;
    g.Update();
    check(g_village.BribedFrame == 7 && g_terms == 1, "no village");
    if (fails == 0) std::printf("00929b10_TEST PASS\n");
    return fails;
}