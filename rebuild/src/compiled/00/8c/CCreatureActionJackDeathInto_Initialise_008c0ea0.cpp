// CCreatureAction_JackDeathInto::Initialise() -- base Initialise; swap creature mode 0x34 for 0x35; spawn the creature's DEATH special effect at its position when its special-effects def maps one
// Leaf: ?Initialise@CCreatureAction_JackDeathInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C0EA0, 268 bytes (272 with padding), virtual Initialise.
// Callees: CCombatActionBase::Initialise 0x00857520 (direct, first), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::
//   LowerBound 0x0040F020 (inlined lookup of 0x31 = CTCCreatureModeManager, bit 17 of +0x24), CTCCreatureModeManager::
//   IsModeActive 0x0073A8A0 / RemoveMode 0x0073ABA0 / AddMode 0x0073AE10, CDefClassBase::GetSubDef<CSpecialEffectsDef>
//   0x00669580, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("DEATH" 0x01277C9C), CCRC::CalcAndLookup(const
//   CCharString&, long*) 0x00669D30 (on the def), CThing::GetPosition 0x004C73D0, the special-effect Create(long, const
//   C3DVector*, bool) 0x006E0880 (__fastcall).
// Shape: the mode manager is a zero-initialised out variable of the landed out-parameter GetInterface (one GetPItem, the
//   `xor esi,esi` preload; the result is not tested); the thing's def is
//   a by-value CDefPointer copy (addref, GetSubDef, release in the full expression); the sub-def and the looked-up
//   effect id are zero-initialised locals of their own `{}` block after the mode code (so the inlined lookup key shares
//   the effect id's frame word, not the sub-def's); the name string is a named local in its own block and shares
//   the sub-def's frame word (the def stays in esi) -- which needs the same-TU GetSubDef stand-in below (retail's LTCG
//   knowledge that the def pointer's address does not escape).
class C3DVector { public: float x, y, z; };
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30 (CCRC, at the def)
};
template<class T> class CDefPointer { public:
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
class CDefinitionManager { public:
    CDefPointer<CSpecialEffectsDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
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
class CThing { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
    bool AddMode(unsigned long mode);                 // 0x0073AE10
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};
void __fastcall CreateSpecialEffect(long effect, const C3DVector* position, bool attached);   // 0x006E0880

class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void Initialise();                                // 0x00857520
};
class CCreatureAction_JackDeathInto : public CCombatActionBase { public:
    void Initialise();
};

void CCreatureAction_JackDeathInto::Initialise()
{
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modes = 0;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    if (modes->IsModeActive(0x34))
        modes->RemoveMode(0x34);
    modes->AddMode(0x35);
{
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    PCreature.GetPItem()->GetDef()->GetSubDef(&effects);
    if (effects.Object) {
        bool found;
        {
            CCharString name("DEATH");
            found = effects->CalcAndLookup(name, &effect);
        }
        if (found)
            CreateSpecialEffect(effect, &PCreature.GetPItem()->GetPosition(), false);
    }
    }
}