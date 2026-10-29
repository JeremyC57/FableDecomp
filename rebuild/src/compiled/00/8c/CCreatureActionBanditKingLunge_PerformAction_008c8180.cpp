// CCreatureAction_BanditKingLunge::PerformAction() -- spawn THING_EXPLOSION_TWINBLADE_POUND 1.5 units ahead of the creature (owner = creature), then create its STUCK special effect at the creature and attach it through the particle emitter
// Leaf: ?PerformAction@CCreatureAction_BanditKingLunge
// Shape: CalcAndLookup (0x00669D30) gets a same-TU out-value-only stand-in body (the 0x008D9740 idiom): the effect index
//   stops escaping, so the GetInterface(0x4B) key shares the dead effect word (+0x1c) as retail.
// Layout: CThing bitset +0x20, map +0x44, PTCDriver +0x5c, physics +0x60, def +0x70, dead flag +0x91 bit 0 (as 0x8C8380/0x8F5060).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CTCBase;
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x008C4170 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
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

struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;              // slot 70 (+0x118)
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x5c - 0x4c];
    CTCBase* PTCDriver;                               // +0x5c
    CPhysicsBody* PhysicsBody;                        // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
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
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};

class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CTCDExplosion { public:
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);   // 0x0080C460
};
struct CTCDriver41 {
    void SetOwner(CThing* owner);                     // 0x0080CBF0
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    CThing* GetCreature() const { return ((CThing*)PCreature.GetPItem()); }
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
};
class CCreatureAction_BanditKingLunge : public CCreatureActionBase { public:
    void PerformAction();
};

void CCreatureAction_BanditKingLunge::PerformAction()
{
    long index = GetPDefinitionManager()->GetDefGlobalIndexFromName("THING_EXPLOSION_TWINBLADE_POUND");
    if (index > 0) {
        C3DVector offset = GetCreature()->GetPhysics()->GetFacing() * 1.5f;
        C3DVector position = GetCreature()->GetPos() + offset;
        CThing* explosion = CTCDExplosion::Create(index, position);
        if (explosion) {
            CTCDriver41* driver;
            if (explosion->HasTC(0x41))
                driver = (CTCDriver41*)explosion->PTCDriver;
            driver->SetOwner(GetCreature());
        }
    }
    CDefPointer<CSpecialEffectsDef> effects;
    bool found;
    CThing* created;
    {
        long effect = 0;
        found = GetCreature()->GetDef()->GetSubDef(&effects) && effects.Object->CalcAndLookup("STUCK", &effect);
        if (found)
            created = CreateSpecialEffect(effect, GetCreature()->GetPos(), false);
    }
    if (found) {
        if (created && !(created->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            created->GetInterface(&emitter, 0x4b);
            emitter->AttachToThing(*GetCreature(), 0x2a, "", 0, 0);
        }
    }
}