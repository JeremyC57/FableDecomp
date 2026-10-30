// NCreatureAction::CActionEventTriggerParticle::Dispatch(const CCreatureActionBase&) -- resolve the effect named EffectName on the source (or its carried weapon) and spawn it, then drop it at the helper/dummy/physics point or attach its emitter to the source
// Grade: DIFFER(547v547) -- 54 difflines over the true extent (547: `ret 4` at +544, int3 padding, then the separate `add ecx,0A8h ; jmp` thunk at 0x00871CB0 -- BOUNDS_FIX); the residue is only scratch-register rotation (eax/ecx/edx for the CCharString argument leas and the C3DVector copy, shifted by one from the first HasHelperPoint argument on). Tried: the attachment test as a bool local (=).
// Leaf: ?Dispatch@CActionEventTriggerParticle
// Layout: CActionEventTriggerParticle from Ego_r.pdb (AttachmentFlags +0x14, AttachmentObject +0x18, EffectName +0x1c, Weapon +0x20,
//   DropRatherThanAttach +0x21); retail CThing view as the landed 0x008717A0 / 0x008C0FB0 (bitset +0x20, map +0x44, PTCDriver +0x5c,
//   physics +0x60, appearance +0x64, def +0x70, flags +0x91).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CRightHandedSet {
    C3DVector First, Second;
    CRightHandedSet() { }
    CRightHandedSet(const CRightHandedSet& other);    // 0x0049CF80
};
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0 (const copy)
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CCharString {
    void* PStringData;
};
struct CDefVirtual {
    virtual ~CDefVirtual();                             // slot 0
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
struct CSpecialEffectsDefPointer {                      // CDefPointer<CSpecialEffectsDef>
    CSpecialEffectsDef* m_p;
    CSpecialEffectsDefPointer() : m_p(0) { }
    ~CSpecialEffectsDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CSpecialEffectsDef* operator->() const { return m_p; }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CSpecialEffectsDef** out);           // 0x00669580 (GetSubDef<CSpecialEffectsDef>)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CSpecialEffectsDef** out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (CSpecialEffectsDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing;
struct CTCGraphicAppearance {
    bool HasHelperPoint(const CCharString& name) const;     // 0x004BE170
    bool HasDummyObject(const CCharString& name) const;     // 0x004BE2C0
    bool GetHelperPoint(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, float scale);   // 0x004C0560
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCDParticleEmitter {
    void SetEmitterPosition(const C3DVector& position);             // 0x006E0500
    void SetEmitterOrientation(const CRightHandedSet& orientation); // 0x006E0480
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                    // 0x006A1440
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                                 // +0x0c
    virtual void v00();
    const CRightHandedSet& GetOrientation() const;
};
struct CPhysicsVirtuals {                               // vtable slot 72 (+0x120): GetOrientation
    void* __vftable_dummy;
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x5c - 0x4c];
    CTCDParticleEmitter* PTCDriver;                     // +0x5c (cached interface-0x4b component)
    struct CPhysics* PPhysics;                          // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                              // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                 // +0x0c
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CCreatureActionBase {
    void AddEffectThing(CThing* thing) const;           // 0x00871CC0 (pushes an intelligent pointer onto the +0x9c list)
};
struct CActionEventTriggerParticle {
    unsigned char _pad00[0x0c];
    CThing* SourceThing;                                // +0x0c
    unsigned char _pad10[4];
    long AttachmentFlags;                               // +0x14
    CCharString AttachmentObject;                       // +0x18
    CCharString EffectName;                             // +0x1c
    bool Weapon;                                        // +0x20
    bool DropRatherThanAttach;                          // +0x21
    void Dispatch(const CCreatureActionBase& action);
};

void CActionEventTriggerParticle::Dispatch(const CCreatureActionBase& action)
{
    CThing* source = SourceThing;
    CTCCarrying* carrying;
    if (Weapon && source->GetInterface(&carrying, 0x46)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon && !(weapon->m_flags91 & 1) && weapon->HasTC(0x48))
            source = weapon;
    }
    CSpecialEffectsDef* effects = 0;
    long effect;
    if (!source->GetDef()->GetSubDef(&effects) || !effects->GetEffectFromString(EffectName, effect)) {
        if (effects && --effects->m_refCount == 0)
            effects->Destroy();
        return;
    }
    if (effect > 0) {
            if (!(AttachmentFlags & 4) || source->PTCGraphicAppearance->HasHelperPoint(AttachmentObject)) {
                if (AttachmentFlags & 2)
                    source->PTCGraphicAppearance->HasDummyObject(AttachmentObject);
            }
            CThing* particle = CreateEffectThing(effect, &source->GetPosition(), false);
            if (particle) {
                action.AddEffectThing(particle);
                CTCDParticleEmitter* emitter = 0;
                if (particle->HasTC(0x4b))
                    emitter = particle->PTCDriver;
                if (DropRatherThanAttach) {
                    if (AttachmentFlags & 4) {
                        C3DVector position;
                        CTCGraphicAppearance* appearance = source->PTCGraphicAppearance;
                        if (appearance->GetHelperPoint(AttachmentObject, 0, 0, &position, 1.0f))
                            emitter->SetEmitterPosition(position);
                    } else if (AttachmentFlags & 2) {
                        C3DVector position;
                        CRightHandedSet orientation;
                        CTCGraphicAppearance* appearance = source->PTCGraphicAppearance;
                        if (appearance->GetDummyObject(AttachmentObject, 0, 0, &position, &orientation, 1.0f)) {
                            emitter->SetEmitterPosition(position);
                            emitter->SetEmitterOrientation(orientation);
                        }
                    } else {
                        CPhysics* physics = source->PPhysics;
                        C3DVector position = physics->Position;
                        emitter->SetEmitterPosition(position);
                        CRightHandedSet orientation(physics->GetOrientation());
                        emitter->SetEmitterOrientation(orientation);
                    }
                } else {
                    AttachmentFlags |= 0x20;
                    emitter->AttachToThing(source, AttachmentFlags, AttachmentObject, 0, 0.0f);
                }
            }
        }
    if (effects && --effects->m_refCount == 0)
        effects->Destroy();
}
