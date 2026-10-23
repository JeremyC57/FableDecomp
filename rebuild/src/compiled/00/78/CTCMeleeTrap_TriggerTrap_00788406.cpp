// CTCMeleeTrap::TriggerTrap() -- fires the trap: starts its (non-looping) trigger animation and hands the trap the durations of its trigger animation and, when the def has one, its second animation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788406, length 239 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CDefClassBase::GetSubDef<CMeleeTrapDef>(CDefPointer<T>*) @ 0x0042B0A2 (catalog GetSubDef<CAbilityDef>, a byte twin; on
// CThing::PDef +0x70, no addref), CTCMeleeTrap::InitialiseAnimation(const CCharString&, bool) @ 0x0078809B, the def's animation table
// (+0x34): GetAnimation(const CCharString&, long, const CAnimFilterBase&) const @ 0x005DC340 and HasAnimation(const CCharString&,
// const CAnimFilterBase&) const @ 0x005DC2E0 (catalog "Animation_FindNthValid" / "CAnimationDef_FindAnimationByName"), CTCBase::GetWorld
// @ 0x00686A70, CMeshDataBank::GetAnimation(long) @ 0x00A242C0 on CWorld::PMainMeshBank (+0x3C; the info's duration at +0x10),
// CTCMeleeTrap::SetStrikeTimes(float, float) @ 0x00787C35 (catalog "ConstructString", propagated), the def's virtual slot 1 on release.
// Globals: the two animation names 0x013BADF8 and 0x013BADFC (CCharString objects); the filter vtables 0x0122FFDC / 0x0122FFCC.
// Layout: ParentThing +0x04; the filter temporary is one vptr word ([ebp-0Ch]) whose derived destructor leaves the base vtable; `this`
// is spilled to [ebp-8]; the two durations live in [ebp-14h] and [ebp-10h] (the second defaults to 0.0f via `fldz`).
// Shape: the def out slot is zeroed and filled before the animation starts; the filter temporary is rebuilt for each table call and
// the entry's index (its first word) feeds the mesh bank lookup; the durations are pushed through `fstp` onto the argument stack.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
};
extern CCharString g_meleeTrapTriggerAnimName;       // 0x013BADF8
extern CCharString g_meleeTrapSecondAnimName;        // 0x013BADFC
struct CAnimFilterBase {                             // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
};
struct CAnimFilterAll : CAnimFilterBase {            // vtable 0x0122FFDC: accepts every entry
    virtual ~CAnimFilterAll() { }
};
struct CAnimationEntry {
    long AnimationIndex;                             // +0x00
};
struct CAnimationTable {
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;                                 // 0x005DC2E0
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
    __forceinline void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {              // the out slot the definition lookups fill (released by hand)
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
struct CMeleeTrapDef : CDefPointeeBase {
    unsigned char   _pad08[0x34 - 0x08];
    CAnimationTable Animations;                      // +0x34
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CMeleeTrapDef>* out) const;   // 0x0042B0A2
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
};
struct C3DAnimationInfo {
    unsigned char _pad00[0x10];
    float Duration;                                  // +0x10
};
struct CMeshDataBank {
    C3DAnimationInfo* GetAnimation(long index);      // 0x00A242C0
};
struct CMeshDataBankPointer {                        // CCountedPointer<CMeshDataBank>
    CMeshDataBank* Data;
    void*          Info;
};
struct CWorld {
    unsigned char        _pad00[0x3c];
    CMeshDataBankPointer PMainMeshBank;              // +0x3c
};

struct CTCMeleeTrap {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    CWorld* GetWorld();                              // 0x00686A70 (CTCBase)
    void InitialiseAnimation(const CCharString& animationName, bool loop);   // 0x0078809B
    void SetStrikeTimes(float trigger, float second);   // 0x00787C35
    void TriggerTrap();
};
// R&D I-3 sweep: a same-TU out-of-line GetSubDef (writes only *out) settles the def null store, the push order and the
// /Os release shape as in retail. The sub-def search itself is the declaration-only helper below.
CMeleeTrapDef* FindMeleeTrapSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CMeleeTrapDef>* out) const
{
    out->Object = FindMeleeTrapSubDef(this);
    return out->Object != 0;
}
#pragma auto_inline(on)
void CTCMeleeTrap::TriggerTrap()
{
    CDefPointer<CMeleeTrapDef> def;
    ParentThing->PDef->GetSubDef(&def);
    InitialiseAnimation(g_meleeTrapTriggerAnimName, false);
    const CAnimationEntry* trigger = def->Animations.GetAnimation(g_meleeTrapTriggerAnimName, 0, CAnimFilterAll());
    CMeshDataBank* bank = GetWorld()->PMainMeshBank.Data;
    float triggerTime = bank->GetAnimation(trigger->AnimationIndex)->Duration;
    float secondTime = 0.0f;
    if (def->Animations.HasAnimation(g_meleeTrapSecondAnimName, CAnimFilterAll())) {
        const CAnimationEntry* second = def->Animations.GetAnimation(g_meleeTrapSecondAnimName, 0, CAnimFilterAll());
        CMeshDataBank* secondBank = GetWorld()->PMainMeshBank.Data;
        secondTime = secondBank->GetAnimation(second->AnimationIndex)->Duration;
    }
    SetStrikeTimes(triggerTime, secondTime);
}