// CThingFilter_HasBeenDiscovered::operator()(const CThing& thing) const -- a thing with a discovery component reports its Discovered flag; a thing without one counts as discovered
// Leaf: ??RCThingFilter_HasBeenDiscovered
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A84B0, length 63 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0.
// Identity: Ego_r.pdb `??RCThingFilter_HasBeenDiscovered@@QBE_NABVCThing@@@Z`; the landed caller
//   GetPBestThingInRoughArea<CFilter_Bind4<...,unary_negate<CThingFilter_HasBeenDiscovered>>> 0x00937420 names this
//   address, and 0x00937360 (unary_negate::operator()) negates it. The catalog's "ETCInterfaceType_LookupCapability"
//   is a descriptive auto label. The filter is an empty class (`this` unused).
// Layout: CTCDiscovery (Ego_r.pdb, 24 bytes) bool Discovered at +0x0c; interface type 0x40 = bit 0 of the presence
//   word +0x28; the thing's {kind, component} vector map at +0x44 (local model of the CTCCarrying rows).
// Shape: PeekTC<CTCDiscovery> with the conversion-temporary key homed in the dead thing-argument slot (no frame),
//   the component loaded into eax for the byte read; the absent TC returns `mov al,1`.
class CTCBase;
enum ETCInterfaceType { TC_DISCOVERY = 0x40 };

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
    __forceinline SEntry* Find(long type) const
    {
        SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->type) ? last : it;
    }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap Components;                       // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool PeekTC(const T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (const T*)Components.Find(T::StatGetInterfaceType())->component;
            return true;
        }
        return false;
    }
};

class CTCDiscovery {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_DISCOVERY; }
    unsigned char _pad00[0x0c];                     // +0x00 CTCBase
    bool Discovered;                                // +0x0c
};

class CThingFilter_HasBeenDiscovered {
public:
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasBeenDiscovered::operator()(const CThing& thing) const
{
    const CTCDiscovery* discovery;
    if (thing.PeekTC(&discovery))
        return discovery->Discovered;
    return true;
}