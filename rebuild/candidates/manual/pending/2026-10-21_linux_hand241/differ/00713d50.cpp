// NTCScriptedControl::CActionFollowCreature::SetAsSneaking(bool) -- switch the followed creature's sneaking on (adding a CTCStealth component when it has none, flagging it, and entering creature mode 7 when it was not sneaking yet) or off (clearing the flag, leaving mode 7, and removing the stealth component this action added)
// Grade: DIFFER(ours v retail) -- 395v395: scratch-register scheduling only in the sneaking-on arm (retail chains the creature load through eax and keeps the bitset word in ecx / re-reads via edx; ours puts the creature in ecx); calls, tests and the sneaking-off arm identical.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713D50, length 395 bytes (`ret 4`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("CTCStealth" @ 0x0123AB08) / ~CCharString @ 0x0099EAE0, CThing::AddTC
//          @ 0x004C9D60, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 54 = bit 22 of the bitset dword at +0x24 =
//          CTCStealth, 49 = bit 17 of +0x24 = CTCCreatureModeManager), CTCCreatureModeManager::AddMode @ 0x0073AE10 /
//          RemoveMode @ 0x0073ABA0, CThing::RemoveTC @ 0x004C9840.
// Shape: the no-change early return comes first; the rest is a __forceinline helper so VC7.1 pushes esi after that test.
//          The on-branch tests HasTC(54) twice (the mask constant is kept in esi), builds the component name as an implicit
//          conversion, re-reads the creature for each lookup (the stealth lookup is the out-parameter form whose miss path
//          reads the never-written slot; the mode-manager lookups only call on a hit); both branches end by storing Sneaking.
// Layout: Ego_r.pdb NTCScriptedControl::CActionBase {vptr, PTCScript +4 (CTCScriptedControl: CTCBase, ParentThing +4),
//          Activated +8, Terminated +9}; CActionFollowCreature PTargetThing +0xc (CScriptThing), PTCOldPhysics +0x18,
//          Sneaking +0x1c, FollowRange +0x20, AddedStealthTC +0x24, AvoidDynamicObstacles +0x25, PreviousPushableByHero +0x26,
//          Running +0x27 (retail agrees); CTCStealth::Sneaking +0xf.
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, void* params);   // 0x004C9D60
    void RemoveTC(long interfaceType);          // 0x004C9840
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCStealth {                             // interface 54
    unsigned char _pad[0x0f];
    bool Sneaking;                              // +0x0f
};
struct CTCCreatureModeManager {                 // interface 49
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCScriptedControl {                     // a CTCBase
    virtual void v00();
    CThing* ParentThing;                        // +0x04
};
struct CScriptThing { unsigned char _body[12]; };

namespace NTCScriptedControl {

struct CActionFollowCreature {
    virtual ~CActionFollowCreature();
    CTCScriptedControl* PTCScript;              // +0x04
    bool Activated;                             // +0x08
    bool Terminated;                            // +0x09
    CScriptThing PTargetThing;                  // +0x0c
    CTCBase* PTCOldPhysics;                     // +0x18
    bool Sneaking;                              // +0x1c
    float FollowRange;                          // +0x20
    bool AddedStealthTC;                        // +0x24
    bool AvoidDynamicObstacles;                 // +0x25
    bool PreviousPushableByHero;                // +0x26
    bool Running;                               // +0x27
    __forceinline void ApplySneaking(bool sneaking);
    void SetAsSneaking(bool sneaking);
};

__forceinline void CActionFollowCreature::ApplySneaking(bool sneaking)
{
    if (sneaking) {
        if (!PTCScript->ParentThing->HasTC(54)) {
            PTCScript->ParentThing->AddTC("CTCStealth", false, 0);
            AddedStealthTC = true;
        }
        CTCStealth* stealth;
        PTCScript->ParentThing->GetInterface(&stealth, 54);
        if (!stealth->Sneaking) {
            stealth->Sneaking = true;
            CTCCreatureModeManager* modes;
            if (PTCScript->ParentThing->GetInterface(&modes, 49))
                modes->AddMode(7);
        }
        Sneaking = true;
    } else {
        CTCStealth* stealth;
        PTCScript->ParentThing->GetInterface(&stealth, 54);
        stealth->Sneaking = false;
        CTCCreatureModeManager* modes;
        if (PTCScript->ParentThing->GetInterface(&modes, 49))
            modes->RemoveMode(7);
        if (AddedStealthTC) {
            PTCScript->ParentThing->RemoveTC(54);
            AddedStealthTC = false;
        }
        Sneaking = false;
    }
}

void CActionFollowCreature::SetAsSneaking(bool sneaking)
{
    if (Sneaking == sneaking)
        return;
    ApplySneaking(sneaking);
}

}
