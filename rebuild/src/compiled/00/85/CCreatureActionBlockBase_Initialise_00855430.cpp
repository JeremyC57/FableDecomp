// CCreatureAction_BlockBase::Initialise() -- if the owner has the interface-0x10 sound component, play the owner's "HIT_BLOCKING" sound pair with default play parameters; then CCombatActionBase::Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855430, length 208 bytes, plain `ret` (virtual, no arguments).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (the owner pointer at +8, twice), CVectorMap::LowerBound 0x0040F020 (inlined
// GetInterface, type 0x10 = bit 16 of the dword at +0x20), CCharString(const char*, long) 0x0099EBF0 ("HIT_BLOCKING" @ 0x01275E90),
// the owner's virtual slot 89 (+0x164: the sound pair for a name, returned through a hidden pointer), the component's play call
// 0x0066B280 (manifest "CAnimateThing_PlayRandomSound"), ~CCharString 0x0099EAE0, CCombatActionBase::Initialise 0x00857520.
// Shape: identical to the landed CAIStateGroup_StealthSeekEnemy::SetWhatsThatSound 0x0093C600 (same GetInterface / implicit string
// temporary / play-parameter block {1,1,0,1,1,0,0,-1}) except the sound pair is the call's first argument (evaluated last, the
// string temporary still built first and destroyed after the play call); the parameter block is assigned in its ctor body with
// Handle before Priority (an init list in declaration order schedules the -1 store after the second 1 store).; the base Initialise is a qualified (direct) call after the block.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CSoundPair { public:
    long First;
    long Second;
};
struct CSoundPlayParams {
    float Volume;                               // +0x00
    float Pitch;                                // +0x04
    float Delay;                                // +0x08
    long Loop;                                  // +0x0c
    long Priority;                              // +0x10
    long Pan;                                   // +0x14
    long Flags;                                 // +0x18
    long Handle;                                // +0x1c
    CSoundPlayParams() { Volume = 1.0f; Pitch = 1.0f; Delay = 0.0f; Loop = 1; Pan = 0; Flags = 0; Handle = -1; Priority = 1; }
};
class CTCSoundPlayer { public:                         // the interface-0x10 component
    void PlaySound(const CSoundPair& sound, const CSoundPlayParams& params, long a, long b, long c);   // 0x0066B280
};

class CThingCreatureBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual CSoundPair GetSoundPair(const CCharString& name);   // slot 89 (+0x164)
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CIntelligentPointer<CThingCreatureBase> Owner;              // +0x08 (the owning creature)
    unsigned char _pad10[0x114 - 0x10];
    virtual void Initialise();                  // 0x00857520
};

class CCreatureAction_BlockBase : public CCombatActionBase { public:
    long BlowStrength;                          // +0x114 (PDB +0x12c)
    float MovementDirection[3];                 // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_BlockBase::Initialise()
{
    CTCSoundPlayer* player;
    if (Owner.GetPItem()->GetInterface(&player, 0x10)) {
        player->PlaySound(Owner.GetPItem()->GetSoundPair("HIT_BLOCKING"), CSoundPlayParams(), 1, 0, 0);
    }
    CCombatActionBase::Initialise();
}