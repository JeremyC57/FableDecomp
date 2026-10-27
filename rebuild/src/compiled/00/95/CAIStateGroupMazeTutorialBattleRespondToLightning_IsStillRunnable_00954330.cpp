// CAIStateGroup_MazeTutorialBattleRespondToLightning::IsStillRunnable() -- the maze target thing exists and is not dead, and the state group is not stopping
// Leaf: ?IsStillRunnable@CAIStateGroup_MazeTutorialBattleRespondToLightning@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00954330, 55 bytes, thiscall, `ret`.
// Identity (probable): the third of the four identical IsStillRunnable bodies in the maze TU; the remaining Ego_r.pdb owners are CAIStateGroup_MazeTutorialBattleRespondToLightning and ...UseShield, in that order in the TU's Initialise sequence (0x00954040 / 0x00954080); no direct caller; Ego_r.pdb ?IsStillRunnable@CAIStateGroup_MazeTutorialBattleRespondToLightning@@IAE_NXZ (RTTI base CAIStateGroup_MazeBase, PTargetThing +0x14).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice), CAIStateGroupBase::IsStopping() const 0x008FD010 (Ego_r.pdb IBE).
// Header debt: engine/CAIStateGroup_MazeTutorialBattleRespondToLightning.h / CAIStateGroup_MazeBase.h / CAIStateGroupBase.h are flat layouts; modelled locally.
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                         // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                                 // +0x91 bit 0: dead
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad04[0x10];
protected:
    bool IsStopping() const;                               // 0x008FD010
};
class CAIStateGroup_MazeBase : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThing> PTargetThing;              // +0x14
};
class CAIStateGroup_MazeTutorialBattleRespondToLightning : public CAIStateGroup_MazeBase {
protected:
    bool IsStillRunnable();
};
bool CAIStateGroup_MazeTutorialBattleRespondToLightning::IsStillRunnable()
{
    if (IS_THING_ALIVE(PTargetThing) && !IsStopping())
        return true;
    return false;
}