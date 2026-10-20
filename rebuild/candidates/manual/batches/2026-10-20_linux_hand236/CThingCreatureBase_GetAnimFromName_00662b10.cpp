// CThingCreatureBase::GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index) -- the index-th appearance animation called `name` passing the filter, else one of any group, else the first "DEFAULT"
// Leaf: ?GetAnimFromName@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662B10, 242 bytes (+14 int3), `ret 0Ch`.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (byte-twin label GetSubDef<CAbilityDef>), the animation
//   set's "has a matching animation" test @ 0x005DC2E0 and CAnimationSet::GetAnimation(const CCharString&, long, const
//   CAnimFilterBase&) @ 0x005DC340, CCharString(const char*, long) @ 0x0099EBF0 ("DEFAULT" @ 0x01230554), ~CCharString
//   @ 0x0099EAE0, the def's virtual slot 1 (+4) at a zero count.
// Identity: the fallback filter is CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store
//   0x0122FFCC (the landed CThingPlayerCreature::vfunc_73 0x00662CC0 model). The overload with a long flag is 0x00662A00.
// Layout: retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34; a counted def holds its count at +4.
// Shape: the sub-def out pointer is the zeroed `push ecx` word, copied into a holder whose destructor releases it at
//   each of the three returns (the landed vfunc_73 model); the fallback filters are temporaries in the dead filter
//   argument slot (the base-vtable store is their destructor); "DEFAULT" is passed as a plain literal so its implicit
//   CCharString temporary is built before the other arguments are pushed.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CAnimationEntry;
struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
struct CAnimationSet {
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;                       // 0x005DC2E0
    CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
struct CDefPointeeBase {                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> struct CDefPointer {          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};
template <class T> struct CHeldDef {            // the def copied out of the escaped out pointer, released at scope end
    T* Object;
    CHeldDef(T* object) : Object(object) { }
    ~CHeldDef() { if (Object) Object->ReleaseDef(); }
    T* operator->() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x2c];
    CAnimationSet Animation;                    // +0x34
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);       // 0x0042B0A2
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

struct CThingCreatureBase {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
    CAnimationEntry* GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index);
};

CAnimationEntry* CThingCreatureBase::GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index)
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(out);
    CHeldDef<CAppearanceDef> def(out.m_pObject);
    const CAnimationSet& animations = def->Animation;
    if (animations.HasAnimation(name, filter))
        return animations.GetAnimation(name, index, filter);
    if (animations.HasAnimation(name, CAnimFilterNoGroup()))
        return animations.GetAnimation(name, index, CAnimFilterNoGroup());
    return animations.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
}
