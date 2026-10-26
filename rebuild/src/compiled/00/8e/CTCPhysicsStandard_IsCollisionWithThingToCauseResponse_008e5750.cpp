// CTCPhysicsStandard::IsCollisionWithThingToCauseResponse(CTCPhysicsBase* other) -- a thing-thing collision causes a physical response only when this component collides with things (two-way or one-way) and the other does too; a parent with a collision-filter component (interface 0x4D) gets the final say for the other's thing
// Leaf: IsCollisionWithThingToCauseResponse
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E5750, 127 bytes.
// Callees: virtual slots 57 (+0xe4, GetCollidesWithThings: the override 0x00724C90 reads +0x84 bit 7) and 58 (+0xe8,
//   GetCollidesWithThingsOneWay: 0x00724CA0 reads +0x85 bit 0); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0
//   (this TU's copy; inlined GetTC, interface 0x4D = bit 13 of CThing+0x28); the filter component's test @ 0x00770DE0
//   (bool (CThing*), tail jump; catalog "ContainerFindElement" is propagated, named descriptively).
// Layout: CTCBase ParentThing +0x04; CThing component bitset +0x20, interface map +0x44.
// Shape: nested ifs with one shared `xor al,al` exit; `HasTC(k)` as the raw masked word, the filter a named local so the
//   final call tail-jumps. GetTC picks the end/hit entry with ONE ternary, so the component is loaded straight into ecx
//   before the argument (an if-assign pick joins it through eax and moves it to ecx).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CTCCollisionFilter {                     // the interface-0x4D component
    bool ShouldCollideWith(CThing* other);      // 0x00770DE0
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056();
    virtual bool GetCollidesWithThings();   // slot 57 (+0xe4)
    virtual bool GetCollidesWithThingsOneWay();   // slot 58 (+0xe8)
    CThing* ParentThing;                        // +0x04
    CThing* GetParentThing_rb() const { return ParentThing; }
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    bool IsCollisionWithThingToCauseResponse(CTCPhysicsBase* other);
};

bool CTCPhysicsStandard::IsCollisionWithThingToCauseResponse(CTCPhysicsBase* other)
{
    if (GetCollidesWithThings() || GetCollidesWithThingsOneWay()) {
        if (other->GetCollidesWithThings()) {
            if (ParentThing->HasTC(0x4d)) {
                CTCCollisionFilter* filter = (CTCCollisionFilter*)ParentThing->GetTC(0x4d);
                return filter->ShouldCollideWith(other->GetParentThing_rb());
            }
            return true;
        }
    }
    return false;
}