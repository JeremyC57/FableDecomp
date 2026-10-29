// CAIStateGroup_Thunder::Update() -- mark the target in danger, and in the idle combat state cast the lightning storm when the target is within the ability range
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096C4F0, 495 bytes.
// Shape: CDefinitionManager::GetDef(long, CDefPointer*) 0x008E31D0 has a same-TU stand-in body that writes the out slot
//   only (the def is then loaded once into esi and reused for the range test and the release), the CVectorMap LowerBound
//   instantiation 0x0040F020 has its same-TU binary-search body, and the static accessor CGameDefinitionManager::Get
//   0x0044C6B0 is `__fastcall` as in the landed headers (that convention puts the name temporary's `lea` after `push 1`).
class CTCBase;
class C3DVector { public: float x, y, z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
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
};
#pragma pack(push, 1)
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
#pragma pack(pop)
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CSpecialAbilityDef {
    virtual void v00();
    virtual void Destroy();
    long m_refs;                                // +0x04
    unsigned char _pad[0x44 - 8];
    float Range;                                // +0x44
};
struct CDefPointer {
    CSpecialAbilityDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (--m_p->m_refs == 0) m_p->Destroy(); }
    CSpecialAbilityDef* operator->() const { return m_p; }
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();       // 0x0044C6B0
    long GetDefGlobalIndex(const CCharString& name, bool b);   // 0x009AD390
    bool GetDef(long index, CDefPointer* out);  // 0x008E31D0
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThingCreatureBase;
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(CThing* creature, long level);   // 0x00747B80
};
struct CTCThunderCombat {                       // interface 0xad
    long GetState();                            // 0x007AED10
    void Reset();                               // 0x007AED50
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0x110];
};
class CCreatureAction_ThunderLightningStorm : public CCreatureActionBase { public:
    CCreatureAction_ThunderLightningStorm(CThingCreatureBase* creature);   // 0x0096C6E0
    ~CCreatureAction_ThunderLightningStorm();   // 0x0062E900
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
};
__forceinline float Distance2D(CThing* target, CThing* me)
{
    const C3DVector& from = me->GetPos();
    const C3DVector& to = target->GetPos();
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    return (float)sqrt(dx * dx + dy * dy);
}
#pragma pack(push, 1)
struct CAIStateGroup_Thunder : CAIStateGroupBase {
    CBaseIntelligentPointer PArenaCentre;       // +0x14
    bool bIsRangeAttack;                        // +0x1c
    CBaseIntelligentPointer PTargetThing;       // +0x1d
    unsigned char _pad_0x25[3];
    long HitCount;                              // +0x28
    virtual void Update();
};
#pragma pack(pop)

void CAIStateGroup_Thunder::Update()
{
    CTCThunderCombat* combat;
    PBrain->PCreature->GetInterface(&combat, 0xad);
    if (IS_THING_ALIVE(PTargetThing)) {
        CTCStealth* stealth;
        if (((CThing*)PTargetThing.GetPItem())->GetInterface(&stealth, 0x36))
            stealth->SetAsInDanger(PBrain->PCreature, 3);
    }
    switch (combat->GetState()) {
    case 0:
        if (IS_THING_ALIVE(PTargetThing)) {
            long index = CGameDefinitionManager::Get().GetDefGlobalIndex("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", true);
            CDefPointer def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            float dist = Distance2D(((CThing*)PTargetThing.GetPItem()), PBrain->PCreature);
            if (dist < def->Range && dist > 1.0f) {
                long result = AIAction_PerformActionAndWait(&CCreatureAction_ThunderLightningStorm(GetCreatureBase()), false);
                if (result != 0) {
                    HitCount = 0;
                    combat->Reset();
                }
            }
        }
        Terminate();
        break;
    case 1:
        Terminate();
        break;
    }
}