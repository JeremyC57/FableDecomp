// CAIStateGroup_MinionGuardFollowPlayer::Update() -- re-arm the one-second timer; while the hero is alive and the
// Grade: DIFFER(299v299) IMPROVED (66 -> 64 difflines): declaring bool reached at the top of the function (before oneSecond) puts the materialised bitset byte at [esp+0xe] and reached at +0xf as retail. Left: esi/edi colouring (retail this=esi, frame rate/villager=edi). Tried: reached declared late (=), frame-rate local inlined (297v299). Next: GetInterfaceT shared CThing header form.
// village has no live guard-follower, become it, walk up to the hero (3 units) and look at it, then terminate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090C540, length 299 bytes (+ int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870, CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::GetCharacterThing
// @ 0x00487DC0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x23 = CTCVillager), GFGetPVillageInterface
// @ 0x007EE540 (x2), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the village's +0xbd guard follower),
// CVillage::SetGuardFollower @ 0x0090C780, AIAction_MoveToThingProximity @ 0x0097E380, AIAction_LookAtThing
// @ 0x0097D6F0 (by-value CIntelligentPointer<const CThing> built in the argument slots, vtable 0x01237F1C),
// CAIStateGroupBase::Terminate @ 0x008FCF40. Global: the frame-rate long @ 0x01375550 (read before the frame call).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

struct CThing {
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
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CAIStateGroupBase {
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
struct CPlayer {
    CThing* GetCharacterThing();                // 0x00487DC0
};
struct CWorldP {
    CPlayer* GetCurrentPlayer();                // 0x0049D9D0
};
struct CGameGlobals {
    unsigned char _pad[0x24];
    CWorldP* m_pWorld;                          // +0x24
};
extern CGameGlobals* g_pMainGameComponent;      // 0x013B86A0
struct CTCVillager;
#pragma pack(push, 1)
struct CVillage {
    unsigned char _pad[0xbd];
    CBaseIntelligentPointer PGuardFollower;     // +0xbd (packed)
    void SetGuardFollower(CThing* guard);       // 0x0090C780
};
#pragma pack(pop)
CVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};

struct CAIStateGroup_MinionGuardFollowPlayer : CAIStateGroupBase {
    unsigned char _pad14[4];
    unsigned long NextCheckFrame;               // +0x18
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing target, long frames, float speed);  // 0x0097D6F0
    void Terminate();                           // 0x008FCF40
    virtual void Update();
};

void CAIStateGroup_MinionGuardFollowPlayer::Update()
{
    bool reached;
    long oneSecond = g_framesPerSecond;
    NextCheckFrame = GFGetCurrentFrame() + oneSecond;
    CThing* hero = g_pMainGameComponent->m_pWorld->GetCurrentPlayer()->GetCharacterThing();
    if (hero && !(hero->m_flags91 & 1)) {
        CTCVillager* villager;
        PBrain->PCreature->GetInterfaceT(&villager, 0x23);
        CVillage* village = GFGetPVillageInterface(villager);
        CThing* follower = village->PGuardFollower.GetPItem();
        if (follower && !(follower->m_flags91 & 1))
            return;
        GFGetPVillageInterface(villager)->SetGuardFollower(PBrain->PCreature);
        AIAction_MoveToThingProximity(1, hero, 3.0f, &reached, 3, true, false, true, true, 0.0f);
        AIAction_LookAtThing(1, hero, 0xb, 0.0027777f);
    }
    Terminate();
}
