// CAIStateGroup_BanditKingLunge::CanLungeAtPlayer(CThing* player) -- can the bandit king lunge now: a live player, a boss component, and either the phase's hit count reached or (phases without one) the lunge interval elapsed
// Retail: Fable.exe (TLC, VC7.1) @ 0x009613A0, 248 bytes (`ret 4`). Ego_r.pdb: ?CanLungeAtPlayer@CAIStateGroup_BanditKingLunge@@IAE_NPAVCThing@@@Z.
// Callees: CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, 0xad = bit 13 of +0x34), CTCBoss::GetBossPhase
// 0x007AED10 and the phase hit counter 0x007AED40 (both labelled "CWorld::DrawGetEnvironment": propagated), the brain
// world accessor 0x008FCFC0, the static frame counter 0x0049D870, __ftol2 0x00BFEA70; frames-per-second global 0x01375550.
// Shape: the landed AI state-group model (0x00950A30): the out pointer is the inlined GetInterface (its type local in
// the dead `player` slot); the phase index is Dinkumware `min(phase, lastPhase)` by reference (`lea ecx` select);
// a zero hit target falls back to the frame timer (first call records the frame), else `return hits >= target;`.
// Layout: CAIStateGroup_BanditKingLunge.h (TimeOfLastLunge +0x1c); the boss def's per-phase vector at +0x40, the
// lunge interval at +0x50 (seconds, scaled by the frame rate).
// Leaf: CanLungeAtPlayer@CAIStateGroup_BanditKingLunge
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
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

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


template<class T> struct CRetailVector {         // {first, last, end}
    T* first;
    T* last;
    T* end;
    unsigned long size() const { return last - first; }
    const T& operator[](unsigned long i) const { return first[i]; }
};
struct CTCBossDef {
    unsigned char _pad[0x40];
    CRetailVector<long> PhaseHits;              // +0x40 per boss phase
    unsigned char _pad4c[4];
    float LungeIntervalSeconds;                 // +0x50
};
struct CTCBoss {
    unsigned char _pad[0x0c];
    CTCBossDef* Def;                            // +0x0c
    long GetBossPhase() const;                  // 0x007AED10
    long GetHitsThisPhase() const;              // 0x007AED40
};

namespace std {
template<class T> inline const T& min(const T& left, const T& right) { return right < left ? right : left; }
}

extern long g_framesPerSecond;                  // 0x01375550
struct CWorldFrame {
    static long GetWorldFrame();                // 0x0049D870 (static: GetPWorld() is evaluated for nothing)
};

struct CAIStateGroup_BanditKingLunge : CAIStateGroupBase {
    unsigned char Player[8];                    // +0x14 CIntelligentPointer<CThingPlayerCreature>
    long TimeOfLastLunge;                       // +0x1c
    bool CanLungeAtPlayer(CThing* player);
};

bool CAIStateGroup_BanditKingLunge::CanLungeAtPlayer(CThing* player)
{
    CTCBoss* boss;
    if (player && !(player->m_flags91 & 1) && PBrain->PCreature->GetInterface(&boss, 0xad)) {
        long hits = boss->Def->PhaseHits[std::min(boss->GetBossPhase(), (long)boss->Def->PhaseHits.size() - 1)];
        if (hits == 0) {
            if (TimeOfLastLunge) {
                long interval = (long)(g_framesPerSecond * boss->Def->LungeIntervalSeconds);
                if (((CWorldFrame*)GetPWorld())->GetWorldFrame() - TimeOfLastLunge > interval)
                    return true;
            }
            else
                TimeOfLastLunge = ((CWorldFrame*)GetPWorld())->GetWorldFrame();
        }
        else
            return boss->GetHitsThisPhase() >= hits;
    }
    return false;
}