// CCreatureActionBase::GetActionSpeedParameter() const -- the action's speed parameter: the creature's CCreatureStatsDef speed when its def has that sub-def, else 0
// Leaf: ?GetActionSpeedParameter@CCreatureActionBase@@MBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692C30, 76 bytes, thiscall, `ret`.
// Identity (proven): slot 63 of the CCreatureActionBase-derived vtables (e.g. CCreatureAction_StartSneaking 0x01258DFC
//   slot 63 = 0x00692C30, 404 vtable references: the base implementation); Ego_r.pdb
//   ?GetActionSpeedParameter@CCreatureActionBase@@MBEMXZ (protected virtual, const, float).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (PCreature +0x08),
//   CDefClassBase::GetSubDef<CCreatureStatsDef>(CDefPointer<const CCreatureStatsDef>*) const 0x00696130 (landed, Ego_r
//   template spelling) on the creature's thing def (+0x70); the def's virtual slot 1 (+4) when its count drops to zero.
// Layout: CCreatureStatsDef::Speed +0x3C (Ego_r.pdb).
// Shape: the thing def is a named local (so &stats is pushed after the GetPItem call); the float result is a zeroed local (frame word 0), the def pointer a zeroed local (frame word 1) whose
//   destructor releases it behind a null test (`dec [ecx+4] ; jne` with the def kept in ecx: needs the stand-in body).
class CDefPointeeBase { public:
    virtual void Destroy();                        // slot 0
    virtual void Delete() const;                   // slot 1 (+0x04)
    mutable long m_refCount;                       // +0x04
};
class CCreatureStatsDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x3C - 0x08];
    float         Speed;                           // +0x3C
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00696130 for T = CCreatureStatsDef
};
void* FindDef_CDefClassBase_GetSubDef_0();        // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail, landed 0x00696130): it writes the out slot only inline, so VC7.1
// knows the def pointer's address does not escape the call (the landed ctor 0x006CD890 idiom): the release then keeps the
// def in ecx (`dec [ecx+4]`) instead of re-reading it after the store.
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

class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void* PData;                                         // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThingCreatureBase { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                 // +0x70
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    unsigned char                           _pad04[4];   // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
protected:
    virtual float GetActionSpeedParameter() const;
};

float CCreatureActionBase::GetActionSpeedParameter() const
{
    float speed = 0.0f;
    CDefPointer<const CCreatureStatsDef> stats;
    CDefClassBase* def = PCreature.GetPItem()->PDef;
    if (def->GetSubDef(&stats))
        speed = stats->Speed;
    return speed;
}