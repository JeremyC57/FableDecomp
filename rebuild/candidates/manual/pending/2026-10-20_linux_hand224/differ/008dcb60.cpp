// CModeJackOfBladesFlash::ExtinguishFlames() -- put out the flames on the perceived target and on every nearby thing
// Grade: DIFFER(ours v retail) -- frame packing: the filter+vector of the nearby-things search get a frame word of their own instead of sharing the GetTC key's word (frame 0x14 v 0x10), so the vector start is reloaded through eax instead of living in ebp
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct CTCBase;
struct CThing;
struct CInterfaceMap {                             // a thing's sorted {type, component} vector
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);          // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(long(interfaceType + 0));
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    void RemoveTC(long interfaceType);             // 0x004C9840
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCPerception {                             // interface 0x34
    CThing* GetPerceivedThing();                   // 0x007446A0
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
struct CThingFilter_NearCreature {                 // the search filter: the creature to search around
    CThingCreatureBase* PCreature;
    CThingFilter_NearCreature(CThingCreatureBase* creature) : PCreature(creature) { }
};
struct CThingSearchTools {
    template <class F> void GetAllThings(const F& filter, vector<CThing*>& things);   // 0x008E3BB0
};
struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;           // +0x20
};
struct CModeJackOfBladesFlash {
    unsigned char           _pad00[0x68];
    long                    Timer;                 // +0x68
    long                    BurnCounter;           // +0x6c
    unsigned char           Filter[0xc];           // +0x70 CAnimFilterOfGroup
    CThingCreatureBase*     PCreature;             // +0x7c
    CBaseIntelligentPointer PTarget;               // +0x80
    CBaseIntelligentPointer PEmitter;              // +0x88
    CBaseIntelligentPointer PSpotLight;            // +0x90
    CWorld* GetWorld();                            // 0x0073A680
    void ExtinguishFlames();
};

void CModeJackOfBladesFlash::ExtinguishFlames()
{
    if (IS_THING_ALIVE(PTarget) && PCreature->HasTC(0x34)) {
        CTCPerception* perception = PCreature->GetTC<CTCPerception>(0x34);
        CThing* perceived = perception->GetPerceivedThing();
        if (perceived && !(perceived->m_flags91 & 1)) {
            CThing* target = perception->GetPerceivedThing();
            if (PTarget.GetPItem() == target)
                PTarget.GetPItem()->RemoveTC(0x89);
        }
    }
    CThingFilter_NearCreature filter(PCreature);
    vector<CThing*> things;
    CWorld* world = GetWorld();
    world->ThingSearchTools->GetAllThings(filter, things);
    long count = things.size();
    for (long i = 0; i < count; ++i) {
        CThing* thing = things[i];
        if (thing && !(thing->m_flags91 & 1))
            thing->RemoveTC(0x89);
    }
}
