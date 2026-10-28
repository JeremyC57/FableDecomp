// NEntityEvents::CEventKilledCreature::CEventKilledCreature(const CThing& killer, const CThing& killed) -- a killed-creature event recording the killed thing's creature group (0 when it has no creature def)
// Leaf: ??0CEventKilledCreature@NEntityEvents@@QAE@ABVCThing@@0@Z
// Retail: Fable.exe @ 0x00811190, 87 bytes, thiscall `ret 8`.
// Identity (proven): it stores vtable 0x012723A8 (RTTI .?AVCEventKilledCreature@NEntityEvents@@) and is the only
//   constructor shape of that class; FableWin.pdb ??0CEventKilledCreature@NEntityEvents@@QAE@ABVCThing@@0@Z (two const
//   CThing&). The body reads only the second argument; the parameter names (killer, killed) are descriptive.
// Layout: Ego_r.pdb CEventBase (vptr, WorldFrame +4), CEventKilledCreature CreatureGroupOfKilledThing +8; CThing
//   PBaseDef +0x70; CCreatureDef CreatureGroup +0xAC (Ego_r +0xBC, four Dinkumware containers 4 bytes smaller each).
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) const 0x00485815 (same-TU
//   stand-in, the landed 0x00864B30 / AddWound 0x00757B30 model); the def's slot 1 on the last release.
class CCreatureDef {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long  RefCount;                                 // +0x04
    unsigned char _pad08[0xAC - 0x08];
    unsigned long CreatureGroup;                            // +0xAC
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CDefinitionManager {
public:
    CDefPointer<const CCreatureDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
class CDefClassBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                         // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
// Same-TU stand-in for GetSubDef<CCreatureDef> 0x00485815 (the landed 0x00864B30 body).
#pragma auto_inline(off)
template <> bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const long* index;
    {
        CCharString name("CCreatureDef");
        index = GetSubDefIndex(name);
    }
    if (index) {
        CDefPointer<const CCreatureDef> result = m_pManager->GetPDefFromGlobalIndex(*index);
        out->Object = result.Object;
        if (out->Object) ++out->Object->RefCount;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThingBaseDef : public CDefClassBase { };
class CThing {
public:
    unsigned char _pad0[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
};
namespace NEntityEvents {
class CEventBase {
public:
    virtual ~CEventBase();
    long WorldFrame;                                        // +0x04
    CEventBase() : WorldFrame(0) { }
};
class CEventKilledCreature : public CEventBase {
public:
    unsigned long CreatureGroupOfKilledThing;               // +0x08
    CEventKilledCreature(const CThing& killer, const CThing& killed);
    virtual ~CEventKilledCreature();
};
}
NEntityEvents::CEventKilledCreature::CEventKilledCreature(const CThing& killer, const CThing& killed)
    : CreatureGroupOfKilledThing(0)
{
    CDefPointer<const CCreatureDef> def;
    if (killed.PBaseDef->GetSubDef(&def))
        CreatureGroupOfKilledThing = def->CreatureGroup;
}