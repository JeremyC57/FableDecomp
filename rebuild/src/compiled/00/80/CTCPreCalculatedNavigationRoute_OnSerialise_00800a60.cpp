// CTCPreCalculatedNavigationRoute::OnSerialise(CPersistContext& context, bool) -- persist the route version (2) and the target thing's UID (taken from the intelligent pointer and remapped when saving, remapped back when loading), then save or load the route itself
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00800A60, 300 bytes, `ret 8`. CTCPreCalculatedNavigationRoute vtable 0x01242444 slot 1.
// Callees: CPersistContext::Transfer(const char*, long&, const long&) @ 0x00410BE0, CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50 (twice), CThing::IsLevelLess @ 0x004C7930, CTCBase::GetThingManager @ 0x00686AE0,
//   CThingManager::RemapUIDForSerialisation @ 0x0051E210, CPersistContext::Transfer(const char*, unsigned __int64&,
//   const unsigned __int64&) @ 0x0044BB00, SaveRoute(CPersistContext&) @ 0x008006D0, LoadRoute(CPersistContext&, long)
//   @ 0x008008B0. Names "PrecCalculatedNavigationRouteVersion" 0x012722AC, "ThingToCalculateRouteToUID" 0x01272290.
// Layout = Ego_r.pdb CTCPreCalculatedNavigationRoute (48 bytes): CTCBase 12 bytes, PThingToCalculateRouteTo
//   (CIntelligentPointer<const CThing>) +0x0C, ThingToCalculateRouteToUID +0x18, Route +0x20.
// Shape: the CTCVillageMember::OnSerialise UID block (0x007EE3D0) behind a version Transfer; the zero register (ebp) is the
//   long default, the null UID, the 64-bit default and the `false` push; the version local is passed on to LoadRoute.
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
    //   instances here: long 0x00410BE0; unsigned __int64 0x0044BB00
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
class CTCPreCalculatedNavigationRoute : public CTCBase {
public:
    CIntelligentPointer<CThing> PThingToCalculateRouteTo;   // +0x0c
    unsigned char           _pad14[0x4];
    unsigned __int64        ThingToCalculateRouteToUID; // +0x18
    unsigned char           Route[0xc];                 // +0x20 vector<pair<C2DVector, unsigned char> >
    void SaveRoute(CPersistContext& context);   // 0x008006D0
    void LoadRoute(CPersistContext& context, long version);   // 0x008008B0
    void OnSerialise(CPersistContext& context, bool);
};

void CTCPreCalculatedNavigationRoute::OnSerialise(CPersistContext& context, bool)
{
    long version = 2;
    context.Transfer<long>("PrecCalculatedNavigationRouteVersion", version, 0);
    if (context.IsSaving()) {
        if (PThingToCalculateRouteTo.GetPItem() == 0) {
            ThingToCalculateRouteToUID = 0;
        } else {
            CThing* item = PThingToCalculateRouteTo.GetPItem();
            ThingToCalculateRouteToUID = item->UID;
            ThingToCalculateRouteToUID = GetThingManager().RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, ThingToCalculateRouteToUID);
        }
        context.Transfer<unsigned __int64>("ThingToCalculateRouteToUID", ThingToCalculateRouteToUID, 0);
        SaveRoute(context);
    } else {
        context.Transfer<unsigned __int64>("ThingToCalculateRouteToUID", ThingToCalculateRouteToUID, 0);
        if (ThingToCalculateRouteToUID != 0)
            ThingToCalculateRouteToUID = GetThingManager().RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, ThingToCalculateRouteToUID);
        LoadRoute(context, version);
    }
}