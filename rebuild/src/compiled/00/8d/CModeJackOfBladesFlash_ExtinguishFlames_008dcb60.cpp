// CModeJackOfBladesFlash::ExtinguishFlames() -- put out the flames on the perceived target and on every nearby thing
// Grade: see STATUS.tsv
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CTCBase;
class CThing;
struct CInterfaceMap {                             // a thing's sorted {type, component} vector
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);          // 0x0040F020
};
enum ETCInterfaceType { };
class CThing { public:
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
    void RemoveTC(ETCInterfaceType interfaceType);             // 0x004C9840
};
class CThingCreatureBase : public CThing { public: };
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
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CThingSearchTools { public:
    CThing* FirstThing();                              // stand-in callees of the stand-in body below
    CThing* NextThing(CThing* previous);
    template <class F> bool Accepts(const F& filter, CThing* thing);
    template <class F> void GetAllThings(const F& filter, vector<CThing*>& things);   // 0x008E3BB0
};
// Stand-in body (the real one is the 0x008E3BB0 instantiation of this TU): it touches `things` only inline, so VC7.1 sees that
// the vector's address does not escape the call (the landed 0x007FD580 idiom).
#pragma auto_inline(off)
template <class F> void CThingSearchTools::GetAllThings(const F& filter, vector<CThing*>& things)
{
    for (CThing* thing = FirstThing(); thing; thing = NextThing(thing)) {
        if (!Accepts(filter, thing))
            continue;
        if (things._M_finish == things._M_end_of_storage) {
            unsigned long size = things._M_finish - things._M_start;
            unsigned long len = size ? 2 * size : 8;
            CThing** start = (CThing**)malloc(len * sizeof(CThing*));
            for (unsigned long i = 0; i < size; ++i)
                start[i] = things._M_start[i];
            if (things._M_start)
                free(things._M_start);
            things._M_start = start;
            things._M_finish = start + size;
            things._M_end_of_storage = start + len;
        }
        *things._M_finish++ = thing;
    }
}
#pragma auto_inline(on)
class CWorld { public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;           // +0x20
};
struct CModeJackOfBladesFlash {
    unsigned char           _pad00[0x68];
    long                    Timer;                 // +0x68
    long                    BurnCounter;           // +0x6c
    unsigned char           Filter[0xc];           // +0x70 CAnimFilterOfGroup
    CThingCreatureBase*     PCreature;             // +0x7c
    CIntelligentPointer<CThing> PTarget;               // +0x80
    CIntelligentPointer<CThing> PEmitter;              // +0x88
    CIntelligentPointer<CThing> PSpotLight;            // +0x90
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
                PTarget.GetPItem()->RemoveTC((ETCInterfaceType)(0x89));
        }
    }
    {
        CThingFilter_NearCreature filter(PCreature);
        vector<CThing*> things;
        CWorld* world = GetWorld();
        world->ThingSearchTools->GetAllThings(filter, things);
        long count = things.size();
        for (long i = 0; i < count; ++i) {
            CThing* thing = things[i];
            if (thing && !(thing->m_flags91 & 1))
                thing->RemoveTC((ETCInterfaceType)(0x89));
        }
    }
}