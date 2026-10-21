// CTCHeroAttachableAppearanceModifiers::IsNaked() const -- true unless one of the worn type-2 appearance modifiers has an appearance-modifier sub-def with morph graphics
// Leaf: IsNaked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706290, 215 bytes (plain `ret`).
// Label: the catalog row says CTCShop::GetName (propagated); the body sits between CTCHeroAttachableAppearanceModifiers
// members (0x00706040 UpdateHiddenModifiers, 0x00706370 RemoveAppearanceModifier), reads that class's AppearanceModifiers
// vector-of-vectors at +0x30, and its only caller is CGameScriptInterface::IsHeroNaked (0x0089B330), which fetches the hero's
// interface 0x5e component and calls it. Ego_r.pdb has `?IsNaked@CTCHeroAttachableAppearanceModifiers@@QBE_NXZ` (const,
// bool, no arguments) right after UpdateHiddenMaterials in that class's member block, so the name is the PDB's.
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef(long, CDefPointer<CThingObjectDef>&)
// @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<CAppearanceModifierDef>&) @ 0x0057FF92;
// the defs' virtual slot 1 (destroy on the last release).
// Layout (retail; the PDB's 16-byte containers are 12 here, as the landed IsWearingAppearanceModifier 0x00706000):
// AppearanceModifiers +0x30 (vector< vector<CModifierEntry{DefIndex, Rendered}> >, element 2 at +0x18); CAppearanceModifierDef
// MorphGraphics (CArray<CEntry>, 12-byte entries) data at +0x34/+0x38 (the CDefClassBase part keeps the PDB 0x28 bytes).
// Shape: both def pointers are zeroed together before the GetDef call; the three tests are one `&&` chain; each CDefPointer
// destructor is a null test plus the pointee's inline `if (--RefCount == 0) Destroy();`, duplicated on the early `return false`.
// GetDef and GetSubDef are same-TU noinline stand-in bodies that write only the out slot (the defs stay in esi/ecx and
// the worn-list pointer keeps ebx). The morph-graphics count is the CArray member's own size() (`this` = def + 0x34), so
// VC7.1 cannot use the dereference to drop the modifier's null test on the early-return path.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {                 // a counted def reference (released at scope end)
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0057FF92 (CAppearanceModifierDef instantiation)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CAppearanceModifierGraphicsEntry { unsigned char _data[0x18]; };
struct CAppearanceModifierDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    struct CGraphicsArray {                                  // MorphGraphics.Graphics (CArray, 12 bytes)
        CAppearanceModifierGraphicsEntry* _Myfirst;
        CAppearanceModifierGraphicsEntry* _Mylast;
        CAppearanceModifierGraphicsEntry* _Myend;
        long size() const { return _Mylast - _Myfirst; }
    } MorphGraphics;                                          // +0x34
    long GetNumMorphGraphics() const { return MorphGraphics.size(); }
};
struct CThingObjectDef : CDefClassBase { };
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingObjectDef>& out);   // 0x0042B040
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingObjectDef>& out)
{
    void* def = Lookup_CDefinitionManager_GetDef(this, globalIndex);
    if (!def)
        return false;
    out.Object = (CThingObjectDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CModifierEntry {
    long DefIndex;
    bool Rendered;
};
struct CModifierVector {
    CModifierEntry* _Myfirst;
    CModifierEntry* _Mylast;
    CModifierEntry* _Myend;
    CModifierEntry* begin() const { return _Myfirst; }
    CModifierEntry* end() const { return _Mylast; }
};
struct CModifierVectorVector {
    CModifierVector* _Myfirst;
    CModifierVector* _Mylast;
    CModifierVector* _Myend;
    const CModifierVector& operator[](long i) const { return _Myfirst[i]; }
};
struct CThing;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
};
struct CTCHeroAttachableAppearanceModifiers : CTCBase {
    void* __vftable_0c;                                 // +0x0c secondary vptr
    unsigned long FrameLastChanged;                     // +0x10
    unsigned char Flags14[4];                           // +0x14 Changed / RecreatePrimitives / AppearanceModifiersLoaded
    unsigned char PrimHandles[0xc];                     // +0x18 vector<NGraphicAppearance::CPrimitive*>
    unsigned char HiddenMaterialNames[0xc];             // +0x24 set<CCharString>
    CModifierVectorVector AppearanceModifiers;          // +0x30
    bool IsNaked() const;
};

bool CTCHeroAttachableAppearanceModifiers::IsNaked() const
{
    const CModifierVector& worn = AppearanceModifiers[2];
    for (CModifierEntry* e = worn.begin(); e != worn.end(); ++e) {
        CDefPointer<CThingObjectDef> def;
        CDefPointer<CAppearanceModifierDef> modifier;
        if (GetDefinitionManager()->GetDef(e->DefIndex, def) && def->GetSubDef(modifier) && modifier->GetNumMorphGraphics() > 0)
            return false;
    }
    return true;
}