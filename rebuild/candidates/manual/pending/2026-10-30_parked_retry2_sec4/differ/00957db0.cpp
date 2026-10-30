// CAIStateGroup_MinionBehaviourCrowdBase::Update() -- walk to the crowd position while the crowd component stays
// Grade: DIFFER(266v266) IMPROVED (92 -> 52 difflines): the in-loop exit 'weight == 0 || !IsInCrowd() || !active' written as three exits, each with its own Terminate()/return, gives retail's block placement (the 00979F70 lever). Left: this/component esi/edi swap (retail this=edi pushed in the prologue). Tried before: if(result==2) form, creature local dropped, for-loop with breaks. Next: the colouring.
// active; once there, look at the crowd focus thing until done and hand over to the class hook (slot 15); any
// other outcome terminates the group.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957DB0, length 266 bytes. Vtable slot 12 of
// CAIStateGroup_MinionBehaviourCrowdBase (0x0128C494).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x34 = bit 20 of +0x24),
// CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (C3DVector by value built in its argument slot through a user
// copy constructor), the component test @ 0x00744430 (manifest "CTCInventoryBase::GetCanItemBeQuickAssigned" is
// propagated), CAIStateGroupBase::Terminate @ 0x008FCF40, AIAction_LookAtThing @ 0x0097D6F0 (by-value
// CIntelligentPointer<const CThing> built in its slot: ctor 0x00A01B10, vtable 0x01237F1C, SetPItem 0x00A01B90),
// own virtual slot 15 (+0x3c).
// Shape: the bitset test's bool result IS the `reached` out-flag passed by reference to MoveToPos (retail spills the
// bit test to the byte the call later gets the address of); two `do { r = Action(); ... } while (r == 4);` yield
// loops, the shared Terminate tail merged from the invalid-component and not-succeeded exits.
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
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { x = v.x; y = v.y; z = v.z; }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CTCCrowdMember {                         // interface 0x34
    unsigned char _pad[0x18];
    float m_weight;                             // +0x18
    unsigned char _pad1c[0x2d - 0x1c];
    bool m_active;                              // +0x2d
    bool IsInCrowd();                           // 0x00744430
};

struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase {
    CBaseIntelligentPointer PFocus;             // +0x14 CIntelligentPointer<CThing>
    unsigned char _pad1c[0x2c - 0x1c];
    C3DVector CrowdPos;                         // +0x2c
    bool m_inCrowd;                             // +0x38
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool& reached, long movement, bool a, bool b,
        const CThing* avoid, bool c, float speed);                         // 0x0097EC00
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing target, long frames, float speed);  // 0x0097D6F0
    void Terminate();                           // 0x008FCF40
    virtual void Update();
};
struct CAIStateGroup_MinionBehaviourCrowdBaseV {
    V4(a) V4(b) V4(c)
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual void OnReachedCrowd();              // slot 15 (+0x3c)
};

void CAIStateGroup_MinionBehaviourCrowdBase::Update()
{
    CThing* creature = PBrain->PCreature;
    bool reached = creature->TestTC(0x34);
    CTCCrowdMember* member;
    if (reached)
        member = (CTCCrowdMember*)creature->FindEntry(0x34)->component;
    long result;
    do {
        result = AIAction_MoveToPos(2, CrowdPos, reached, 3, true, true, 0, true, 0.0f);
        if (member->m_weight == 0.0f) {
            Terminate();
            return;
        }
        if (!member->IsInCrowd()) {
            Terminate();
            return;
        }
        if (!member->m_active) {
            Terminate();
            return;
        }
    } while (result == 4);
    if (result != 2) {
        Terminate();
        return;
    }
    do {
        result = AIAction_LookAtThing(2, PFocus, 0x11, 0.01f);
    } while (result == 4);
    if (result == 2)
        ((CAIStateGroup_MinionBehaviourCrowdBaseV*)this)->OnReachedCrowd();
}
