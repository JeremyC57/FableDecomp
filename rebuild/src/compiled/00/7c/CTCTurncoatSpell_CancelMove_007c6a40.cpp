// CTCTurncoatSpell::CancelMove() -- when the owner's special-abilities component (0xA7) is running ability 5 (turncoat), release its mutex lock and clear the current ability (any other running ability returns early); then set the will-button timer's state to 3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C6A40, length 83 bytes (+ int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inside CThing's inline GetInterface, the
//          landed 0x007504E0 / 0x007A44C0 shape).
// Naming: Ego_r.pdb has ?CancelMove@CTCTurncoatSpell@@QAEXXZ. The only caller, CTCSpecialAbilities::
//   CancelCurrentMutexAbility (0x0072B588), looks up interface 0x9E and calls this on that component; 0x9E is what
//   CTCTurncoatSpell::GetInterfaceType returns (0x007C7790, slot 21 of vtable 0x0126E5C4, RTTI .?AVCTCTurncoatSpell@@).
//   The body sits inside the CTCTurncoatSpell TU (between SetPTarget 0x007C6940 and StartTurncoatingEffects
//   0x007C6AA0). The real CTCWillEnflame::CancelMove is 0x007A44C0 (ability 3, interface 0x95). Was landed as
//   "CTCWillEnflame::CancelMove".
// Layout: CTCBase ParentThing +4. CThing component bits +0x20, interface map +0x44. CTCSpecialAbilities (interface
//   0xA7) AbilityMutexLock +0x0C, CurrentAbility +0x10 (EHeroAbility; 5 is the ability whose level the landed
//   CTCTurncoatSpell::SetPTarget 0x007C6940 reads). CTCTurncoatSpell::Timer (CWillButtonHelperTimer) +0x0C, its
//   CurrentState (EButtonHeldState) +0x14 -> +0x20 (ahead of the timer's vector, so PDB and retail agree). A local
//   model: engine/CTCTurncoatSpell.h is a flattened struct that cannot take the member function.
// Shape: the presence bit (type 0xA7 = word 5 bit 7) folds to `mov cl,[eax+34h] ; test cl,cl ; jns`; the key is the
//   `push ecx` frame slot; the component comes back through the out pointer (eax); a running ability other than 5
//   returns early, the two clears share one zero register and the `[edi+20h] = 3` store is the common tail.
// Leaf: ?CancelMove@CTCTurncoatSpell@@QAEXXZ
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long UsingComponents[9];       // +0x20 std::bitset of interface types
    CInterfaceMap Components;               // +0x44
    template <class T> bool GetInterface(long type, T** out)   // inline: the landed 0x007504E0 shape
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCSpecialAbilities { public:                // interface 0xA7
    unsigned char _pad00[0x0c];
    bool          AbilityMutexLock;         // +0x0c
    long          CurrentAbility;           // +0x10 EHeroAbility (5 = turncoat)
};

struct CWillButtonHelperTimer {             // Ego_r.pdb, 0x38 bytes; only the head is modelled
    long          ButtonHeldDownTimer;      // +0x00
    long          WillAccumulatedLevel;     // +0x04
    unsigned char PCreature[0x8];           // +0x08 CIntelligentPointer<CThing>
    long          MyAbility;                // +0x10 EHeroAbility
    long          CurrentState;             // +0x14 EButtonHeldState
};

class CTCTurncoatSpell {
public:
    void*         __vftable;                // +0x00
    CThing*       ParentThing;              // +0x04 (CTCBase)
    bool          ToBeRemoved;              // +0x08
    bool          IsInGlobalUpdateTCs;      // +0x09
    CWillButtonHelperTimer Timer;           // +0x0c
    void CancelMove();
};

void CTCTurncoatSpell::CancelMove()
{
    CTCSpecialAbilities* abilities;
    if (ParentThing->GetInterface(0xA7, &abilities)) {
        if (abilities->CurrentAbility != 5)
            return;
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    Timer.CurrentState = 3;
}
