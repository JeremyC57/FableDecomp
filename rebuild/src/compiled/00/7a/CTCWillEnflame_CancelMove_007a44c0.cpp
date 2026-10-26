// CTCWillEnflame::CancelMove() -- when the owner's special-abilities component (0xA7) is running ability 3 (enflame), release its mutex lock and clear the current ability (any other running ability returns early); then remove this TC (interface 0x95) from the owner
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A44C0, 86 bytes (+10 int3), `ret`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inside CThing's inline GetInterface, the
//          landed 0x007504E0 / 0x007C6A40 shape), CThing::RemoveTC(long) @ 0x004C9840.
// Naming: Ego_r.pdb has ?CancelMove@CTCWillEnflame@@QAEXXZ; CTCWillEnflame's GetInterfaceType (vtable 0x0126C824 slot
//   21, 0x007A5040) returns 0x95, the type this body removes, and all three callers (0x0069F79E, 0x0072B502,
//   0x008BA694) look up interface 0x95 and call it on that component. The catalog label "CTCEnflame_ResetState" is
//   descriptive. (The landed 0x007C6A40 "CTCWillEnflame::CancelMove" is reached through interface 0x9E =
//   CTCTurncoatSpell, see NOTES.)
// Layout: CTCBase ParentThing +4. CThing component bits +0x20, interface map +0x44; CTCSpecialAbilities (interface
//   0xA7, landed 0x008CDA30 model) AbilityMutexLock +0x0C, CurrentAbility +0x10 (EHeroAbility, 3 = enflame).
// Shape: the parent read once into a local (edi, reused as RemoveTC's `this`); the presence bit (0xA7 = word 5 bit 7)
//   folds to `mov al,[edi+34h] ; test al,al ; jns`; the out-pointer GetInterface leaves the component in eax; the two
//   clears share one zero register; a running ability other than 3 returns before RemoveTC.
// Leaf: ?CancelMove@CTCWillEnflame
class CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long UsingComponents[9];       // +0x20 std::bitset of interface types
    CInterfaceMap Components;               // +0x44
    template <class T> bool GetInterface(long type, T** out)   // inline: the landed 0x007504E0 shape
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    void RemoveTC(long interfaceType);      // 0x004C9840
};

class CTCSpecialAbilities { public:                // interface 0xA7
    unsigned char _pad00[0x0c];
    bool          AbilityMutexLock;         // +0x0c
    long          CurrentAbility;           // +0x10 EHeroAbility
};

class CTCWillEnflame { public:
    void*         __vftable;                // +0x00
    CThing*       ParentThing;              // +0x04 (CTCBase)
    void CancelMove();
};

void CTCWillEnflame::CancelMove()
{
    CThing* parent = ParentThing;
    CTCSpecialAbilities* abilities;
    if (parent->GetInterface(0xA7, &abilities)) {
        if (abilities->CurrentAbility != 3)
            return;
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    parent->RemoveTC(0x95);
}