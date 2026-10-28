// CTCDummyVillager::SetupFromData(unsigned long defType, CThingBuilding* home, CThingBuilding* work, bool respawnable) -- record the def type and the home / work buildings; a respawnable dummy is due back tomorrow, or (when its def has a creature def) after a random wait between the creature's RespawnWaitInDays min and max from today; otherwise never
// Leaf: SetupFromData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072F170, 384 bytes (`ret 10h`). Ego_r.pdb
// `?SetupFromData@CTCDummyVillager@@QAEXKPAVCThingBuilding@@0_N@Z`.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 (then the CIntelligentPointer<CThingBuilding> vtable 0x01244344), SetPItem
// @ 0x00A01B90, GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10, CTCBase::GetWorld @ 0x00686A70, CTCBase::
// GetDefinitions @ 0x006869D0, CDefinitionManager::GetDef<T> @ 0x0042AF3C, CDefClassBase::GetSubDef<CCreatureDef> @ 0x00485815,
// CTCBase::GetPRandomSeed @ 0x00686DA0, GFROR13 @ 0x00497890 (__fastcall).
// Layout: Ego_r.pdb CTCDummyVillager (as the landed SetupFromVillager 0x0072F2F0): DefType +0x0c, HomeBuilding +0x10,
// WorkBuilding +0x20, DayToBeRecreated +0x40. CWorld +0x1c the game clock (day +0x14). CCreatureDef RespawnWaitInDaysMin / Max
// at +0xb8 / +0xbc (Ego_r.pdb +0xc8 / +0xcc less 0x10: four retail 12-byte vectors before them).
// Shape: the building pointers are assigned from temporaries (as SetupFromVillager); the UIDs are modelled as two words (an
// __int64 member pads the vptr to 8); the random wait is the inline LCG + GFROR13 with a zero-range guard.
class CBaseObject;
class CThingBuilding;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CBaseObject* item = other.CBaseIntelligentPointer::GetPItem();
        SetPItem(item);
        return *this;
    }
};

template<class T> class CDefPointer;
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long RefCount;                                  // +0x04
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 for CCreatureDef
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class CCreatureDef : public CDefClassBase {
public:
    unsigned char _pad08[0xb8 - 0x08];
    long RespawnWaitInDaysMin;                              // +0xb8
    long RespawnWaitInDaysMax;                              // +0xbc
};
class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C
};
class CGameDefinitionManager : public CDefinitionManager
{
};
void* FindDef_0072f170(long index);
void* FindSubDef_0072f170(const void* def);
#pragma auto_inline(off)
// Same-TU stand-in bodies for the two def lookups (retail instantiates them elsewhere; with their bodies visible VC7.1 knows the
// out pointers do not escape, so the creature def stays in ebx and the releases are `dec [reg+4]`).
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    out->Object = (const T*)FindDef_0072f170(index);
    return out->Object != 0;
}
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    out->Object = (const T*)FindSubDef_0072f170(this);
    return out->Object != 0;
}
#pragma auto_inline(on)
long __fastcall GFROR13(unsigned long);     // 0x00497890
__forceinline unsigned long GFRandomBelow(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
struct CGameClock {
    unsigned char _pad00[0x14];
    long Day;                                               // +0x14
};
class CWorld { public:
    unsigned char _pad00[0x1c];
    CGameClock* Clock;                                      // +0x1c
};

class CTCBase
{
public:
    virtual void v00();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    unsigned long* GetPWorldSeed() const;   // 0x00686DA0
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCDummyVillager : public CTCBase { public:
    void*                                 ParentThing;      // +0x04 (CTCBase)
    unsigned char                         _pad08[0x4];
    unsigned long                         DefType;          // +0x0c
    CIntelligentPointer<CThingBuilding>   HomeBuilding;     // +0x10
    unsigned long                         HomeBuildingUID[2];// +0x18
    CIntelligentPointer<CThingBuilding>   WorkBuilding;     // +0x20
    unsigned long                         WorkBuildingUID[2];// +0x28
    unsigned char                         Creature[8];      // +0x30
    unsigned long                         CreatureUID[2];    // +0x38
    long                                  DayToBeRecreated; // +0x40
    void SetupFromData(unsigned long defType, CThingBuilding* home, CThingBuilding* work, bool respawnable);
};

void CTCDummyVillager::SetupFromData(unsigned long defType, CThingBuilding* home, CThingBuilding* work, bool respawnable)
{
    DefType = defType;
    HomeBuilding = CIntelligentPointer<CThingBuilding>(home);
    WorkBuilding = CIntelligentPointer<CThingBuilding>(work);
    if (respawnable) {
        DayToBeRecreated = GetWorld().Clock->Day + 1;
        CDefPointer<const CDefClassBase> def;
        GetDefinitionManager().GetDef(DefType, &def);
        CDefPointer<const CCreatureDef> creatureDef;
        if (def->GetSubDef(&creatureDef)) {
            long today = GetWorld().Clock->Day;
            DayToBeRecreated = creatureDef->RespawnWaitInDaysMin
                + GFRandomBelow(creatureDef->RespawnWaitInDaysMax - creatureDef->RespawnWaitInDaysMin, GetPWorldSeed()) + today;
        }
    } else
        DayToBeRecreated = 0x7fffffff;
}