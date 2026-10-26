// NCreatureMode::CModeHoverBase::OnDeactivate() -- vtable slot 6 (inherited by CModeWalkInAllDirections): the base deactivation (mode no longer active), then the hover particle effect is killed if it is still alive
// Leaf: ?OnDeactivate@CModeHoverBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EA9B0, 53 bytes (+11 int3 padding).
// Name: FableWin.pdb ?OnDeactivate@CModeHoverBase@NCreatureMode@@UAEXXZ. The catalog label CModeWalkInAllDirections::vfunc_6
// comes from slot 6 of the vtable 0x012687AC (rtti-vtables.tsv), whose slot 2 is the landed CModeHoverBase::AnimUpdate
// 0x008EAC30: CModeWalkInAllDirections declares no OnDeactivate (PDB), so slot 6 is the hover base's.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThing::Kill(bool) @ 0x004C9B80.
// Layout: CTCCreatureModeManager::CModeBase Active +0x18 (as the landed CModeBase::FrameUpdate 0x0073AC50);
// CModeHoverBase PParticleEffect (CIntelligentPointer<CThing>) +0x38 = Ego_r.pdb +0x44 less the 4-byte-smaller retail
// HoverStates vector and the 8-byte-smaller retail AnimQueue list. Dead = bit 0 of the thing's +0x91.
// Shape: the inline CModeBase::OnDeactivate (`Active = false`, a byte store before anything else), then the landed
// IS_THING_ALIVE(const CBaseIntelligentPointer&) inline from CModeOutOfActionBase::OnDeactivate 0x008D8F30 (the
// pointer's address in esi for all three GetPItem calls) and `Kill(true)`. Plain `ret`.
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;               // +0x91 bit 0: dead
    void Kill(bool immediately);           // 0x004C9B80
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

namespace NCreatureMode {

struct CModeBase {                         // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThing*       ParentThing;             // +0x04
    unsigned char WeaponBasedFilter[0xc];  // +0x08 CAnimFilterOfGroup
    long          FramesBeforeRemovingSelf;   // +0x14
    bool          Active;                  // +0x18
    virtual void OnDeactivate() { Active = false; }
};

struct CModeHoverBase : CModeBase {
    unsigned char           _pad1c[0x38 - 0x1c];   // HoverStates, CurrentName, PCurrentState, CurrentMovementType, AnimQueue
    CIntelligentPointer<CThing> PParticleEffect;       // +0x38
    virtual void OnDeactivate();
};

void CModeHoverBase::OnDeactivate()
{
    CModeBase::OnDeactivate();
    if (IS_THING_ALIVE(PParticleEffect))
        PParticleEffect.GetPItem()->Kill(true);
}

} // namespace NCreatureMode