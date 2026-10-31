// CAIStateGroup_MazeShieldedChase::IsStillRunnable() -- runnable while the target is alive, the group is not stopping and the creature has interface 0x93; without it, reset the creature's will-power indicator (interface 0xCE) to -1 and stop
// Grade: DIFFER(136v136) -- register colour only: retail keeps this in edi and &PTargetThing (then the interface map) in esi; ours swaps them
// Leaf: ?IsStillRunnable@CAIStateGroup_MazeShieldedChase@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954740, 136 bytes. Ego_r ?IsStillRunnable@CAIStateGroup_MazeShieldedChase@@
//   IAE_NXZ (protected; donor 0x00979200).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x2), CAIStateGroupBase::IsStopping() const 0x008FD010 (Ego_r
//   IBE), CVectorMap LowerBound 0x0040F020, CTCAICreatureWillPowerIndicator::Reset(float) 0x007E0E70 (Ego_r QAEXM).
// Shape: IS_THING_ALIVE (the landed 0x0095A420 helper over CIntelligentPointer<T>::GetPItem), IsStopping, a raw presence test
//   of interface 0x93, then the out-parameter GetInterface with the one-ternary pick (map pointer named) guarding Reset(-1.0f).
// Tried (this session): one-ternary GetInterface (GetInterface part now identical), landed 0x0095A420 helper + split ifs (this
//   file), unnamed map (134). Earlier: inline GetCreature(), split ifs, positive && form, template IS_THING_ALIVE, IsStillValid.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];                 // +0x4c..+0x90
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CTCAICreatureWillPowerIndicator {        // interface type 0xce
    void Reset(float value);                    // 0x007E0E70
};

class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    void* PData;                                         // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
protected:
    bool IsStopping() const;                    // 0x008FD010 (Ego_r IBE)
public:
};

struct CAIStateGroup_MazeShieldedChase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;   // +0x14 CIntelligentPointer<CThingCreatureBase>
protected:
    bool IsStillRunnable();
};

bool CAIStateGroup_MazeShieldedChase::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    if (IsStopping())
        return false;
    CThing* creature = PBrain->PCreature;
    if (!creature->HasTC(0x93)) {
        CTCAICreatureWillPowerIndicator* indicator;
        if (creature->GetInterface(&indicator, 0xce))
            indicator->Reset(-1.0f);
        return false;
    }
    return true;
}
