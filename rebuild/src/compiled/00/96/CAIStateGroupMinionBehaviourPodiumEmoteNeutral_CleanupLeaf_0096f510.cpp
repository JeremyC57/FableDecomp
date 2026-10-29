// CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::CleanupLeaf() -- Leave creature mode 23 (0x17) on the brain's creature's mode manager
// Leaf: ?CleanupLeaf@CAIStateGroup_MinionBehaviourPodiumEmoteNeutral@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096F510, 83 bytes (+ int3 padding), thiscall.
// Identity: CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::CleanupLeaf() -- proven; evidence: it is slot 16 of vtable 0x0128D16C (RTTI .?AVCAIStateGroup_MinionBehaviourPodiumEmoteNeutral@@), the leaf-cleanup slot that CAIStateGroup_MinionBehaviourCrowdBase::Cleanup 0x00957BE0 tail-calls (`jmp [eax+0x40]`); FableWin.pdb name set holds ?CleanupLeaf@CAIStateGroup_MinionBehaviourPodiumEmoteNeutral@@UAEXXZ.
//   Family G047 (8 single-mode state-group hooks; each row's own catalog label agrees).
// Callees: CInterfaceMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x31 = bit 17 of the bitset dword at +0x24),
//   CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0 (FableWin.pdb ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z).
// Shape: the mode manager is an uninitialised local filled by the inlined GetInterface: on the hit path the lookup result
//   stays in eax, the miss path reads the never-written slot it shares with the key temporary (retail duplicates the
//   call tail); frame: one `push ecx`. Model after the landed CAIStateGroup_MinionReactToCriminal::OnActivate 0x00930DC0.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                       // a thing's sorted {type, component} vector map
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CAIBrain {
public:
    unsigned char _pad[0x18];
    CThing* Creature;                                        // +0x18 (CThingAICreature*)
};
class CTCCreatureModeManager {                               // interface type 0x31 component
public:
    bool AddMode(unsigned long mode);                        // 0x0073AE10
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();                            // slot 0
    virtual void v01();
    virtual void OnActivate();                               // slot 2  (+0x08)
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                                  // slot 9  (+0x24)
    virtual void OnReplace();                                // slot 10 (+0x28)
    CAIBrain* PBrain;                                        // +0x04
    long ScriptAIPriority;                                   // +0x08
    void* Name;                                              // +0x0c CCharString
    bool Terminated;                                         // +0x10
    unsigned char Priority;                                  // +0x11
    bool Enabled;                                            // +0x12
};
class CAIStateGroup_MinionBehaviourCrowdBase : public CAIStateGroupBase {
public:
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void CleanupLeaf();                              // slot 16 (+0x40; CrowdBase::Cleanup 0x00957BE0 tail-calls it)
};
class CAIStateGroup_MinionBehaviourPodiumEmoteNeutral : public CAIStateGroup_MinionBehaviourCrowdBase {
public:
    virtual void CleanupLeaf();                              // slot 16
};

void CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::CleanupLeaf()
{
    CTCCreatureModeManager* pModes;
    PBrain->Creature->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(23);
}