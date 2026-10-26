// CTCHeroRangedAttacks::OnCreate() -- cache the parent thing as the parent creature and take the creature's hero-combat def into PHeroCombatDef
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789C20, length 97 bytes (+15 int3 padding). Vtable 0x0126BAF4 slot 4 (the
// catalog's "vfunc_4"; slot 4 of the CTCBase vtable is OnCreate).
// Callees: the thing's hero-combat def getter @ 0x00663340 (CDefPointer by hidden pointer; manifest "CEventKilledCreature" ctor
// is a propagated label), the def's slot 1 on release.
// Shape: as CTCHeroExperience::OnCreate (0x006D6EE0): the temporary def pointer goes through CDefPointer::operator= and is
// destroyed by its null-tested dtor calling the def's inline ReleaseTemporary() (ref-count base at +4).
// Layout: Ego_r.pdb CTCHeroRangedAttacks (engine/CTCHeroRangedAttacks.h, same offsets in retail): PParentCreature +0x0c,
// PHeroCombatDef +0x10.
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefObject : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};
class CHeroCombatDef : public CDefObject { public:};
class CThingCreatureBase;
class CThing {
public:
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const;  // 0x00663340
};
class CTCHeroRangedAttacks {
public:
    virtual ~CTCHeroRangedAttacks();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x4];
    CThingCreatureBase*         PParentCreature;   // +0x0c
    CDefPointer<CHeroCombatDef> PHeroCombatDef;    // +0x10
    virtual void OnCreate();
};

void CTCHeroRangedAttacks::OnCreate()
{
    PParentCreature = (CThingCreatureBase*)ParentThing;
    PHeroCombatDef = ParentThing->GetHeroCombatDef();
}