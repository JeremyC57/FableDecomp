// CTCHeroAttachableAppearanceModifiers::IsNaked() const -- true unless one of the worn type-2 appearance modifiers has an appearance-modifier sub-def with morph graphics
// Grade: DIFFER(233v215) -- the lane hoists the out-slot zero into ebx (cmp al,bl tests) and spills the worn-list pointer that retail keeps in ebx
// Leaf: IsNaked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706290, 215 bytes (plain `ret`).
// Label: the catalog row says CTCShop::GetName (propagated); the body sits between CTCHeroAttachableAppearanceModifiers
// members (0x00706040 UpdateHiddenModifiers, 0x00706370 AddAppearanceModifier), reads that class's AppearanceModifiers
// vector-of-vectors at +0x30, and its only caller is CGameScriptInterface::IsHeroNaked (0x0089B330), which fetches the hero's
// interface 0x5e component and calls it -- named IsNaked here (the PDB name is not known).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef(long, CDefPointer<CThingObjectDef>&)
// @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<CAppearanceModifierDef>&) @ 0x0057FF92;
// the defs' virtual slot 1 (destroy on the last release).
// Layout (retail; the PDB's 16-byte containers are 12 here, as the landed IsWearingAppearanceModifier 0x00706000):
// AppearanceModifiers +0x30 (vector< vector<CModifierEntry{DefIndex, Rendered}> >, element 2 at +0x18); CAppearanceModifierDef
// MorphGraphics (CArray<CEntry>, 12-byte entries) data at +0x34/+0x38 (the CDefClassBase part keeps the PDB 0x28 bytes).
// Shape: both def pointers are zeroed together before the GetDef call; the three tests are one `&&` chain; each CDefPointer
// destructor is a null test plus the pointee's inline `if (--RefCount == 0) Destroy();`, duplicated on the early `return false`.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {                 // the out slot (released by the holder below)
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0057FF92 (CAppearanceModifierDef instantiation)
};
struct CAppearanceModifierGraphicsEntry { unsigned char _data[0x18]; };
struct CAppearanceModifierDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAppearanceModifierGraphicsEntry* MorphGraphics_first;   // +0x34 MorphGraphics.Graphics (CArray)
    CAppearanceModifierGraphicsEntry* MorphGraphics_last;    // +0x38
    long GetNumMorphGraphics() const { return MorphGraphics_last - MorphGraphics_first; }
};
struct CThingObjectDef : CDefClassBase { };
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingObjectDef>& out);   // 0x0042B040
};
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
        CDefPointer<CThingObjectDef> defOut;
        CDefPointer<CAppearanceModifierDef> modifierOut;
        bool found = GetDefinitionManager()->GetDef(e->DefIndex, defOut);
        CDefHolder<CThingObjectDef> def(defOut.Object);
        if (found) {
            bool hasModifier = def->GetSubDef(modifierOut);
            CDefHolder<CAppearanceModifierDef> modifier(modifierOut.Object);
            if (hasModifier && modifier->GetNumMorphGraphics() > 0)
                return false;
        }
    }
    return true;
}
