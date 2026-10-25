// NCreatureMode::GetUpFromSitting(CThingCreatureBase& creature, long sit_mode_type, const CCharString& anim, CAIBrain* brain) -- get the creature up: when its current mode is the sitting mode, end that mode with a CCreatureAction_GetUpFromSitting (true); otherwise, when the creature is free to perform a plain get-up action, hand a clone of it to the brain's output (or set it directly without a brain) and return false
// Leaf: ?GetUpFromSitting@NCreatureMode@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085FF10, 336 bytes incl. int3 padding, /Gr `__fastcall` (creature in ecx,
//   the mode type in edx, anim and brain on the stack), `ret 8`.
// Identity: PDB name set `?GetUpFromSitting@NCreatureMode@@YI_NAAVCThingCreatureBase@@JABVCCharString@@PAVCAIBrain@@@Z`;
//   callers 0x009388B9, 0x0094993C, 0x0096D095.
// Callees: CVectorMap::LowerBound const 0x004365B0 (PeekTC 0x31), CTCCreatureModeManager::GetCurrentMode 0x0073A6E0
//   (catalog "CPixelShader::CountReferences" propagated), the mode's virtual slot 1 GetType, CCreatureAction_GetUpFromSitting
//   ctor (creature, anim, long, bool) 0x00860150, CModeSitBase::RemoveModeWithAction 0x008D7230, ~CCombatActionBase
//   0x0062E900 (catalog "~CTCLightningOrb"), CCombatActionBase ctor (creature, long, const CSoundPair&) 0x00858030,
//   CCombatActionBase::SetCombatAnimation 0x008574D0 (catalog "CTCInGameMenu::SetPDef"), CThingCreatureBase::
//   IsFreeToPerformAction 0x00662210 / SetCurrentAction 0x006644F0, CBrainOutput ctor 0x00835620 / dtor 0x006D60C0, the
//   action's virtual slot 11 Clone, CCountedPointer<CCreatureActionBase>::Reset 0x00668D90, CAIBrain::SetOutput 0x0088B8F0
//   (catalog "CQuestManager::RemoveAvailableQuestCardType").
// Layout: CThing UsingComponents +0x20, Components +0x44; TC 0x31 CTCCreatureModeManager. CCreatureAction_GetUpFromSitting
//   (vtable 0x01276C2C) is 0x11C bytes: CCombatActionBase 0x114, the mode type +0x114, a bool +0x118. CBrainOutput 0x48
//   bytes: Types +0, PAction +0x20.
// Shape: the mode manager pointer is assigned under HasTC(0x31) into an otherwise unset local (absent: the dead key slot is
//   read); the sitting path's action is a temporary of the out-of-line 4-argument ctor; the other path's action is a
//   named local in its own block (it shares the temporary's frame slot) built by a second, __forceinline 2-argument ctor
//   (0 / false, the same base call and SetCombatAnimation as 0x00860150); the clone goes through an inline CBrainOutput
//   helper taking `const CCreatureActionBase&` (a call on the named local itself is devirtualised by VC7.1, retail keeps
//   the slot-11 virtual call).
class CCharString {
public:
    void* PStringData;
};

class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* PeekTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool IsFreeToPerformAction(const CCreatureActionBase& action);   // 0x00662210
    bool SetCurrentAction(const CCreatureActionBase& action);        // 0x006644F0
};

class CSoundPair {
public:
    long First;
    long Second;
    CSoundPair() : First(0), Second(0) { }
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CCreatureActionBase* Clone() const;             // slot 11 (+0x2c)
};

class CCombatActionBase : public CCreatureActionBase {      // retail sizeof 0x114
public:
    unsigned char _pad04[0x114 - 0x04];
    CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                                   // 0x0062E900
    void SetCombatAnimation(const CCharString& name);       // 0x008574D0
};

class CCreatureAction_GetUpFromSitting : public CCombatActionBase {   // retail sizeof 0x11c, vtable 0x01276C2C
public:
    long SitModeType;                                       // +0x114
    bool Flag;                                              // +0x118
    CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& anim, long sit_mode_type,
                                     bool flag);            // 0x00860150
    __forceinline CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& anim)
        : CCombatActionBase(creature, 0x32, CSoundPair()), SitModeType(0), Flag(false)
    {
        SetCombatAnimation(anim);
    }
    virtual CCreatureActionBase* Clone() const;
};

class CModeBase {
public:
    virtual ~CModeBase();
    virtual unsigned long GetType() const;                  // slot 1
};
class CModeSitBase : public CModeBase {
public:
    void RemoveModeWithAction(const CCreatureActionBase& action);   // 0x008D7230
};

class CTCCreatureModeManager {                              // interface type 0x31
public:
    CModeBase* GetCurrentMode() const;                      // 0x0073A6E0
};

template <class T> class CCountedPointer {
public:
    T* Object;
    void* Block;
    void Reset(T* object);                                  // 0x00668D90
};

class CBrainOutput {                                        // 0x48 bytes
public:
    unsigned long Types;                                    // +0x00
    long          LookFlags;                                // +0x04
    unsigned char NavigationInput[0x18];                    // +0x08
    CCountedPointer<CCreatureActionBase> PAction;           // +0x20
    unsigned char _pad28[0x48 - 0x28];
    CBrainOutput();                                         // 0x00835620
    void SetAsPerformAction(const CCreatureActionBase& action)   // inline (descriptive name: sets type 8 + the clone)
    {
        Types |= 8;
        PAction.Reset(action.Clone());
    }
    ~CBrainOutput();                                        // 0x006D60C0
};

class CAIBrain {
public:
    void SetOutput(const CBrainOutput& output);             // 0x0088B8F0
};

namespace NCreatureMode {

bool __fastcall GetUpFromSitting(CThingCreatureBase& creature, long sit_mode_type, const CCharString& anim, CAIBrain* brain)
{
    const CTCCreatureModeManager* mode_manager;
    if (creature.HasTC(0x31))
        mode_manager = (const CTCCreatureModeManager*)creature.PeekTC(0x31);
    CModeBase* mode = mode_manager->GetCurrentMode();
    if (mode->GetType() == sit_mode_type) {
        static_cast<CModeSitBase*>(mode)->RemoveModeWithAction(
            CCreatureAction_GetUpFromSitting(creature, anim, sit_mode_type, true));
        return true;
    }
    {
        CCreatureAction_GetUpFromSitting action(creature, anim);
        if (creature.IsFreeToPerformAction(action)) {
            if (brain != 0) {
                CBrainOutput output;
                output.SetAsPerformAction(action);
                brain->SetOutput(output);
            } else
                creature.SetCurrentAction(action);
        }
    }
    return false;
}

}