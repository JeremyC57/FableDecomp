// CTCProjectileTrap::UpdateTrap() -- per-frame trap logic: target the hero when the current target is gone, turn the trap's look component toward the target, then count the idle/shot timers down and fire (or go idle after a full round)
// Leaf: UpdateTrap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00787E12, 279 bytes (plain `ret`). CTCProjectileTrap vtable 0x0126B90C slot 29. The trap
//   TU is size-optimised with a frame pointer: `optimize("y", off)` + `("s", on)` (`push ebp ; mov ebp,esp`, `and [esi+68h],0`,
//   `leave`), as the landed TriggerTrap (0x00787CA5) and CTCMeleeTrap::UpdateAnimation (0x00788555).
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`, twice), CTCBase::GetPlayerManager @ 0x00686D40,
//   CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, IS_THING_ALIVE<CThing*> @ 0x0040DCE0
//   (`__fastcall`, by const reference), CIntelligentPointer(CThing*) @ 0x004A96F0, CIntelligentPointer::operator= @ 0x004A9760,
//   ~CBaseIntelligentPointer @ 0x00A01C10, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020 (interface
//   0x43 = CTCLook), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, CTCLook::LookAtPos(const
//   C3DVector&, long) @ 0x00753E90, __ftol2 @ 0x00BFEA70, virtual TriggerTrap (slot 30, +0x78). g_framesPerSecond @ 0x01375550.
// Layout: Ego_r.pdb CTCTrapBase with 12-byte retail vectors (PTrapDef +0x54, Active +0x58, Triggered +0x59), CTCProjectileTrap
//   ShotTimer +0x60, IdleTimer +0x64, NumShotsThisRound +0x68, PTarget +0x6C (PDB -0xC); CTrapDef NumShotsToFireBeforeIdle +0x58,
//   SecondsIdle +0x5C (def bodies sit 4 bytes low).
// Shape: the hero is a named local passed by const reference (frame word [ebp-4]); the assignment goes through the implicit
//   conversion temporary; the look component is HasTC + the end/hit ternary lookup.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
struct C3DVector;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
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
struct CComponentBitset {                               // std::bitset<274>
    unsigned long m_words[9];
    bool test(long bit);                                // 0x00410DE0 (std::bitset::test)
};
class CThing {
public:
    unsigned char    _pad00[0x20];
    CComponentBitset UsingComponents;                   // +0x20
    CInterfaceMap    m_interfaces;                      // +0x44
    const C3DVector& GetPosition();                     // 0x004C73D0
};
struct CTCLook {
    void LookAtPos(const C3DVector& position, long priority);   // 0x00753E90
};
void LookAtSink_s7(CTCLook* look, float x, long priority);   // stand-in: the look update (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for CTCLook::LookAtPos (0x00753E90): reads the position through its const reference, lets nothing escape.
__declspec(noinline) void CTCLook::LookAtPos(const C3DVector& position, long priority)
{
    LookAtSink_s7(this, *(const float*)&position, priority);
}
#pragma auto_inline(on)
struct CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    CThing* GetPItem() const;                           // 0x00A01B50
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CIntelligentPointer_CThing(CThing* thing);          // 0x004A96F0
    CIntelligentPointer_CThing& operator=(const CIntelligentPointer_CThing& other);   // 0x004A9760
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer_CThing* pointer);   // 0x004CC340
bool PointerAliveSink_s7(CThing* item);   // stand-in: the dead-flag test
#pragma auto_inline(off)
// Same-TU body of IS_THING_ALIVE<CIntelligentPointer<CThing>> (0x004CC340, __fastcall): reads the pointer only.
__declspec(noinline) bool __fastcall IS_THING_ALIVE(const CIntelligentPointer_CThing* pointer)
{
    CThing* item = pointer->GetPItem();
    if (item == 0)
        return false;
    return PointerAliveSink_s7(item);
}
#pragma auto_inline(on)
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
#pragma auto_inline(off)
// Same-TU body of IS_THING_ALIVE<CThing*> (0x0040DCE0, __fastcall): reads the pointer through its const reference only.
__declspec(noinline) bool __fastcall IS_THING_ALIVE(CThing* const& thing)
{
    if (thing == 0)
        return false;
    return PointerAliveSink_s7(thing);
}
#pragma auto_inline(on)
extern long g_framesPerSecond;                          // 0x01375550
struct CTrapDef {
    unsigned char _pad00[0x58];
    long  NumShotsToFireBeforeIdle;                     // +0x58 (PDB +0x5c)
    float SecondsIdle;                                  // +0x5c (PDB +0x60)
};
struct CTCTrapBase {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    const CTrapDef* PTrapDef;                           // +0x54
    bool          Active;                               // +0x58
    bool          Triggered;                            // +0x59
    bool          LastTriggerOnProximity;               // +0x5a
    long          TrapType;                             // +0x5c ETrapType
    CPlayerManager* GetPlayerManager();                 // 0x00686D40 (CTCBase)
};
struct CTCProjectileTrap : CTCTrapBase {
    long ShotTimer;                                     // +0x60
    long IdleTimer;                                     // +0x64
    long NumShotsThisRound;                             // +0x68
    CIntelligentPointer_CThing PTarget;                 // +0x6c
    void UpdateTrap();
    void TriggerTrap();
};
struct CTCProjectileTrapV {                             // vtable view: slot 30 TriggerTrap
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void UpdateTrap();
    virtual void TriggerTrap();
};

void CTCProjectileTrap::UpdateTrap()
{
    if (!Triggered || !Active)
        return;
    if (!IS_THING_ALIVE(&PTarget)) {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
        if (IS_THING_ALIVE(hero))
            PTarget = hero;
    }
    if (!IS_THING_ALIVE(&PTarget))
        return;
    CThing* thing = ParentThing;
    if (thing->UsingComponents.test(0x43)) {
        long key = 0x43;
        CInterfaceMap* map = &thing->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CTCLook* look = (CTCLook*)((it == map->last || 0x43 < it->type) ? map->last : it)->component;
        look->LookAtPos(PTarget.GetPItem()->GetPosition(), 0x17);
    }
    if (IdleTimer > 0) {
        --IdleTimer;
    } else {
        if (ShotTimer > 0)
            --ShotTimer;
        if (ShotTimer == 0) {
            if (NumShotsThisRound >= PTrapDef->NumShotsToFireBeforeIdle) {
                IdleTimer = (long)(g_framesPerSecond * PTrapDef->SecondsIdle);
                NumShotsThisRound = 0;
            } else {
                ((CTCProjectileTrapV*)this)->TriggerTrap();
            }
        }
    }
}