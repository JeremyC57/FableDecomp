// COpinionReactionManager::GetThingOpinionOfPlayer(const CThing& thing, EOpinion opinion) const -- one opinion value of `thing` towards the hero: gather the opinion set from the thing's opinion-of-hero and the hero's opinion source, index it
// Leaf: ?GetThingOpinionOfPlayer@COpinionReactionManager@@
// Grade: DIFFER(229v227) -- the five zero stores: retail `xor edx,edx` and stores straight through eax; ours copies the fill cursor (`mov edx,eax`) and zeroes ecx (2 bytes). Everything else (store order end/finish/start, the source register) matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x00701750, 227 bytes, `ret 8`.
// Label correction: catalog "COpinionReactionManager_GetOpinionValue" -> COpinionReactionManager::GetThingOpinionOfPlayer
//   (Ego_r.pdb name set; returns one opinion of a thing towards the hero).
// Callees: CVectorMap LowerBound (const) 0x004365B0, malloc 0x00BFEA0E, CWorld::GetCurrentPlayer 0x0049D9D0,
//   CPlayer::GetControlledCreature (const) 0x00487DD0, GetOpinions 0x00700AE0, free 0x00BFEA14.
// Tried (4): STLport vector(n) over a zero-initialising _Vector_base (254: the member zeros survive), the allocation in
//   the init list (230: start stored first, finish via `add eax,14h`), the landed alloc-proxy CArrayBase + FillN
//   (254), this local-pointer form (fill first, then end/finish/start) with the source pointer zeroed after the hero
//   fetch (229).
// Next: FillN without the cursor copy (STLport's fill_n advances `__first` itself) -- likely the last 2 bytes.
extern "C" void* __cdecl malloc(unsigned int size);      // MSVCR71 import thunk 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // MSVCR71 import thunk 0x00BFEA14

template<class T> __forceinline T* FillN(T* first, unsigned int count, const T& value)
{
    T* cur = first;
    for (; count > 0; --count, ++cur)
        *cur = value;
    return cur;
}

template<class T> class CArray {                         // retail CArray<T>: the three STLport vector pointers (12 bytes)
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    explicit CArray(unsigned int count)
    {
        T* data = (T*)malloc(count * sizeof(T));
        T* last = FillN(data, count, T());
        _M_end_of_storage = data + count;
        _M_finish = last;
        _M_start = data;
    }
    ~CArray()
    {
        if (_M_start)
            free(_M_start);
    }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};

enum EOpinion { OPINION_COUNT = 5 };
enum ETCInterfaceType { TC_CREATURE_OPINION_OF_HERO = 0x4f, TC_OPINION_SOURCE = 0x50 };

class COpinionSet {                                       // Ego_r.pdb: CArray<float> OpinionValues (retail 12 bytes)
public:
    CArray<float> OpinionValues;                          // +0x00
    COpinionSet() : OpinionValues(OPINION_COUNT) {}
    float operator[](long index) const { return OpinionValues[index]; }
};

class CTCBase;
class CTCCreatureOpinionOfHero;
class CTCOpinionSource;

class CInterfaceMap {                                     // CVectorMap<ETCInterfaceType, CTCBase*>: sorted {type, component} vector
public:
    struct SEntry {
        long type;
        CTCBase* component;
    };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const long& type) const;     // 0x004365B0 (const)
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentInterfaces[9];                   // +0x20 bitset of interface types
    CInterfaceMap Interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(long type, const T** out) const
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &Interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};

class CPlayer {
public:
    const CThing* GetControlledCreature() const;          // 0x00487DD0
};

class CWorld {
public:
    CPlayer* GetCurrentPlayer() const;                    // 0x0049D9D0
};

class COpinionReactionManagerDef;
template<class T> class CDefPointer {
public:
    T* Object;
};

class COpinionReactionManager {                           // Ego_r.pdb (0x738 bytes)
public:
    CDefPointer<const COpinionReactionManagerDef> PDef;   // +0x00
    CWorld* World;                                        // +0x04
    float GetThingOpinionOfPlayer(const CThing& thing, EOpinion opinion) const;
    void GetOpinions(const CTCCreatureOpinionOfHero* opinionOfHero, COpinionSet& opinions, const CTCOpinionSource* source) const;   // 0x00700AE0
};

float COpinionReactionManager::GetThingOpinionOfPlayer(const CThing& thing, EOpinion opinion) const
{
    const CTCCreatureOpinionOfHero* opinionOfHero = 0;
    thing.GetInterface(TC_CREATURE_OPINION_OF_HERO, &opinionOfHero);
    COpinionSet opinions;
    const CThing* hero = World->GetCurrentPlayer()->GetControlledCreature();
    const CTCOpinionSource* source = 0;
    if (hero)
        hero->GetInterface(TC_OPINION_SOURCE, &source);
    GetOpinions(opinionOfHero, opinions, source);
    return opinions[opinion];
}
