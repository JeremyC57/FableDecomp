// CCreatureAction_OnGenerate::Initialise() -- the GENERATE_DROP animation, base Initialise, then spawn the creature's GENERATE special effect at its position when its special-effects def maps one
// Leaf: Initialise
// Grade: DIFFER(269v262) -- same gap as 008c9fe0: retail caches the sub-def pointer in ebp right after GetSubDef (push ebp, ebp also the zero register); ours re-reads it from the frame word; rest identical
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B74D0, 262 bytes (272 with padding), virtual Initialise.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("GENERATE_DROP" 0x0127A5AC / "GENERATE" 0x0127A5A0), CCombatActionBase::SetAnimationName
//   0x008574D0, CCombatActionBase::Initialise 0x00857520, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup 0x00669D30, CThing::GetPosition
//   0x004C73D0, the special-effect Create 0x006E0880 (__fastcall).
// Shape: the JackDeathInto 0x008C0EA0 model with the lookup in one `&&` expression: the thing's def is a by-value
//   CDefPointer temporary and the name an implicit conversion, both destroyed at the end of the full expression under
//   VC's construction flags (ebx bits 1 and 2); the found flag lives in a frame byte; the GetSubDef same-TU stand-in.
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30 (CCRC, at the def)
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointer<CSpecialEffectsDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void Destroy(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x1c - 8];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CSpecialEffectsDef> 0x00669580 (its landed body): VC7.1 then knows the sub-def pointer's
// address does not escape, so the def is held in esi and the name string reuses its frame word, as in retail.
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    long* pIndex;
    {
        CCharString name("CSpecialEffectsDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CSpecialEffectsDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    const C3DVector& GetPosition() const;             // 0x004C73D0
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
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
    CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(m_pDef); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};

void __fastcall CreateSpecialEffect(long effect, const C3DVector* position, bool attached);   // 0x006E0880

struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
struct CCreatureAction_OnGenerate : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_OnGenerate::Initialise()
{
    {
        CCharString name("GENERATE_DROP");
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    bool found = PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects->CalcAndLookup("GENERATE", &effect);
    if (found)
        CreateSpecialEffect(effect, &PCreature.GetPItem()->GetPosition(), false);
}
