// CThingCreatureBase::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other) -- base game-object collision handling; a creature in mode 0x42 hitting a surface that is not floor-like (normal . up < 0.866) gets knocked down; then (both alive) the creature's hit-on-collision component handles the hit
// Leaf: ?OnCollision@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00667310, 307 bytes (+13 int3), `ret 10h`. CThingCreatureBase vtable 0x0125AC0C slot 41.
// Callees: CThingGameObject::OnCollision @ 0x00839080, CInterfaceMap::LowerBound const @ 0x004365B0 / non-const @ 0x0040F020,
//   CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated; see
//   labels_chunk196_corrections), the mode's virtual slot 1 (its type), the CCombatAction_GenericStrikeResponseKnockdown
//   ctor @ 0x008679C0, CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CBaseIntelligentPointer @ 0x00A01C10,
//   ~CCharString @ 0x0099EAE0, ~CCombatActionBase @ 0x0062E900, CTCHitOnCollision::OnCollision @ 0x007C81F0.
//   Float pool: 0.0f @ 0x0122DEDC, 0.866f @ 0x0125B168.
// Layout: retail CThing (local model as in the landed sibling CThingAICreature::OnCollision 0x00832840): interface bitset
//   +0x20 (TC 0x31 = word +0x24 bit 17, TC 0xC8 = word +0x38 bit 8), CVectorMap +0x44, Dead = +0x91 bit 0. The knockdown
//   action is a local model: retail CCombatActionBase is 0x114 bytes, then AnimName (CCharString) +0x114 and PThingThatHit
//   (CIntelligentPointer) +0x118 (the PDB's base is 0x18 bytes larger); 0x13c bytes in all (the PDB packs DirectionOfHit unaligned after GotPointOfHit).
// Shape: the mode test and the knockdown live inside HasTC(0x31) with the const GetTC; the slope test is an inline dot
//   product with the constant up vector (the compiler keeps (Y + X) * 0 + Z); the knockdown action is a temporary passed
//   straight to SetCurrentAction, destroyed by its implicit (inline) destructor.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};
enum ECollideType { COLLIDE_NONE = 0, COLLIDE_LANDSCAPE = 1 };
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CThing;
struct CCreatureMode {
    virtual void v00();
    virtual long GetType() const;               // slot 1
};
struct CTCCreatureModeManager {
    CCreatureMode* GetCurrentMode() const;      // 0x0073A6E0
};
struct CTCHitOnCollision {
    void OnCollision(C3DVector* position, C3DVector* normal, CThing* other);   // 0x007C81F0
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

struct CThingGameObject : CThing {
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);     // 0x00839080
};

struct CCharString {
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    void* m_vtable;
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CThingCreatureBase;
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCombatAction_GenericStrikeResponseKnockdown : CCombatActionBase {
    CCharString             AnimName;           // +0x114
    CBaseIntelligentPointer PThingThatHit;      // +0x118
    unsigned char           _hitInfo[0x1c];     // +0x120 PointOfHit, GotPointOfHit, DirectionOfHit, flags (PDB packing; size 0x13c)
    CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* creature, CThing* hitter, C3DVector* point,
                                                 C3DVector* direction);   // 0x008679C0
};

struct CThingCreatureBase : CThingGameObject {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);
};

void CThingCreatureBase::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other)
{
    CThingGameObject::OnCollision(type, position, normal, other);
    if (HasTC(0x31)) {
        const CThingCreatureBase* self = this;
        if (self->GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetType() == 0x42
            && normal->Dot(C3DVector(0.0f, 0.0f, 1.0f)) < 0.866f)
            SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(this, other, position, normal));
    }
    if (!Dead && other && !other->Dead && HasTC(0xC8))
        GetTC<CTCHitOnCollision>(0xC8)->OnCollision(position, normal, other);
}
