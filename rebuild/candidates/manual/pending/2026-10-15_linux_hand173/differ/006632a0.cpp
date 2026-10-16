// CThingCreatureBase::IsCombatAnimationValid(long animation) -- does the creature's appearance animation set hold this animation (no group filter)
// Grade: DIFFER(183v156) -- lane gap (GOTCHAS retry 5: CDefPointer release without re-read): each of the three def releases is retail `mov ecx,[slot] ; test ; je ; dec [ecx+4] ; jne`, ours re-reads the slot and the count after the dec (inline dtor) or takes the ReleaseTemporary `add eax,4 ; dec edx ; mov [eax],edx` form (Release() member). Control flow, the block-scoped second def and the filter temporary match.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006632A0, 154 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (twice; byte-twin label GetSubDef<CAbilityDef>),
//   CAnimationSet::HasAnimation(long, const CAnimFilter&) @ 0x005DC2E0 (auto label "CAnimationDef_FindAnimationByName").
// Identity: the frame word 0x0122FFDC is a CAnimFilterNoGroup temporary (RTTI locator at vtable-4).
// Layout: retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34; def ref count +4, virtual Destroy slot 1.
// Shape: the first GetSubDef only tests that the creature has an appearance def; the animation set is then taken from a
//   second CDefPointer in its own block, released (inline dtor) before the set is used; each return arm releases the
//   first def through its own inline dtor copy.
struct CAnimationEntry;
struct CAnimFilter {
    virtual bool Accept(const CAnimationEntry* entry) const = 0;
    virtual CAnimFilter* Clone() const = 0;
};
struct CAnimFilterNoGroup : CAnimFilter {      // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilter* Clone() const;                         // 0x00429A2F
};
struct CAnimationSet {
    bool HasAnimation(long animation, const CAnimFilter& filter) const;     // 0x005DC2E0
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CAppearanceDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --((CDefClassBase*)Object)->RefCount == 0)
            ((CDefClassBase*)Object)->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CThingBaseDef {
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;      // 0x0042B0A2
};

struct CThingCreatureBase {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
    bool IsCombatAnimationValid(long animation);
};

bool CThingCreatureBase::IsCombatAnimationValid(long animation)
{
    CDefPointer<const CAppearanceDef> def;
    if (PBaseDef->GetSubDef(&def)) {
        const CAnimationSet* set;
        {
            CDefPointer<const CAppearanceDef> appearance;
            PBaseDef->GetSubDef(&appearance);
            set = &appearance->Animation;
        }
        if (set->HasAnimation(animation, CAnimFilterNoGroup()))
            return true;
    }
    return false;
}
