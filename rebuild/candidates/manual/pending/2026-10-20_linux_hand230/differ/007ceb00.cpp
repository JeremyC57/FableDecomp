// CTCElectrocutedEffect::CTCElectrocutedEffect(CThing* thing) -- constructor: CTCBase, a null def, zero timers, a null effect pointer and an empty tint list; load SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF, collect the things the owner carries into the tint list, then reset the effect
// Leaf: ??0CTCElectrocutedEffect
// Grade: DIFFER(299v299) -- one frame slot: the discarded __copy result (hidden-pointer return temporary) is homed in the dead index word [esp+10h] in ours, in the dead `thing`/name word [esp+24h] in retail; everything else matches (the 0x007CE1F0 LTCG-style convention reproduces as `__fastcall`; tag temporary needs a user ctor, TintList the alloc-proxy model)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CEB00, 297 bytes (+ int3 padding), `ret 4`; vtable 0x0126F044.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer ctor / SetPItem @ 0x00A01B10 / 0x00A01B90
//   (CIntelligentPointer<CThing> vtable 0x01238C6C); CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0;
//   CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall, twice); CDefinitionManager::GetGlobalIndexFromClassIndex(const
//   CCharString&, long) const @ 0x009AD390; CDefinitionManager::GetDef<CSpecialAbilitiesElectrocutedEffectDef>(long,
//   CDefPointer*) const @ 0x007CEC90; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x46 =
//   CTCCarrying); the carried-things collector (vector<CThing*>&) @ 0x006A2220; vector<CIntelligentPointer<CThing> >::reserve
//   @ 0x0077E580; the STLport random-access __copy into a back_insert_iterator @ 0x007CE1F0 (hidden result in ecx, first in
//   edx, the rest on the stack: modelled `__fastcall`); free @ 0x00BFEA14; CTCElectrocutedEffect::Reset @ 0x007CE7A0.
// Layout = Ego_r.pdb CTCElectrocutedEffect (retail 12-byte vector): PDef +0x0c, CurTime +0x10, FadeInTime +0x14,
//   FadeOutTimeStart +0x18, FadeOutTime +0x1c, PEffect (CIntelligentPointer<CThing>) +0x20, TintList
//   (vector<CIntelligentPointer<CThing> >) +0x28.
// Shape: the landed CTCForcePushPower ctor 0x007A54A0 pattern (init list sharing the ebx zero, the def name a named
//   CCharString homed in the dead `thing` slot, the index a named local); the carried things come in a local
//   vector<CThing*> (zeroed after the lookup, freed by its inline destructor); `copy(begin, end, back_inserter(TintList))`
//   inlines to the __copy call with the iterator-category temporary and a null distance pointer.
class CThing;
class CTCBase;
class CBaseObject;
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
struct CThingPointer : CBaseIntelligentPointer {        // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer(CThing* item = 0) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
template <class P> struct CAllocProxy {                // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    P _M_data;
    CAllocProxy(P v) : _M_data(v) { }
};
struct CThingPointerVector {                            // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CThingPointer* _M_start;
    CThingPointer* _M_finish;
    CAllocProxy<CThingPointer*> _M_end_of_storage;
    CThingPointerVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void reserve(unsigned long n);                      // 0x0077E580
};
struct CThingVector {                                   // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CBackInserter {                                  // back_insert_iterator<vector<CIntelligentPointer<CThing> > >
    CThingPointerVector* container;
    explicit CBackInserter(CThingPointerVector& v) : container(&v) { }
};
CBackInserter __fastcall __copy(CThing** first, CThing** last, CBackInserter result,
                                const random_access_iterator_tag&, int*);   // 0x007CE1F0
inline CBackInserter copy(CThing** first, CThing** last, CBackInserter result)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
struct CDefPointeeBase;
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CSpecialAbilitiesElectrocutedEffectDef;
struct CDefinitionManager {
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesElectrocutedEffectDef>* out) const;   // 0x007CEC90
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCCarrying {                                    // interface 0x46
    void GetCarriedThings(CThingVector& things);        // 0x006A2220
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* thing);                             // 0x00686800
};
class CTCElectrocutedEffect : public CTCBase {          // vtable 0x0126F044
public:
    CDefPointer<const CSpecialAbilitiesElectrocutedEffectDef> PDef;   // +0x0c
    long CurTime;                                       // +0x10
    long FadeInTime;                                    // +0x14
    long FadeOutTimeStart;                              // +0x18
    long FadeOutTime;                                   // +0x1c
    CThingPointer PEffect;                              // +0x20
    CThingPointerVector TintList;                       // +0x28
    virtual void v00();
    void Reset();                                       // 0x007CE7A0
    CTCElectrocutedEffect(CThing* thing);
};

CTCElectrocutedEffect::CTCElectrocutedEffect(CThing* thing)
    : CTCBase(thing), CurTime(0), FadeInTime(0), FadeOutTimeStart(0), FadeOutTime(0)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (ParentThing->HasTC(0x46)) {
        CTCCarrying* carrying = ParentThing->GetTC<CTCCarrying>(0x46);
        CThingVector things;
        carrying->GetCarriedThings(things);
        TintList.reserve(things.size());
        __copy(things._M_start, things._M_finish, CBackInserter(TintList), random_access_iterator_tag(), (int*)0);
    }
    Reset();
}
