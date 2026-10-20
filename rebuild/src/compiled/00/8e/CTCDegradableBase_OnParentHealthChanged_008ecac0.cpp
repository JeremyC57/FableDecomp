// CTCDegradableBase::OnParentHealthChanged(bool fromHit) -- for a degradable def that is not yet fully degraded: move to the stage matching the parent's health when it is lower than the current one (new mesh, smash particles unless hit, the parent made unhittable at the last stage) and pass the stage's navigation blocking to the switchable-navigation component
// Leaf: OnParentHealthChanged
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ECAC0, 287 bytes (+ int3 padding), `ret 4` (one exit is the tail `jmp` to
//   SetAsBlockingNavigation with its argument rewritten in the `fromHit` slot).
// Callees: the parent physical thing's health fraction getter @ 0x006A4E40 (manifest "CParticleSystem::GetNormalisedTime" is
//   propagated), GetDegradableInfoForHealthPercentage(float) @ 0x008EC940, ChangeMesh(const CEngineGraphic&) @ 0x008EC9C0,
//   ShowParticleEffects(long) @ 0x008ECA50, IsFullyDegraded @ 0x008EC900 (second use only), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for interface 0x79, bit 25 of +0x2c), CTCSwitchableNavigation::
//   SetAsBlockingNavigation(bool) @ 0x00787580, the def's virtual slot 1 when a copied def pointer drops its last reference.
// Globals: the 1e-4f tolerance @ 0x0129BA3C (an extern float variable).
// Layout = Ego_r.pdb CTCDegradableBase (as the landed OnInitialActivate 0x008ECBE0 / IsFullyDegraded 0x008EC900): ParentThing
//   +0x04, PDefinition +0x10, PCurrentDegradableInfo +0x14, PParentThingPhysical +0x18; CDegradableDef Degradable +0x25,
//   Degradations (retail 12-byte vector of 0x1c-byte CDegradableInfo) +0x30; CDegradableInfo HealthPercentage +0x00,
//   EngineGraphic +0x04, SmashParticleEmitter +0x14, BlocksNavigation +0x18; the parent's hittable bit = bit 0 of +0xbc.
// Shape: the def through the inline by-value GetDefinition() (null-tested add-ref, release without a null test, the
//   result parked in bl; the test is written positively around the body -- `if (!Degradable) return;` parks the
//   negation with `test ; sete bl`); IsFullyDegraded is inlined at its first use only (an inline copy of the landed body) and called
//   out of line at the second; `cur - new > eps` gives `test ah,41h ; jne` to the shared return; the component comes from
//   the out-parameter GetInterface; its call is the last statement, so VC7.1 tail-jumps.
extern float g_epsilon_0129ba3c;                            // 0x0129BA3C (1e-4f)
struct CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
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
struct CThingPhysical {
    unsigned char _pad0[0xbc];
    unsigned char Hittable : 1;                             // +0xbc bit 0
    float GetHealthPercentage();                            // 0x006A4E40
};
struct CEngineGraphic { unsigned char _body[0x10]; };
struct CDegradableInfo {                                    // 0x1c bytes
    float          HealthPercentage;                        // +0x00
    CEngineGraphic EngineGraphic;                           // +0x04
    long           SmashParticleEmitter;                    // +0x14
    bool           BlocksNavigation;                        // +0x18
};
template<class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& back() const { return *(m_pLast - 1); }
};
struct CDegradableDef {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long          RefCount;                                 // +0x04
    unsigned char _pad08[0x25 - 0x08];
    bool          Degradable;                               // +0x25
    unsigned char _pad26[0x30 - 0x26];
    CRetailVector<CDegradableInfo> Degradations;            // +0x30
    void AddRef() { ++RefCount; }
    void Release() { --RefCount; if (RefCount == 0) DestroySelf(); }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
struct CTCSwitchableNavigation {
    void SetAsBlockingNavigation(bool blocking);            // 0x00787580
};
struct CTCDegradableBase {
    virtual void v00();
    CThing*                     ParentThing;                // +0x04 (CTCBase)
    bool                        ToBeRemoved;                // +0x08
    bool                        IsInGlobalUpdateTCs;        // +0x09
    long                        LastWorldFrameHit;          // +0x0c
    CDefPointer<CDegradableDef> PDefinition;                // +0x10 CDefPointer<const CDegradableDef>
    CDegradableInfo*            PCurrentDegradableInfo;     // +0x14
    CThingPhysical*             PParentThingPhysical;       // +0x18
    CDefPointer<CDegradableDef> GetDefinition() const { return PDefinition; }
    bool IsFullyDegradedInline() const                      // the landed 0x008EC900 body, inlined at the first use
    {
        if (PCurrentDegradableInfo != 0) {
            if (PCurrentDegradableInfo == &GetDefinition()->Degradations.back())
                return true;
        }
        return false;
    }
    bool IsFullyDegraded() const;                           // 0x008EC900
    const CDegradableInfo* GetDegradableInfoForHealthPercentage(float percentage);   // 0x008EC940
    void ChangeMesh(const CEngineGraphic& graphic);         // 0x008EC9C0
    void ShowParticleEffects(long emitter);                 // 0x008ECA50
    void OnParentHealthChanged(bool fromHit);
};

void CTCDegradableBase::OnParentHealthChanged(bool fromHit)
{
    if (GetDefinition()->Degradable) {
        if (IsFullyDegradedInline())
            return;
        CDegradableInfo* info = (CDegradableInfo*)GetDegradableInfoForHealthPercentage(PParentThingPhysical->GetHealthPercentage());
        if (!info)
            return;
        if (PCurrentDegradableInfo && !(PCurrentDegradableInfo->HealthPercentage - info->HealthPercentage > g_epsilon_0129ba3c))
            return;
        PCurrentDegradableInfo = info;
        ChangeMesh(info->EngineGraphic);
        if (!fromHit)
            ShowParticleEffects(PCurrentDegradableInfo->SmashParticleEmitter);
        if (IsFullyDegraded())
            PParentThingPhysical->Hittable = false;
        CTCSwitchableNavigation* navigation;
        if (ParentThing->GetInterface(&navigation, 0x79))
            navigation->SetAsBlockingNavigation(PCurrentDegradableInfo->BlocksNavigation);
    }
}