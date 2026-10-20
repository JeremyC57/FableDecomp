// CTCActivationTrigger::OnSerialise(CPersistContext& context, bool) -- persist the receptor's thing UID: saving takes it from the linked receptor (0 when none) and remaps it for the save; loading remaps a non-zero saved UID back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077B0D0, length 232 bytes (+ int3 padding), `ret 8`.
// Callees: CThing::IsLevelLess @ 0x004C7930, CTCBase::GetThingManager @ 0x00686AE0 (`mov eax,[0x013B8A1C] ; ret`),
// CThingManager::RemapUIDForSerialisation(bool, bool, long, unsigned __int64) @ 0x0051E210 (edx:eax result),
// CPersistContext::Transfer(const char*, unsigned __int64&, const unsigned __int64&) @ 0x0044BB00 (manifest
// "Transfer<signed_char>", a folded label); the name literal "ReceptorUID" @ 0x0126ACB0.
// Layout = Ego_r.pdb CTCActivationTrigger (32 bytes): CTCBase {vptr, ParentThing +4, flags +8}, PTCReceptor +0x0C,
// ReceptorUID +0x10, Activated +0x18; the receptor's ParentThing (+4) carries the thing UID at +0x18; retail CThing level
// short +0x8E; CPersistContext mode at +0x18 (1 or 3 = saving).
// The engine header CTCBase (vptr word, ParentThing, flags) is the base.
// Shape: the save and load arms each have their own Transfer (the default is a `0` literal bound to the const reference,
// zeroed after the pushes); the zero register ebx serves the null UID, the default and the load-arm `> 0` test (unsigned
// 64-bit: `ja`/`jbe`); the UID is assigned from the receptor first, then remapped in place.
#include "engine/CTCBase.h"
struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char _pad20[0x8e - 0x20];
    short m_level;                              // +0x8e
    bool IsLevelLess();                         // 0x004C7930
};
struct CThingManager {
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
};
class CTCActivationReceptorBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
};
class CTCActivationTrigger : public CTCBase {   // a real base keeps the vptr word 4 bytes wide next to the __int64
public:
    CTCActivationReceptorBase* PTCReceptor;     // +0x0c
    unsigned __int64 ReceptorUID;               // +0x10
    bool    Activated;                          // +0x18
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    void OnSerialise(CPersistContext& context, bool);
};

void CTCActivationTrigger::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        if (PTCReceptor == 0) {
            ReceptorUID = 0;
        } else {
            ReceptorUID = PTCReceptor->ParentThing->UID;
            ReceptorUID = GetThingManager()->RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, ReceptorUID);
        }
        context.Transfer("ReceptorUID", ReceptorUID, 0);
    } else {
        context.Transfer("ReceptorUID", ReceptorUID, 0);
        if (ReceptorUID > 0)
            ReceptorUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, ReceptorUID);
    }
}
