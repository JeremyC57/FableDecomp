// CTCStealth::StartSneaking() -- once: put the creature's mode manager into mode 7 (sneaking) and mark the component as sneaking
// Leaf: StartSneaking
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747560, length 81 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCCreatureModeManager::AddMode(unsigned long)
// @ 0x0073AE10.
// The parent thing's interface bitset at +0x20 (type 0x31 -> word 1 = +0x24, bit 17 = 0x20000) and sorted interface map
// at +0x44: the landed out-parameter GetInterface shape of 0x007504E0.
// Layout: Ego_r.pdb CTCStealth with the retail 12-byte vector (as the landed ~CTCStealth 0x007495D0): Sneaking +0x0f,
// PDef +0x1c, RunningAwayFromCombatTimer +0x30, InDangerCoolOffTimer +0x34, DangerThings first/last +0x38/+0x3c
// (16-byte pair<CIntelligentPointer<CThing const>, CDangerStatus> elements).
// Name and signature confirmed by ghidra_out/egor_signatures.tsv (Ego_r).
// Shape: the flag store is shared by both arms of the interface test (it follows the inner if).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                             // +0x44
    template <class T>
    __forceinline bool GetInterface(long type, T** out)     // inline: the landed 0x007504E0 GetInterface shape
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode);                       // 0x0073AE10
};

struct CDangerThing {                                       // pair<CIntelligentPointer<CThing const>, NStealth::CDangerStatus>, 16 bytes
    struct CPointer {
        void*   m_vtable;
        void*   m_pItem;
        CThing* GetPItem() const;                           // CBaseIntelligentPointer::GetPItem @ 0x00A01B50
    } first;                                                // +0x00
    long Status;                                            // +0x08
    long Timer;                                             // +0x0c
};

class CStealthDef { public:
    unsigned char _pad[0x44];
    float         RunningAwayFromCombatHangtimeSeconds;     // +0x44
};

class CTCStealth { public:
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    bool          Viewable;                                 // +0x0c
    bool          Smellable;                                // +0x0d
    bool          Hearable;                                 // +0x0e
    bool          Sneaking;                                 // +0x0f
    bool          SneakingToggleSet;                        // +0x10
    float         DistanceCanBeHeardFrom;                   // +0x14
    float         MaxSneakingSpeed;                         // +0x18
    CStealthDef*  PDef;                                     // +0x1c CDefPointer<CStealthDef const>
    long          FrameStartedWalking;                      // +0x20
    long          FrameStartedSneakingAndMoving;            // +0x24
    long          SneakingAndMovingDeedHandle;              // +0x28
    long          ActionPermittedToSneakID;                 // +0x2c
    long          RunningAwayFromCombatTimer;               // +0x30
    long          InDangerCoolOffTimer;                     // +0x34
    CDangerThing* m_pDangerFirst;                           // +0x38 DangerThings (retail 12-byte vector)
    CDangerThing* m_pDangerLast;                            // +0x3c
    CDangerThing* m_pDangerEnd;                             // +0x40
    long          MaxNumberOfDangerThings;                  // +0x44
    long          RegionBasedMaxNumberOfDangerThings;       // +0x48
    void StartSneaking();
};

void CTCStealth::StartSneaking()
{
    if (!Sneaking) {
        CTCCreatureModeManager* modes;
        if (ParentThing->GetInterface(0x31, &modes))
            modes->AddMode(7);
        Sneaking = true;
    }
}