// CAIStateGroup_MinionReactToCrime::OnActivate() -- apply the "reporting crime to guard" opinion deed mask to the
// creature's opinion-of-hero component (when it has one) and remember that it was applied.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009587D0, length 107 bytes (+ int3 padding). Vtable slot 2.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x4F = CTCCreatureOpinionOfHero, bit
// 15 of the bitset dword at +0x28: `test ch,ch ; jns`), CCharString::CCharString(const char*, long) @ 0x0099EBF0
// with the literal "OPINION_DEED_MASK_REPORTING_CRIME_TO_GUARD" @ 0x01294E48 and -1, CTCCreatureOpinionOfHero::
// SetOpinionDeedMask(const CCharString&) @ 0x006822E0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: `DeedMaskApplied = false` first (scheduled into the creature loads), then `if (GetInterface(&p, 0x4f))
// { { CCharString mask("...", -1); p->SetOpinionDeedMask(mask); } DeedMaskApplied = true; }` -- the string is a
// NAMED local in an inner scope (its address is passed with `lea edx,[esp+8]`; a temporary reuses the
// constructor's eax) destroyed before the flag store. DeedMaskApplied is +0x1a (engine header name; the header
// flattens the base so the class is modelled locally, CCharString locally since only its ctor/dtor are called).
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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CTCCreatureOpinionOfHero {               // interface type 0x4F
    void SetOpinionDeedMask(const CCharString& mask);   // 0x006822E0
};

struct CAIStateGroup_MinionReactToCrime : CAIStateGroupBase {
    long OpinionDeedHandle;                     // +0x14
    bool GuardTold;                             // +0x18
    bool AllowDuplicateReactToCrime;            // +0x19
    bool DeedMaskApplied;                       // +0x1a
    virtual void OnActivate();
};

void CAIStateGroup_MinionReactToCrime::OnActivate()
{
    DeedMaskApplied = false;
    CTCCreatureOpinionOfHero* pOpinion;
    if (PBrain->PCreature->GetInterface(&pOpinion, 0x4f)) {
        {
            CCharString mask("OPINION_DEED_MASK_REPORTING_CRIME_TO_GUARD", -1);   // named: its address is formed with `lea`
            pOpinion->SetOpinionDeedMask(mask);
        }
        DeedMaskApplied = true;
    }
}