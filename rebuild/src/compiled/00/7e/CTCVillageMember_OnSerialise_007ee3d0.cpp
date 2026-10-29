// CTCVillageMember::OnSerialise(CPersistContext& context, bool) -- persist the member's village UID: saving takes it from the village intelligent pointer (0 when none) and remaps it for the save; loading remaps a non-zero saved UID back
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EE3D0, 242 bytes, `ret 8`. CTCVillageMember vtable 0x0124007C slot 1 (the manifest
//   label "CTCPreCalculatedNavigationRoute::OnSerialise" is propagated; the route's own OnSerialise is 0x00800A60).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CThing::IsLevelLess @ 0x004C7930,
//   CTCBase::GetThingManager @ 0x00686AE0, CThingManager::RemapUIDForSerialisation(bool, bool, long, unsigned __int64)
//   @ 0x0051E210, CPersistContext::Transfer(const char*, unsigned __int64&, const unsigned __int64&) @ 0x0044BB00 (manifest
//   "Transfer<signed_char>", a folded label); the name literal "VillageUID" @ 0x0127172C.
// Layout = Ego_r.pdb CTCVillageMember (32 bytes): CTCBase {vptr, ParentThing +4, flags +8}, PDef +0x0C,
//   PVillage (CIntelligentPointer<CThing>) +0x10, VillageUID +0x18; the village thing's UID at +0x18; CThing level +0x8E.
// Shape: same as the landed CTCActivationTrigger::OnSerialise (0x0077B0D0): the save arm calls GetPItem once for the null
//   test and once for the UID; the load arm tests the UID with `!= 0` (`or ecx,edx ; je`), and ebx (zero) also serves the
//   `false` push and the Transfer default.
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
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char _pad20[0x8e - 0x20];
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
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: unsigned __int64 0x0044BB00
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
class CVillageMemberDef;
class CTCVillageMember : public CTCBase {
public:
    CVillageMemberDef*      PDef;               // +0x0c
    CIntelligentPointer<CThing> PVillage;           // +0x10
    unsigned __int64        VillageUID;         // +0x18
    void OnSerialise(CPersistContext& context, bool);
};

void CTCVillageMember::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        if (PVillage.GetPItem() == 0) {
            VillageUID = 0;
        } else {
            CThing* item = PVillage.GetPItem();
            VillageUID = item->UID;
            VillageUID = GetThingManager().RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, VillageUID);
        }
        context.Transfer<unsigned __int64>("VillageUID", VillageUID, 0);
    } else {
        context.Transfer<unsigned __int64>("VillageUID", VillageUID, 0);
        if (VillageUID != 0)
            VillageUID = GetThingManager().RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, VillageUID);
    }
}