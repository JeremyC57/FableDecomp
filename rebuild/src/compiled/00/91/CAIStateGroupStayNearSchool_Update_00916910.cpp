// CAIStateGroup_StayNearSchool::Update() -- during school hours, keep walking back to within 15 units of the (alive)
// school while the kid's village school is open; terminate when any of that stops holding.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00916910, length 300 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the +0x170 school pointer), IsInSchoolHours @ 0x00916790,
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x23 = CTCVillager; the bitset test's bool materialised
// to a frame byte), GFGetPVillageInterface @ 0x007EE540 (x2), CTCVillage::IsCookTime @ 0x00658A35,
// AIAction_MoveToThingProximity @ 0x0097E380 (by-value CIntelligentPointer<const CThing> built in its slot, vtable
// 0x01237F1C), Terminate @ 0x008FCF40.
// Shape: `do { if (!valid) { Terminate(); return; } r = Move(...); } while (r == 4);` (the exits tail-merge with the
// outer Terminate); the move target is a named pointer read before the argument pushes.
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
    const C3DVector& GetPos() const;            // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb)
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
    // the ternary form (landed 0x00954690): the end/hit choice is ONE ternary, so the component lands straight in
    // the register of its use (ecx for a call on it); the miss path reads the never-written slot
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
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
    // the KidCleanup form (landed 0x00915190): `if (mask) *pp = Lookup();` with an entry-value lookup
    __forceinline CTCBase* LookupValue(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap::SEntry* p = m_interfaces.LowerBound(&type);
        if (p == m_interfaces.last) p = m_interfaces.last;
        else if (p->type > interfaceType) p = m_interfaces.last;
        return p->component;
    }
    template<class T> __forceinline void GetInterfaceK(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            *ppOut = (T*)LookupValue(interfaceType);
    }
    // GetInterface<T>() returning the component: hit = the entry's component, miss = the never-written local
    template<class T> __forceinline T* PeekInterface(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = (T*)FindEntry(interfaceType)->component;
        return component;
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
public:
    void Terminate();   // 0x008FCF40
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DVector { public: float x, y, z; };
class C2DVector { public:
    float x, y;
    float GetLengthSquared() const { return x * x + y * y; }
};
struct CRefCounted {                            // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
class CMaterialDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    CRefCounted Ref;                            // +0x04
    unsigned char _pad08[0x3c - 0x08];
    unsigned long Flags;                        // +0x3c (bit 2: a school building)
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T>
class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
class CThingBuilding : public CThing { public:
    CDefPointer<CMaterialDef> PeekPMaterial();  // 0x0082D2D0
};
struct CThingPos : CThing {
};
struct CTCVillager;
class CTCVillage;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);  // 0x007EE540
struct CThingSchoolKid : CThing {
    unsigned char _pad94[0x170 - 0x94];
    CBaseIntelligentPointer PSchool;            // +0x170 CIntelligentPointer<CThingBuilding>
};
// the kid is within 20 units (XY) of the school: the delta kept on the FPU, squared length against 400
__forceinline bool IsNearSchool(CThingBuilding* school, CThingPos* kid)
{
    const C3DVector& schoolPos = school->GetPos();
    const C3DVector& kidPos = kid->GetPos();
    C2DVector delta;
    delta.x = kidPos.x - schoolPos.x;
    delta.y = kidPos.y - schoolPos.y;
    return delta.GetLengthSquared() < 400.0f;
}
struct CAIStateGroup_StayNearSchool : CAIStateGroupBase {
    CThingSchoolKid* GetCreature() const { return (CThingSchoolKid*)PBrain->PCreature; }
    bool IsInSchoolHours();                     // 0x00916790
    bool IsRunnable();
    virtual void Update();
};
struct CTCVillageS {
    bool IsCookTime();                          // 0x00658A35 (manifest name; the village's school/cook hours test)
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
struct CAIStateGroup_StayNearSchoolU : CAIStateGroup_StayNearSchool {
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
};

void CAIStateGroup_StayNearSchool::Update()
{
    CAIStateGroup_StayNearSchoolU* self = (CAIStateGroup_StayNearSchoolU*)this;
    CThing* school = GetCreature()->PSchool.GetPItem();
    if (school && !(school->m_flags91 & 1) && IsInSchoolHours()) {
        long result;
        do {
            CThing* current = GetCreature()->PSchool.GetPItem();
            CTCVillager* villager;
            if (!current || (current->m_flags91 & 1) || !GetCreature()->GetInterfaceT(&villager, 0x23)
                    || !GFGetPVillageInterface(villager)
                    || !((CTCVillageS*)GFGetPVillageInterface(villager))->IsCookTime()) {
                self->Terminate();
                return;
            }
            CThing* target = GetCreature()->PSchool.GetPItem();
            bool reached;
            result = self->AIAction_MoveToThingProximity(2, target, 15.0f, &reached, 3, true, false, true, true, 0.0f);
        } while (result == 4);
        return;
    }
    self->Terminate();
}