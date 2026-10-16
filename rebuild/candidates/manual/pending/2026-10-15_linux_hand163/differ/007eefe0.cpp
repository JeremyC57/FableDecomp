// CTCExpression::OnCreate() -- when no expression def is set yet, take the parent def's expression sub-def and, if it names an expression, fetch that expression def into PDef
// Grade: DIFFER(134v130) -- ours keeps &PDef in ebp (lea ebp,[edi+0Ch]) with this in edi and reloads the sub-def object from the frame; retail keeps this in ebp, recomputes lea eax,[ebp+0Ch] at the GetDef push and holds the sub-def object in edi from right after GetSubDef through its release (cached across the parent-def release and GetDef calls); tried a CDefPointer local with dtor at the top / inside the if, /Oa, a hand-released out pointer with an object local (best)
// Leaf: OnCreate
//
// Retail 0x007EEFE0, 130 bytes (plain `ret`; vtable 0x012401F4 slot 4, Ego_r.pdb
// `?OnCreate@CTCExpression@@UAEXXZ`; the catalog "CTCExpression::vfunc_4" is the slot name). Callees:
// CDefClassBase::GetSubDef<...> @ 0x005DA240 (manifest template name is a byte twin), CTCBase::
// GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDef<CExpressionDef>(long, CDefPointer*)
// @ 0x005F81BE (manifest "GetDef<CSpecialAbilitiesDrunkennessDef>", a byte-twin template); the defs'
// virtual slot 1 when a def pointer drops its last reference.
// Layout = Ego_r.pdb CTCExpression (0x10): PDef +0xc; retail CThing's def pointer object at +0x70; the
// sub-def's expression def index at +0x28.
// Shape: the sub-def pointer is a named local zeroed up front; the parent def is an inline by-value
// getter (null-tested add-ref into esi) whose release follows the `GetSubDef && index > 0` test kept
// in bl; the sub-def object stays in edi for the index push and its own null-tested release.
struct CDefClassBase;
struct CExpressionDef;

struct CRefCountedDef {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DestroySelf(); }
};

template<class T> struct CDefPointerOut {                   // the out-parameter def pointer, released by hand
    T* Object;
    CDefPointerOut() : Object(0) { }
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return Object; }
};

struct CExpressionUserDef : CRefCountedDef {                // the parent def's expression sub-def
    unsigned char _pad08[0x28 - 0x08];
    long          ExpressionDefIndex;                       // +0x28
};

struct CDefClassBase : CRefCountedDef {
    bool GetSubDef(CDefPointerOut<CExpressionUserDef>* out);   // 0x005DA240
};

struct CExpressionDef : CRefCountedDef {
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CExpressionDef>* out);  // 0x005F81BE
};

struct CThing {
    unsigned char              _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                        // +0x70
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
};

struct CTCExpression {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CDefPointer<CExpressionDef> PDef;                       // +0x0c
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0 (CTCBase)
    void OnCreate();
};

void CTCExpression::OnCreate()
{
    if (PDef.Object == 0)
    {
        CDefPointerOut<CExpressionUserDef> userDef;
        bool hasExpression = ParentThing->GetDef()->GetSubDef(&userDef) && userDef.Object->ExpressionDefIndex > 0;
        CExpressionUserDef* user = userDef.Object;
        if (hasExpression)
            GetDefinitionManager()->GetDef(user->ExpressionDefIndex, &PDef);
        if (user != 0)
            user->Release();
    }
}
