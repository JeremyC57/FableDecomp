// CTCOwnedEntity::OnSerialise(CPersistContext& context, bool) -- persist the navigation-TC flag, a version number (1) and the owner's UID: saving writes the UID remapped for the save, loading reads it and remaps a non-zero version-1 UID back
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8460, length 297 bytes (+ int3 padding), `ret 8`. FableWin.pdb
// ?OnSerialise@CTCOwnedEntity@@MAEXAAVCPersistContext@@_N@Z.
// Callees: CPersistContext::Transfer<bool> @ 0x004045C0, Transfer<long> @ 0x00410BE0, Transfer<unsigned __int64> @ 0x0044BB00
// (each `(name, value&, const default&)`), CThing::IsLevelLess @ 0x004C7930, CTCBase::GetThingManager @ 0x00686AE0,
// CThingManager::RemapUIDForSerialisation(bool, bool, long, unsigned __int64) @ 0x0051E210 (edx:eax).
// Keys: 0x01270C04 "SwitchableNavigationTCAdded", 0x01266744 "VersionNumber", 0x01270BF8 "OwnerUID".
// Shape: the landed CTCActivationTrigger::OnSerialise (0x0077B0D0) model: the literal defaults bound to the const references
// (ebx zero register; the long default homed in the dead `context` argument slot), separate save / load arms, the save arm
// transferring a remapped copy of OwnerUID (a frame __int64) and the load arm remapping the member in place; the load
// arm's version local is left uninitialised (retail stores nothing before the Transfer).
// Layout: Ego_r.pdb CTCOwnedEntity (0x20): CTCBase {vptr, ParentThing +4, flags +8}, SwitchableNavigationTCAdded +0x0c,
// POwner +0x0d (CIntelligentPointer<CThing>, 8 bytes, unaligned in the PDB too), OwnerUID +0x18; retail CThing level short
// +0x8e; CPersistContext mode +0x18 (1 or 3 = saving).
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
class CThingManager;

class CTCBase : public CTCBaseLayout
{
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
};
class CThing { public:
    unsigned char _pad00[0x8e];
    short m_level;                              // +0x8e
public:
    bool IsLevelLess() const;   // 0x004C7930
};
class CThingManager { public:
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);                          // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);                          // 0x00410BE0
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);  // 0x0044BB00
};
class CTCOwnedEntity : public CTCBase {         // a real base keeps the vptr word 4 bytes wide next to the __int64
public:
    bool  SwitchableNavigationTCAdded;          // +0x0c
    unsigned char POwner[0x0b];                 // +0x0d CIntelligentPointer<CThing> (8 bytes) + padding
    unsigned __int64 OwnerUID;                  // +0x18
    void OnSerialise(CPersistContext& context, bool);
};

void CTCOwnedEntity::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("SwitchableNavigationTCAdded", SwitchableNavigationTCAdded, false);
    if (context.IsSaving()) {
        long version = 1;
        context.Transfer("VersionNumber", version, 0L);
        unsigned __int64 uid = GetThingManager().RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, OwnerUID);
        context.Transfer("OwnerUID", uid, 0);
    } else {
        long version;
        context.Transfer("VersionNumber", version, 0L);
        context.Transfer("OwnerUID", OwnerUID, 0);
        if (version == 1 && OwnerUID != 0)
            OwnerUID = GetThingManager().RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, OwnerUID);
    }
}