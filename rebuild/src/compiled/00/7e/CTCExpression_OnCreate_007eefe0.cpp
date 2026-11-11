// CTCExpression::OnCreate() -- when no expression def is set yet, take the parent def's expression sub-def and, if it names an expression, fetch that expression def into PDef
// Leaf: OnCreate
//
// Retail 0x007EEFE0, 130 bytes (plain `ret`; vtable 0x012401F4 slot 4, Ego_r.pdb
// `?OnCreate@CTCExpression@@UAEXXZ`; the catalog "CTCExpression::vfunc_4" is the slot name). Callees:
// CDefClassBase::GetSubDef<CExpressionSubDef> @ 0x005DA240 (pushes "CExpressionSubDef"), CTCBase::
// GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CExpressionDef>(long, CDefPointer<const T>*) const
// @ 0x005F81BE (byte-twin template; the instantiation follows PDef's type, Ego_r CDefPointer<const CExpressionDef>); the defs'
// virtual slot 1 when a def pointer drops its last reference.
// Layout = Ego_r.pdb CTCExpression (0x10): PDef +0xc; retail CThing's def pointer object at +0x70; the
// sub-def's expression def index (CExpressionSubDef::ExpressionDef) at +0x28.
// Shape: the sub-def pointer is a named local zeroed up front; the parent def is an inline by-value
// getter (null-tested add-ref into esi) whose release follows the `GetSubDef && index > 0` test kept
// in bl; the sub-def object stays in edi for the index push and its own null-tested release.
class CDefClassBase;
class CExpressionDef;

struct CRefCountedDef {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DestroySelf(); }
};

template<class T> class CDefPointer { public:
    CRefCountedDef* Object;                                 // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return static_cast<T*>(Object); }
};

class CDefClassBase : public CRefCountedDef { public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005DA240 GetSubDef<CExpressionSubDef> (pushes "CExpressionSubDef")
};
class CThingBaseDef : public CDefClassBase { public: };
class CExpressionSubDef : public CDefClassBase { public:   // the parent def's expression sub-def (Ego_r.pdb 0x2c bytes)
    unsigned char _pad08[0x28 - 0x08];
    long          ExpressionDef;                            // +0x28 (the expression def's global index)
};
// The sub-def out slot is released by hand at the end of the scope (retail keeps the object in edi): this pointer kind is
// modelled without a destructor.
template<> class CDefPointer<const CExpressionSubDef> { public:
    CRefCountedDef* Object;
    CDefPointer() : Object(0) { }
};

void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CExpressionSubDef>(CDefPointer<const CExpressionSubDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CExpressionDef : public CDefClassBase { public:
};

class CDefinitionManager { public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x005F81BE GetDef<CExpressionDef>
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template<> __declspec(noinline) bool CDefinitionManager::GetDef<CExpressionDef>(long index, CDefPointer<const CExpressionDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char              _pad00[0x70];
    CDefPointer<const CThingBaseDef> PDef;                  // +0x70
    CDefPointer<const CThingBaseDef> GetDef() const { return PDef; }
};

class CTCBase { public:                                     // the component base (members this row reads)
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
class CTCExpression : public CTCBase { public:
    CDefPointer<const CExpressionDef> PDef;                 // +0x0c
    virtual void OnCreate();                                // Ego_r.pdb ?OnCreate@CTCExpression@@UAEXXZ
};

void CTCExpression::OnCreate()
{
    if (PDef.Object == 0)
    {
        CDefPointer<const CExpressionSubDef> subDef;
        bool hasExpression = ParentThing->GetDef()->GetSubDef(&subDef) && static_cast<const CExpressionSubDef*>(subDef.Object)->ExpressionDef > 0;
        const CExpressionSubDef* expression = static_cast<const CExpressionSubDef*>(subDef.Object);
        if (hasExpression)
            GetDefinitionManager().GetDef(expression->ExpressionDef, &PDef);
        if (subDef.Object != 0)
            subDef.Object->Release();
    }
}