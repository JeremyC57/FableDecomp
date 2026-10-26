// CTCHeroExperience::OnCreate() -- fetch the hero-experience sub-def of the parent thing's def into PDef and, for a hero thing (CThing +0x6c bit 3), take its hero-combat def into PHeroCombatDef
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6EE0, length 115 bytes (+13 int3 padding). Vtable 0x0125F6C4 slot 4 (the
// catalog's "vfunc_4"; slot 4 of the CTCBase vtable is OnCreate).
// Callees: CDefClassBase::GetSubDef<CHeroExperienceDef>(CDefPointer*) @ 0x006DAA90 (manifest names a propagated instantiation),
// the thing's hero-combat def getter @ 0x00663340 (CDefPointer by hidden pointer; manifest "CEventKilledCreature" ctor is a
// propagated label), the def's slot 1 on release.
// Shape: the temporary def pointer is assigned through CDefPointer::operator= (identity test, release + clear, AddRef) and then
// destroyed by its dtor, which has a null test and calls the def's inline ReleaseTemporary() whose DecRef/GetRefCount live on
// the ref-count base at +4 (`mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ...`).
// Layout: retail PDef +0x54, PHeroCombatDef +0x58 (engine/CTCHeroExperience.h has the PDB offsets +0x70/+0x74); CThing def +0x70,
// CThing flag byte +0x6c.
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
    T* operator->() const { return Object; }
};
class CHeroExperienceDef : public CDefObject { public:};
class CHeroCombatDef : public CDefObject { public:};
class CDefClassBase : public CDefObject { public:
    bool GetSubDef(CDefPointer<CHeroExperienceDef>* out);  // 0x006DAA90
};
class CThing {
public:
    unsigned char _pad00[0x6c];
    unsigned char _flags6c_0 : 3;
    unsigned char IsHeroThing : 1;              // +0x6c bit 3
    unsigned char _pad6d[0x70 - 0x6d];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const;  // 0x00663340
};
class CTCHeroExperience {
public:
    virtual ~CTCHeroExperience();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x54 - 0x08];
    CDefPointer<CHeroExperienceDef> PDef;       // +0x54
    CDefPointer<CHeroCombatDef>     PHeroCombatDef;   // +0x58
    virtual void OnCreate();
};

void CTCHeroExperience::OnCreate()
{
    ParentThing->PDef->GetSubDef(&PDef);
    if (ParentThing->IsHeroThing)
        PHeroCombatDef = ParentThing->GetHeroCombatDef();
}