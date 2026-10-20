// CThingFilter_IsInSameVolumeContextAsThing::operator()(CThing* thing) const -- a thing passes when the override is set, or when it and the filter's reference thing agree on being inside a live volume (the building it is inside, else its owner)
// Leaf: ??RCThingFilter_IsInSameVolumeContextAsThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8C60, length 193 bytes, `ret 4`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x75: the inside-building component, TC 0x33: the owned-entity
//   component), CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: CThingFilter_IsInSameVolumeContextAsThing (Ego_r.pdb, 8 bytes): PThingInside +0, Override +4; retail CThing
//   interface bitset +0x20 (TC 0x33 = word +0x24 bit 19, TC 0x75 = word +0x2c bit 21), interface map +0x44, Dead +0x91
//   bit 0; the TC 0x75 component's building pointer at +0xc (as in the landed EntityGetStandingInsideBuilding 0x0089BC50),
//   the TC 0x33 component's owner pointer at +0xd (CTCOwnedEntity::POwner, an odd-offset intelligent pointer).
// Shape: the volume thing is chosen by HasTC/GetTC on 0x75 then 0x33 (the two GetPItem calls merge into one site), both
//   aliveness tests are the inline IS_THING_ALIVE into bool locals, and the agreement is `(!a && !b) || (a && b)`.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

class CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {
    CBaseObject* GetPItem() const;              // 0x00A01B50
    CThing* GetThing() const { return (CThing*)GetPItem(); }
};
struct CTCInsideBuilding {                      // TC 0x75
    unsigned char _pad00[0x0c];
    CBaseIntelligentPointer PBuilding;          // +0x0c
};
struct CTCOwnedEntity {                         // TC 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer POwner;             // +0x0d
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
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
inline bool IS_THING_ALIVE(const CThing* thing) { return thing && !thing->Dead; }

struct CThingFilter_IsInSameVolumeContextAsThing {
    CThing* PThingInside;                       // +0x00
    bool Override;                              // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsInSameVolumeContextAsThing::operator()(CThing* thing) const
{
    if (Override)
        return true;
    CThing* volume = 0;
    if (thing->HasTC(0x75))
        volume = thing->GetTC<CTCInsideBuilding>(0x75)->PBuilding.GetThing();
    else if (thing->HasTC(0x33))
        volume = thing->GetTC<CTCOwnedEntity>(0x33)->POwner.GetThing();
    bool thingInVolume = IS_THING_ALIVE(volume);
    bool referenceInVolume = IS_THING_ALIVE(PThingInside);
    if ((!thingInVolume && !referenceInVolume) || (thingInVolume && referenceInVolume))
        return true;
    return false;
}
