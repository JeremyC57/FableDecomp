// CThingFilter_IsOpinionSourceWithFreshDeed::operator()(CThing* source) const -- an opinion source passes when it carries a virtual deed that is not the one last seen from it, and the creature is aware of it (always-aware sources, else the nonstop awareness or the sight test)
// Leaf: ??RCThingFilter_IsOpinionSourceWithFreshDeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00960FC0, length 188 bytes, `ret 4`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x50 = CTCOpinionSource), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50, IsAwareOfThingImplementation @ 0x00864700 (`__fastcall`, `ret 4`: creature, thing, flag),
//   CanCreatureSeeCreature @ 0x008645E0 (`__fastcall`, `ret 4`; the landed callers' spelling).
// Layout: CThingFilter_IsOpinionSourceWithFreshDeed (Ego_r.pdb, 24 bytes): PCreature (CIntelligentPointer<CThingAICreature>)
//   +0, LastVDeedID +8, LastSourceUID (unsigned __int64) +0x10; retail CThing UniqueID +0x18, interface bitset +0x20 (TC
//   0x50 = word +0x28 bit 16), interface map +0x44; CTCOpinionSource UseNavLineOfSight +0x18, AlwaysAwareOf +0x75,
//   ReactNonstopFlag +0x77, VirtualDeed +0x80, VirtualDeedID +0x84.
// Shape: nested ifs with the single false exit last; HasTC/GetTC inline; the stale-deed test is the deed id compare plus a 64-bit UID equality; each awareness test is
//   an inline helper whose by-value flag parameter is homed in the dead argument slot before the creature is fetched.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

class CThing;
class CThingAICreature;
class COpinionDeed;
struct CTCOpinionSource {
    unsigned char _pad00[0x18];
    bool UseNavLineOfSight;                     // +0x18
    unsigned char _pad19[0x75 - 0x19];
    bool AlwaysAwareOf;                         // +0x75
    bool UnknownThreatSource;                   // +0x76
    bool ReactNonstopFlag;                      // +0x77
    unsigned char _pad78[0x80 - 0x78];
    COpinionDeed* VirtualDeed;                  // +0x80
    long VirtualDeedID;                         // +0x84
};

class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

bool __fastcall IsAwareOfThingImplementation(CThingAICreature* creature, CThing* thing, bool flag);   // 0x00864700
bool __fastcall CanCreatureSeeCreature(const CThing* viewer, const CThing* target, bool ignoreLight);  // 0x008645E0

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseObject* GetPItem() const;              // 0x00A01B50
    CThing* GetThing() const { return (CThing*)GetPItem(); }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetTypedItem() const { return (T*)GetThing(); }
};

struct CThingFilter_IsOpinionSourceWithFreshDeed {
    CIntelligentPointer<CThingAICreature> PCreature;    // +0x00
    long LastVDeedID;                                   // +0x08
    unsigned __int64 LastSourceUID;                     // +0x10
    bool IsAware(CThing* source, bool flag) const { return IsAwareOfThingImplementation(PCreature.GetTypedItem(), source, flag); }
    bool CanSee(CThing* source, bool flag) const { return CanCreatureSeeCreature((CThing*)PCreature.GetTypedItem(), source, flag); }
    bool operator()(CThing* source) const;
};

bool CThingFilter_IsOpinionSourceWithFreshDeed::operator()(CThing* source) const
{
    if (source->HasTC(0x50)) {
        CTCOpinionSource* opinionSource = source->GetTC<CTCOpinionSource>(0x50);
        if (opinionSource->VirtualDeed) {
            if (!(LastVDeedID == opinionSource->VirtualDeedID && LastSourceUID == source->UniqueID)) {
                if (opinionSource->AlwaysAwareOf)
                    return true;
                bool aware;
                if (opinionSource->ReactNonstopFlag)
                    aware = IsAware(source, opinionSource->UseNavLineOfSight);
                else
                    aware = CanSee(source, opinionSource->UseNavLineOfSight);
                if (aware)
                    return true;
            }
        }
    }
    return false;
}
