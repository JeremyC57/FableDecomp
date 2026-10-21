// CAIStateGroup_MeleeCombatNearbyBase::OnReadyToAttack() -- in combat mode 3, when the current region forbids fighting, ask for money and terminate unless the group may continue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D720, length 128 bytes. Vtable slot 15 of
// CAIStateGroup_MeleeCombatNearbyBase (0x0128B954).
// Callees: the creature's virtual slot 48 (+0xc0, a mode query), CAIStateGroupBase::PeekWorldMap @ 0x008FCFF0 (x2),
// CWorldMap's current-region index getter @ 0x004FB150 (`[this+9Ch]`; manifest "CWorld::DrawGetEnvironment" is
// propagated), CWorldMap::GetRegion(long) @ 0x004FC180 (0x58-byte regions), CAIStateGroup_MeleeCombatNearbyBase::
// RequestMoney @ 0x0093D580, this group's virtual slot 5 (+0x14), CAIStateGroupBase::Terminate @ 0x008FCF40, the
// region def's virtual slot 1 (destroy, tail-jumped).
//
// Shape: `GetRegion(PeekWorldMap()->index())` with PeekWorldMap re-called for the object; the region's counted def
// pointer (+0x24) is copy-constructed (from a named region reference) into a CDefPointer local whose null test guards
// the whole body; the copy ctor add-refs through an inline AddRef() (`inc dword ptr [esi+4]`)
// and the dtor releases (`dec [esi+4] ; jne`, then the virtual destroy) at both exits. The def
// flag at +0x9d and the region/def classes are descriptive. No engine header models the vptr chain: local model.
struct CRegionDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    unsigned char _pad[0x95];                   // +0x08..+0x9c
    unsigned char m_noFighting;                 // +0x9d (descriptive)
    void AddRef() { ++m_refCount; }
};

struct CRegionDefPointer {                       // CDefPointer<CRegionDef>
    CRegionDef* m_pDef;
    CRegionDefPointer(const CRegionDefPointer& o) : m_pDef(o.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CRegionDefPointer() { if (m_pDef && --m_pDef->m_refCount == 0) m_pDef->Destroy(); }
    CRegionDef* operator->() const { return m_pDef; }
};
struct CRegion {                                // 0x58 bytes
    unsigned char _pad[0x24];
    CRegionDefPointer PDef;                     // +0x24
};

struct CWorldMap {
    long GetCurrentRegionIndex();               // 0x004FB150
    CRegion& GetRegion(long index);             // 0x004FC180
};

struct CThingCreatureBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47();
    virtual long GetCombatMode();               // slot 48 (+0xc0, descriptive)
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool CanContinue();                 // slot 5 (+0x14, descriptive)
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void OnReadyToAttack();             // slot 15 (+0x3c)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CWorldMap* PeekWorldMap();                  // 0x008FCFF0
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    void RequestMoney();                        // 0x0093D580
    virtual void OnReadyToAttack();
};

void CAIStateGroup_MeleeCombatNearbyBase::OnReadyToAttack()
{
    if (PBrain->PCreature->GetCombatMode() == 3) {
        CRegion& region = PeekWorldMap()->GetRegion(PeekWorldMap()->GetCurrentRegionIndex());
        CRegionDefPointer def = region.PDef;
        if (def.m_pDef) {
            if (def->m_noFighting) {
                RequestMoney();
                if (!CanContinue()) {
                    Terminate();
                    return;
                }
            }
        }
    }
}