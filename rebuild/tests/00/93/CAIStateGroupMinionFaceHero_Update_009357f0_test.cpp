#include <stdio.h>
#include <string.h>
// CAIStateGroup_MinionFaceHero::Update() -- for up to six seconds while the hero lives: look at the hero, and stop once the hero (perceived in extended line of sight) is within 2.2m; idle a tick between looks; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009357F0, length 384 bytes (both exits are `jmp Terminate` tail calls).
// Callees: GFGetCurrentFrame @ 0x0049D870 (__fastcall, no object), CBaseIntelligentPointer ctor/GetPItem/SetPItem
//          (PHero +0x14), CAIStateGroupBase::AIAction_LookAtThing @ 0x0097D6F0 (CIntelligentPointer<const CThing> by
//          value, vtable 0x01237F1C), the inlined STLport lower_bound of the interface map (0x34 = CTCPerceiveThing, bit 20
//          of +0x24), CTCPerceiveThing::IsPlayerExtendedLineOfSight @ 0x00744830, CThing::GetPosition @ 0x004C73D0,
//          AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Globals: FramesPerSecond @ 0x01375550, 4.84f (2.2 squared) @ 0x012922D4.
// Shape: FramesPerSecond is read into a named local at the top of each pass (before the frame call) and the elapsed
//          test is unsigned (`ja`) against it * 6; the dead-hero / timeout exit is the loop's own exit and the other exits
//          are inner `Terminate(); return;`s (two separate tails); IS_THING_ALIVE reads the dead bit through an inline
//          CThing::IsDead and the interface map's lower_bound is __forceinline; the distance is `creature - hero` built member-wise, squared as one
//          expression (VC7.1 sums Z first).
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04), the hero
//          intelligent pointer at +0x14.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __forceinline SEntry* LowerBound(const long* type)        // STLport lower_bound over the 8-byte entries (inlined in this TU)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector operator-(const C3DVector& v) const { C3DVector r; r.X = X - v.X; r.Y = Y - v.Y; r.Z = Z - v.Z; return r; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCPerceiveThing {                       // interface 0x34
    bool IsPlayerExtendedLineOfSight();         // 0x00744830
};
unsigned long __fastcall GFGetCurrentFrame();   // 0x0049D870
extern unsigned long FramesPerSecond;           // 0x01375550

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MinionFaceHero : CAIStateGroupBase {
    CBaseIntelligentPointer PHero;              // +0x14
    void Update();
};

void CAIStateGroup_MinionFaceHero::Update()
{
    unsigned long start = GFGetCurrentFrame();
    while (IS_THING_ALIVE(PHero)) {
        unsigned long fps = FramesPerSecond;
        if (GFGetCurrentFrame() - start > fps * 6)
            break;
        if (AIAction_LookAtThing(1, CIntelligentPointer<const CThing>(PHero), 0xf, 0.0027777f) != 2) {
            Terminate();
            return;
        }
        CTCPerceiveThing* perceive;
        if (GetCreature()->GetInterface(&perceive, 0x34) && perceive->IsPlayerExtendedLineOfSight()
            && IS_THING_ALIVE(PHero)) {
            CThing* creature = GetCreature();
            const C3DVector& hero = PHero.GetPItem()->GetPosition();
            const C3DVector& me = creature->GetPosition();
            float dx = me.X - hero.X;
            float dy = me.Y - hero.Y;
            float dz = me.Z - hero.Z;
            if (dx * dx + dy * dy + dz * dz < 4.84f) {
                Terminate();
                return;
            }
        }
        if (AIAction_DoNothing() != 4) {
            Terminate();
            return;
        }
    }
    Terminate();
}

// ---- test stubs ----
struct CTCBase { int dummy; };
struct CPerceiveTC : CTCBase { int los; int calls; };
static CPerceiveTC g_perceive;
bool CTCPerceiveThing::IsPlayerExtendedLineOfSight() { CPerceiveTC* p = (CPerceiveTC*)this; p->calls++; return p->los != 0; }
static C3DVector g_pos[2];
static CThing* g_heroThing; static CThing* g_creatureThing;
const C3DVector& CThing::GetPosition() { return this == g_heroThing ? g_pos[0] : g_pos[1]; }
static unsigned long g_frame, g_step;
unsigned long __fastcall GFGetCurrentFrame() { unsigned long f = g_frame; g_frame += g_step; return f; }
unsigned long FramesPerSecond = 30;
void CBaseIntelligentPointer::v00() { }
template<class T> void CIntelligentPointer<T>::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CAIStateGroupBase::~CAIStateGroupBase() { }
static int g_looks, g_lookIdx, g_lookRet[8], g_doIdx, g_doRet[8], g_term, g_killAfterDo;
static CThing* g_lookedAt;
long CAIStateGroupBase::AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed)
{
    g_looks++; g_lookedAt = thing.GetPItem();
    if (repeat != 1 || frames != 0xf || speed < 0.0027f || speed > 0.0028f) return -99;
    return g_lookRet[g_lookIdx++];
}
long CAIStateGroupBase::AIAction_DoNothing()
{
    long r = g_doRet[g_doIdx++];
    if (g_killAfterDo && g_doIdx == g_killAfterDo) g_heroThing->m_flags91 |= 1;
    return r;
}
void CAIStateGroupBase::Terminate() { g_term++; }

static CThing hero, creature;
static CAIBrain brain;
static CInterfaceMap::SEntry entries[3];
static int fails;
static void check(int ok, const char* what) { if (!ok) { fails++; printf("FAIL %s\n", what); } }
static void reset(int heroAlive, int heroDead)
{
    memset(&hero, 0, sizeof hero); memset(&creature, 0, sizeof creature);
    g_heroThing = &hero; g_creatureThing = &creature;
    if (heroDead) hero.m_flags91 = 1;
    hero.m_flags91 |= 0x80;
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x34; entries[1].component = &g_perceive;
    entries[2].type = 0x50; entries[2].component = 0;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3;
    g_perceive.los = 0; g_perceive.calls = 0;
    g_frame = 1000; g_step = 1; g_looks = g_lookIdx = g_doIdx = g_term = g_killAfterDo = 0; g_lookedAt = 0;
    memset(g_lookRet, 0, sizeof g_lookRet); memset(g_doRet, 0, sizeof g_doRet);
    g_pos[0].X = 0; g_pos[0].Y = 0; g_pos[0].Z = 0;
    g_pos[1].X = 10; g_pos[1].Y = 0; g_pos[1].Z = 0;
    (void)heroAlive;
}
static void run(int withHero)
{
    CAIStateGroup_MinionFaceHero sg;
    brain.PCreature = &creature;
    sg.PBrain = &brain;
    if (withHero) sg.PHero.SetPItem(&hero);
    sg.Update();
}
int main()
{
    // 1: no hero
    reset(1, 0); run(0); check(g_term == 1 && g_looks == 0, "nohero");
    // 2: dead hero
    reset(1, 1); run(1); check(g_term == 1 && g_looks == 0, "dead");
    // 3: timeout on the first pass (elapsed 181 > 180)
    reset(1, 0); g_step = 181; run(1); check(g_term == 1 && g_looks == 0, "timeout");
    // 3b: elapsed exactly 180 is not a timeout
    reset(1, 0); g_step = 180; g_lookRet[0] = 1; run(1); check(g_term == 1 && g_looks == 1, "edge");
    // 4: look fails
    reset(1, 0); g_lookRet[0] = 1; run(1); check(g_term == 1 && g_looks == 1 && g_doIdx == 0 && g_lookedAt == &hero, "lookfail");
    // 5: no perceive TC: idle twice then idle fails
    reset(1, 0); g_lookRet[0] = g_lookRet[1] = g_lookRet[2] = 2; g_doRet[0] = g_doRet[1] = 4; g_doRet[2] = 3;
    run(1); check(g_term == 1 && g_looks == 3 && g_doIdx == 3 && g_perceive.calls == 0, "noperceive");
    // 6: perceive, LOS, close -> stop before idling
    reset(1, 0); creature.m_presentBits[1] = 1UL << 20; g_perceive.los = 1; g_lookRet[0] = 2;
    g_pos[1].X = 1; g_pos[1].Y = 1; g_pos[1].Z = 1;   // 3 < 4.84
    run(1); check(g_term == 1 && g_looks == 1 && g_doIdx == 0 && g_perceive.calls == 1, "close");
    // 7: perceive, LOS, far -> idle (fails)
    reset(1, 0); creature.m_presentBits[1] = 1UL << 20; g_perceive.los = 1; g_lookRet[0] = 2; g_doRet[0] = 0;
    g_pos[1].X = 2; g_pos[1].Y = 1; g_pos[1].Z = 0;   // 5 >= 4.84
    run(1); check(g_term == 1 && g_doIdx == 1 && g_perceive.calls == 1, "far");
    // 8: perceive without LOS -> idle, then timeout exit on the second pass
    reset(1, 0); creature.m_presentBits[1] = 1UL << 20; g_lookRet[0] = 2; g_doRet[0] = 4; g_step = 100;
    run(1); check(g_term == 1 && g_looks == 1 && g_doIdx == 1 && g_perceive.calls == 1, "nolos");
    // 9: hero dies during the idle -> loop exit
    reset(1, 0); g_lookRet[0] = g_lookRet[1] = 2; g_doRet[0] = g_doRet[1] = 4; g_killAfterDo = 2;
    run(1); check(g_term == 1 && g_looks == 2 && g_doIdx == 2, "diesmid");
    // 10: perceive, LOS, zero distance -> stop before idling
    reset(1, 0); creature.m_presentBits[1] = 1UL << 20; g_perceive.los = 1; g_lookRet[0] = 2; g_doRet[0] = 0;
    g_pos[1].X = 0; g_pos[1].Y = 0; g_pos[1].Z = 0;
    run(1); check(g_term == 1 && g_doIdx == 0, "closezero");
    if (!fails) printf("009357f0_TEST PASS\n");
    return fails;
}