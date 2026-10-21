// CThingCreatureBase::GetRandomAnimFromName(const CCharString& name, long exclude) const -- a random appearance animation called `name` (not the excluded one) passing the current creature mode's filter, else a random one of any group, else the first "DEFAULT"
// Leaf: ?GetRandomAnimFromName@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662D80, length 356 bytes (+12 int3), `ret 8`.
//   Signature from Ego_r.pdb `?GetRandomAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z`.
// Callees: as the landed sibling GetAnimFromName 0x00662A00: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2,
//   CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 and the mode's virtual slot 4 (+0x10, its animation filter), the
//   animation set's "has a matching animation" test @ 0x005DC2E0, the random pick @ 0x005DCC00 (name, seed, filter,
//   exclude), CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&) @ 0x005DC340, the game random
//   seed getter CThing::GetRandomSeed @ 0x004C7AE0, CCharString(const char*, long) @ 0x0099EBF0 ("DEFAULT" @ 0x01230554),
//   ~CCharString @ 0x0099EAE0, the def's virtual slot 1 (+4) at a zero count.
// Identity: CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store 0x0122FFCC.
// Layout: retail CThing PBaseDef +0x70; retail CThingCreatureBase PTCModeManager +0x134 (PDB +0x158); CAppearanceDef
//   Animation +0x34; a counted def holds its count at +4.
// Shape: the sub-def is fetched before the mode's filter (the reverse of the sibling); the "DEFAULT" fallback first runs
//   a has-animation test whose result is discarded (retail calls 0x005DC2E0 and ignores al), then the lookup.
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
    CAnimationEntry* GetRandomAnimation(const CCharString& name, unsigned long* seed, const CAnimFilterBase& filter,
                                        long exclude) const;                                                     // 0x005DCC00
};
struct CCreatureModeBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CAnimFilterBase& GetAnimFilter();      // slot 4 (+0x10)
};
struct CTCCreatureModeManager {
    CCreatureModeBase* GetCurrentMode();        // 0x0073A6E0
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
    unsigned char  _pad74[0x134 - 0x74];
    CTCCreatureModeManager* PTCModeManager;     // +0x134
    unsigned long* GetRandomSeed() const;       // 0x004C7AE0
    const CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long exclude) const;
};

const CAnimationEntry& CThingCreatureBase::GetRandomAnimFromName(const CCharString& name, long exclude) const
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(out);
    const CAnimFilterBase& filter = PTCModeManager->GetCurrentMode()->GetAnimFilter();
    CHeldDef<CAppearanceDef> def(out.m_pObject);
    const CAnimationSet& animations = def->Animation;
    if (animations.HasAnimation(name, filter))
        return *animations.GetRandomAnimation(name, GetRandomSeed(), filter, exclude);
    if (animations.HasAnimation(name, CAnimFilterNoGroup()))
        return *animations.GetRandomAnimation(name, GetRandomSeed(), CAnimFilterNoGroup(), exclude);
    animations.HasAnimation("DEFAULT", CAnimFilterNoGroup());
    return *animations.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
}
