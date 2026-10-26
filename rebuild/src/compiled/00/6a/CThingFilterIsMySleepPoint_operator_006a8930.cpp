// CThingFilter_IsMySleepPoint::operator()(CThing* thing) const -- a thing is the sleeper's sleep point when it is a single-person occupiable of occupy type 2 that is either unowned or owned by the sleeper
// Leaf: ??RCThingFilter_IsMySleepPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8930, length 162 bytes, `ret 4`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0xac = CTCSinglePersonOccupiable, TC 0x33 = CTCOwnedEntity),
//   CTCSinglePersonOccupiable::IsOfType(EOccupyType) @ 0x0087C420 (engine_api.tsv), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50.
// Layout: CThingFilter_IsMySleepPoint (Ego_r.pdb, 4 bytes): Sleeper +0; retail CThing interface bitset +0x20 (TC 0x33 =
//   word +0x24 bit 19, TC 0xac = word +0x34 bit 12), interface map +0x44; CTCOwnedEntity POwner +0xd (odd offset).
// Shape: nested ifs with one `return false` at the end (the exits laid out last), the unowned case `return true`, and
//   the owner (a named local) compared and returned directly (`xor edx,edx ; sete dl ; mov al,dl`).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

class CThing;
enum EOccupyType { OCCUPY_TYPE_SLEEP_2 = 2 };
class CTCSinglePersonOccupiable { public:
    bool IsOfType(EOccupyType type);            // 0x0087C420
};
class CBaseObject;
struct CBaseIntelligentPointer {
    CBaseObject* GetPItem() const;              // 0x00A01B50
    CThing* GetThing() const { return (CThing*)GetPItem(); }
};
class CTCOwnedEntity { public:                         // TC 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer POwner;             // +0x0d
};

class CThing {
public:
    unsigned char _pad00[0x20];
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

class CThingFilter_IsMySleepPoint { public:
    CThing* Sleeper;                            // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsMySleepPoint::operator()(CThing* thing) const
{
    if (thing->HasTC(0xac)) {
        if (thing->GetTC<CTCSinglePersonOccupiable>(0xac)->IsOfType(OCCUPY_TYPE_SLEEP_2)) {
            if (thing->HasTC(0x33)) {
                CThing* owner = thing->GetTC<CTCOwnedEntity>(0x33)->POwner.GetThing();
                return owner == Sleeper;
            }
            return true;
        }
    }
    return false;
}