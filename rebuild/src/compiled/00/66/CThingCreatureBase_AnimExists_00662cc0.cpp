// CThingCreatureBase::AnimExists(const CCharString& name, bool useModeFilterOnly) const -- vtable slot 73: whether the appearance animation set has an animation called `name` passing the current creature mode's filter, else (unless restricted to the mode filter) one of any group
// Leaf: ?AnimExists@CThingCreatureBase@@UBE_NABVCCharString@@_N@Z
// Identity (sweep3; was CThingPlayerCreature::vfunc_73): Ego_r.pdb / FableWin.pdb ?AnimExists@CThingCreatureBase@@UBE_NABVCCharString@@_N@Z,
//   the public virtual whose (const CCharString&, bool) -> bool signature this body has; CThingPlayerCreature has no AnimExists
//   in either PDB, so its slot 73 is the inherited base body, while the override ?AnimExists@CThingAICreature@@MBE... is slot 73
//   of the CThingAICreature vtable 0x0127293C (0x00830F20, the same lookup written differently); the body sits in the
//   CThingCreatureBase run next to GetRandomAnimFromNameWithFilter 0x00662EF0 (Ego_r ?...@CThingCreatureBase@@QBE...), which
//   calls the same CAnimationSet::AnimExists 0x005DC2E0 (Ego_r ?AnimExists@CAnimationSet@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662CC0, length 181 bytes, `ret 8`, bool in al; slot 73 of vtable
//   0x012457FC = CThingPlayerCreature (rtti-vtables.tsv; inherited from CThingCreatureBase).
// Callees: CThingBaseDef::GetSubDef<CAppearanceDef> @ 0x0042B0A2, CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0,
//   the mode's virtual slot 4 (+0x10, its animation filter; name unproven), the animation set's "has a matching
//   animation" test @ 0x005DC2E0 (as in GetRandomAnimFromNameWithFilter 0x00662EF0), the def's virtual Destroy (slot 1).
// Identity: the fallback filter is CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store
//   0x0122FFCC (the landed CTCAnimationComplex::OnCreate model).
// Layout: retail CThing PBaseDef +0x70; retail CThingCreatureBase PTCModeManager +0x134 (PDB +0x158); CAppearanceDef
//   Animation +0x34.
// Shape: the sub-def out pointer is the zeroed `push ecx` word; the mode is fetched before the def is copied out of it
//   into a small holder whose destructor releases it (null-tested, inline at every return); the fallback filter
//   temporary lands in the dead flag slot and the result rides in bl across the release;
//   the restricted `return false` is written last (its block is laid out last).
class CCharString;
struct CAnimationEntry;
class CAnimFilterBase {                         // vtable 0x0122FFCC
public:
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
class CAnimFilterNoGroup : public CAnimFilterBase {   // vtable 0x0122FFDC
public:
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
class CAnimationSet {
public:
    bool AnimExists(const CCharString& name, const CAnimFilterBase& filter) const;     // 0x005DC2E0
};

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CHeldDef {            // the def copied out of the escaped out pointer, released at scope end
    T* Object;
    CHeldDef(T* object) : Object(object) { }
    ~CHeldDef() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};

struct CCreatureModeBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CAnimFilterBase& GetAnimFilter_slot4();       // slot 4 (+0x10)
};
struct CTCCreatureModeManager {
    CCreatureModeBase* GetCurrentMode();        // 0x0073A6E0
};

class CThingCreatureBase {
public:
    // vptr +0x00 (AnimExists is vtable slot 73)
    unsigned char _pad04[0x70 - 4];
    CThingBaseDef* PBaseDef;                    // +0x70 CDefPointer<CThingBaseDef const>::Object
    unsigned char _pad74[0x134 - 0x74];
    CTCCreatureModeManager* PTCModeManager;     // +0x134
    virtual bool AnimExists(const CCharString& name, bool useModeFilterOnly) const;   // slot 73
};

bool CThingCreatureBase::AnimExists(const CCharString& name, bool useModeFilterOnly) const
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(&out);
    CCreatureModeBase* mode = PTCModeManager->GetCurrentMode();
    CHeldDef<CAppearanceDef> def(out.Object);
    CAnimationSet& animations = def->Animation;
    if (animations.AnimExists(name, mode->GetAnimFilter_slot4()))
        return true;
    if (!useModeFilterOnly)
        return animations.AnimExists(name, CAnimFilterNoGroup());
    return false;
}