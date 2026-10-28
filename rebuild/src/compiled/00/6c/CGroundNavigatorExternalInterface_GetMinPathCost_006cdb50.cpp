// CGroundNavigatorExternalInterface::GetMinPathCost() const -- the creature's navigation def (CCreatureNavigationDef sub-def of its thing def) minimum path cost
// Leaf: ?GetMinPathCost@CGroundNavigatorExternalInterface@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CDB50, 52 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x0125EC9C (RTTI CGroundNavigatorExternalInterface) slot 18, beside slot 19 GetMaxPathCost
//   (0x006CDB90, the +0x2C twin); Ego_r.pdb ?GetMinPathCost@CGroundNavigatorExternalInterface@@UBEMXZ and
//   CCreatureNavigationDef::MinPathCost +0x28 / MaxPathCost +0x2C (Ego_r.pdb, the same def whose Radius +0x30 the landed
//   ctor 0x006CD890 reads).
// Callees: CDefClassBase::GetSubDef<CCreatureNavigationDef>(CDefPointer<const CCreatureNavigationDef>*) const 0x006CD2F0
//   (landed; Ego_r.pdb template spelling), the def's virtual slot 1 (+4) when its count drops to zero.
// Layout: Creature +0x04 (engine/CGroundNavigatorExternalInterface.h; the class is polymorphic, so it is modelled
//   locally as in the landed ctor 0x006CD890), the creature's thing def (CDefClassBase*) +0x70.
// Shape: the out pointer is a zeroed local; the def is copied into a holder whose destructor releases it with no null test
//   (the landed ctor's idiom), so the float is read before the release and comes back through the frame slot.
class CDefPointeeBase { public:
    virtual void Destroy();                        // slot 0
    virtual void Delete() const;                   // slot 1 (+0x04)
    mutable long m_refCount;                       // +0x04
    void Release() const { if (--m_refCount == 0) Delete(); }
};
class CCreatureNavigationDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    float         MinPathCost;                     // +0x28
    float         MaxPathCost;                     // +0x2C
    float         Radius;                          // +0x30
    bool          AvoidDynamicObstacles;           // +0x34
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {             // releases the fetched def without a null test
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { Object->Release(); }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x006CD2F0 for T = CCreatureNavigationDef
};
void* FindDef_CDefClassBase_GetSubDef_0();        // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail, landed 0x006CD2F0): it writes the out slot only inline, so VC7.1
// knows the def pointer's address does not escape the call (the landed ctor 0x006CD890 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingCreatureBase { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                           // +0x70
};
class CGroundNavigatorExternalInterface { public:  // vtable 0x0125EC9C
    virtual ~CGroundNavigatorExternalInterface();
    CThingCreatureBase* Creature;                  // +0x04
    float               Radius;                    // +0x08
    virtual float GetMinPathCost() const;
};

float CGroundNavigatorExternalInterface::GetMinPathCost() const
{
    CDefPointer<const CCreatureNavigationDef> out;
    Creature->PDef->GetSubDef(&out);
    CDefHolder<const CCreatureNavigationDef> def(out.Object);
    return def->MinPathCost;
}