// CTCActionUseWallMount::OnActionUse(CThingCreatureBase* user) -- after the base handling, a living user first lets the mounted thing's wall-mount effects (0xDB) handle the use; when they do not, the user starts the use-wall-mount action on this object
// Leaf: ?OnActionUse@CTCActionUseWallMount
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EFE70, 256 bytes, `ret 4`; the OnActionUse slot 28 (catalog "vfunc_28").
// Callees: CTCActionUseBase::OnActionUse 0x007EAB90 (manifest "$E2" is propagated), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020, the wall mount's mounted-thing getter 0x007DE4E0 (interface 0xDA; descriptive name,
//   manifest label propagated), the effects' use test 0x007DF3D0 (manifest "CTCCameraCollision::HasCameraPhysicsMesh
//   Object", propagated; descriptive name) and CTCWallMountEffects::OnWallMountUseAction(CThingCreatureBase*)
//   0x007DF4B0 (interface 0xDB), CCreatureAction_UseWallMount ctor 0x00843610 (vtable 0x0127498C per RTTI; the
//   manifest and the landed 0x007EDDF0 call it CCreatureAction_DeclarePlayerACriminal), CThingCreatureBase::
//   SetCurrentAction 0x006644F0, ~CBaseIntelligentPointer 0x00A01C10, ~CCreatureActionBase 0x00693EF0.
// Layout: CTCBase ParentThing +4; CThing dead bit +0x91 bit 0, interface bitset +0x20, map +0x44; the action is
//   0xb0 bytes (retail CCreatureActionBase 0xa8 + the target intelligent pointer).
// Shape: nested ifs with the effects' veto as an early `return`; the wall-mount component is the ternary GetTC with its
//   own null test, the effects component a named local (esi); the action is a temporary passed by address straight
//   from the ctor (implicit destructor: member then base, no vtable store).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CCreatureActionBase;
class CThingPlayerCreature;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};
inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}
class CTCWallMount { public:                           // interface 0xDA
    CThing* GetMountedThing();                  // 0x007DE4E0
};
class CTCWallMountEffects { public:                    // interface 0xDB
    bool IsUsable();                            // 0x007DF3D0
    bool OnWallMountUseAction(CThingCreatureBase* user);   // 0x007DF4B0
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: 
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_UseWallMount : public CCreatureActionBase { public:   // vtable 0x0127498C
    CIntelligentPointer<CThing> PWallMount;     // +0xa8
    CCreatureAction_UseWallMount(CThingCreatureBase* owner, CThingPlayerCreature* target);   // 0x00843610
};

class CTCActionUseBase {
public:
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    void OnActionUse(CThingCreatureBase* user); // 0x007EAB90
};
class CTCActionUseWallMount : public CTCActionUseBase {
public:
    void OnActionUse(CThingCreatureBase* user);
};

void CTCActionUseWallMount::OnActionUse(CThingCreatureBase* user)
{
    CTCActionUseBase::OnActionUse(user);
    if (IsThingAlive(user)) {
        if (ParentThing->HasTC(0xda)) {
            CTCWallMount* mount = ParentThing->GetTC<CTCWallMount>(0xda);
            if (mount) {
                CThing* mounted = mount->GetMountedThing();
                if (IsThingAlive(mounted) && mounted->HasTC(0xdb)) {
                    CTCWallMountEffects* effects = mounted->GetTC<CTCWallMountEffects>(0xdb);
                    if (effects->IsUsable() && effects->OnWallMountUseAction(user))
                        return;
                }
            }
        }
        user->SetCurrentAction(&CCreatureAction_UseWallMount(user, (CThingPlayerCreature*)ParentThing));
    }
}