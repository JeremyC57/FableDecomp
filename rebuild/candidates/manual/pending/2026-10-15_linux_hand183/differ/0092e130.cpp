// Grade: DIFFER(229v229) -- register swap: retail keeps this in esi and the CTCSleep in edi (edi pushed after the HasTC test), ours the reverse | tried: nested-if form (235 bytes), early-return form (size fixed), GetInterface out-param
// CAIStateGroup_MinionSleep::IsRunnable() -- runnable straight away on the sleep component's fast track; otherwise only
// when the component wants sleep, the GoToBed base is runnable, the creature stands on the bed spot (2D distance
// <= 0.01) and its body/move target is at the bed orientation spot (2D distance <= 0.1).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E130, length 229 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC + entry lookup, type 0xD4 = CTCSleep, bit 20 of the dword
// at +0x38), CTCSleep::GetFastTrack @ 0x0092C3B0, CAIStateGroup_GoToBed::IsRunnable @ 0x0092D6F0 (qualified base call),
// the creature position getter @ 0x004C73D0, the creature's +0x60 object virtual slot 72 (+0x120, returns a block
// whose +0x0c is a C3DVector); 0.01f @ 0x01230A5C, 0.1f @ 0x01291BF4.
// Shape: `C2DVector(a.X - b.X, a.Y - b.Y).GetMagnitude()` keeps both components on the x87 stack (`fstp st(2) ;
// fstp st(0)` after the fsqrt); `<= k` gives `test ah,41h ; je`. The CTCSleep "wants sleep" flag is the bool at +0x0c.
// No usable engine header layout for the GoToBed part; BedPos (+0x3c) / BedFacingPos (+0x54) named descriptively.
#include <math.h>
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
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
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y); }
};

struct CTCSleep {
    unsigned char _pad[0xc];
    bool WantsToSleep;                          // +0x0c
    bool GetFastTrack();                        // 0x0092C3B0
};

struct CThingCreaturePos {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CMoveTarget {
    unsigned char _pad[0xc];
    C3DVector Pos;                              // +0x0c
};
struct CCreatureBody {
    virtual void v00();
    // slot 72 (+0x120)
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10();
    virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20();
    virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25();
    virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40();
    virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45();
    virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49(); virtual void s50();
    virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55();
    virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60();
    virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65();
    virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70();
    virtual void s71();
    virtual const CMoveTarget* GetMoveTarget();  // slot 72 (+0x120)
};
struct CThingWithBody {
    unsigned char _pad[0x60];
    CCreatureBody* PBody;                       // +0x60
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    unsigned char _pad_0x14[0x28];              // +0x14..+0x3b
    C3DVector BedPos;                           // +0x3c
    unsigned char _pad_0x48[0xc];               // +0x48..+0x53
    C3DVector BedFacingPos;                     // +0x54
    virtual bool IsRunnable();                  // 0x0092D6F0
};

struct CAIStateGroup_MinionSleep : CAIStateGroup_GoToBed {
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionSleep::IsRunnable()
{
    if (!PBrain->PCreature->HasTC(0xd4))
        return false;
    CTCSleep* sleep = (CTCSleep*)PBrain->PCreature->FindEntry(0xd4)->component;
    if (!sleep->GetFastTrack()) {
        if (!sleep->WantsToSleep)
            return false;
        if (!CAIStateGroup_GoToBed::IsRunnable())
            return false;
        const C3DVector& pos = ((CThingCreaturePos*)PBrain->PCreature)->GetPosition();
        if (!(C2DVector(BedPos.X - pos.X, BedPos.Y - pos.Y).GetMagnitude() <= 0.01f))
            return false;
        const C3DVector& target = ((CThingWithBody*)PBrain->PCreature)->PBody->GetMoveTarget()->Pos;
        if (!(C2DVector(BedFacingPos.X - target.X, BedFacingPos.Y - target.Y).GetMagnitude() <= 0.1f))
            return false;
    }
    return true;
}
