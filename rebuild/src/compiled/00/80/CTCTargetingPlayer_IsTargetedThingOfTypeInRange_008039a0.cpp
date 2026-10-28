// CTCTargetingPlayer::IsTargetedThingOfTypeInRange(unsigned long typeMask) -- the targeted thing must carry one of the targeting flags; unless the type is 2 the world's free-targeting switch accepts it outright, otherwise it must lie within the targeting range the owner's targeting def gives for that type and the weapon in its primary slot
// Leaf: vfunc_38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008039A0, 371 bytes (`ret 4`); CTCTargetingPlayer vtable 0x0124280C slot 38
//          (catalog "vfunc_38"; the name used here is descriptive, the source keeps the catalog leaf).
// Callees: virtual GetPTargetedThing (slot 33, +0x84; three calls); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const
//          0x004365B0 (CTCTargeted 9, CTCCarrying 0x46); CTCBase::PeekWorld() const 0x00686A80; CWorld accessor 0x0049D9D0 (its
//          bool +0x20a; names descriptive); CDefClassBase::GetSubDef<CTargetingDef> 0x007EA960 (manifest twin label
//          "<CAICreatureWillPowerIndicatorDef>"); CTCCarrying::GetThingInPrimarySlot 0x006A1440;
//          GetTargetingRangeForTargetType(unsigned long, const CTargetingDef*, CThing*, CThing*) 0x008028D0 (__fastcall);
//          CThing::GetPos 0x004C73D0 (x2); the def's slot 1 (destroy).
// Layout: CTCTargeted::TargetingFlags +0x0c (engine/CTCTargeted.h, as the landed IsTargetedThingOfType 0x007F5A70); retail
//          CThing interface bitset +0x20, map +0x44, the def class +0x70 (descriptive).
// Shape: both interface lookups are the out-pointer GetInterface without a test, its entry pick a ternary (component lands in ecx); the
//          squared distance is summed Z, X, Y; the def pointer's dtor releases with `dec [ebp+4]`.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CTargetingDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    void Release() { if (--m_count == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
};
class CDefinitionManager { public:
    CTargetingDef* PeekDefFromGlobalIndex(long index);  // stand-in only
};
class CDefClassBase { public:
    virtual long* GetSubDefIndex(const char* name);    // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef(CDefPointer<CTargetingDef>* out);   // 0x007EA960 (GetSubDef<CTargetingDef>)
};
// Stand-in for the GetSubDef<CTargetingDef> template body (0x007EA960, not graded here), as in the landed GetTotalWinnings
// 0x005EA310: compiled above the caller without inlining so VC7.1 sees what it writes.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CTargetingDef>* out)
{
    long* index = GetSubDefIndex("CTargetingDef");
    if (index == 0)
        return false;
    CTargetingDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out->Object != def) {
        if (out->Object)
            out->Object->Release();
        out->Object = def;
        if (def)
            ++def->m_count;
    }
    return true;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CTCTargeted { public:
    unsigned char _pad00[0x0c];
    unsigned long TargetingFlags;               // +0x0c
};
class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
float __fastcall GetTargetingRangeForTargetType(unsigned long type, const CTargetingDef* def, CThing* owner, CThing* weapon);   // 0x008028D0
class CPlayer { public:                          // descriptive
    unsigned char _pad00[0x20a];
    bool FreeTargeting;                         // +0x20a
};
class CWorld { public:
public:
    const CPlayer* PeekMyPlayer() const;   // 0x0049D9D0
};
class CTCBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing() const;  // slot 33 (+0x84)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCTargetingBase : public CTCBase
{
public:
};

class CTCTargetingPlayer : public CTCTargetingBase { public:
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool vfunc_38(unsigned long typeMask);
};

bool CTCTargetingPlayer::vfunc_38(unsigned long typeMask)
{
    if (!GetPTargetedThing())
        return false;
    const CTCTargeted* targeted;
    GetPTargetedThing()->GetInterface(&targeted, 9);
    if (!(targeted->TargetingFlags & typeMask))
        return false;
    if (typeMask != 2 && PeekWorld().PeekMyPlayer()->FreeTargeting)
        return true;
    CTCCarrying* carrying;
    CDefPointer<CTargetingDef> def;
    ParentThing->PDef->GetSubDef(&def);
    ParentThing->GetInterface(&carrying, 0x46);
    float range = GetTargetingRangeForTargetType(typeMask, def.Object, ParentThing, carrying->GetThingInPrimarySlot());
    CThing* owner = ParentThing;
    const C3DVector& targetPos = GetPTargetedThing()->GetPos();
    const C3DVector& ownerPos = owner->GetPos();
    C3DVector offset(ownerPos.X - targetPos.X, ownerPos.Y - targetPos.Y, ownerPos.Z - targetPos.Z);
    bool inRange = offset.Y * offset.Y + offset.X * offset.X + offset.Z * offset.Z <= range * range;
    return inRange;
}