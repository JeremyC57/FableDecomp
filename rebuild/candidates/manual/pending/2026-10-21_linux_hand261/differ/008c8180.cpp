// CCreatureAction_BanditKingLunge::PerformAction() -- spawn THING_EXPLOSION_TWINBLADE_POUND 1.5 units ahead of the creature (owner = creature), then create its STUCK special effect at the creature and attach it through the particle emitter
// Grade: DIFFER(506v506) -- GetInterface(0x4B) key slot colours with the STUCK temp (+0x18) where retail shares the dead effect slot (+0x1c); rest identical
// Leaf: ?PerformAction@CCreatureAction_BanditKingLunge
// Layout: CThing bitset +0x20, map +0x44, PTCDriver +0x5c, physics +0x60, def +0x70, dead flag +0x91 bit 0 (as 0x8C8380/0x8F5060).
// Tried: function-scoped effect (a), early-return (b), block-scoped effect + found (c, this), + LowerBound stand-in (d).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CTCBase;
struct CThing;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
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
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;              // slot 70 (+0x118)
};
struct CThing {
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
    const C3DVector& GetPosition() const;             // 0x004C73D0
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b);   // 0x006E0BE0
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CTCDExplosion {
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);   // 0x0080C460
};
struct CTCDriver41 {
    void SetOwner(CThing* owner);                     // 0x0080CBF0
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
};
struct CCreatureAction_BanditKingLunge : CCreatureActionBase {
    void PerformAction();
};

void CCreatureAction_BanditKingLunge::PerformAction()
{
    long index = GetPDefinitionManager()->GetDefGlobalIndexFromName("THING_EXPLOSION_TWINBLADE_POUND");
    if (index > 0) {
        C3DVector offset = GetCreature()->GetPhysics()->GetFacing() * 1.5f;
        C3DVector position = GetCreature()->GetPosition() + offset;
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
            created = CreateSpecialEffect(effect, GetCreature()->GetPosition(), false);
    }
    if (found) {
        if (created && !(created->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            created->GetInterface(&emitter, 0x4b);
            emitter->AttachToThing(GetCreature(), 0x2a, "", 0, 0);
        }
    }
}
