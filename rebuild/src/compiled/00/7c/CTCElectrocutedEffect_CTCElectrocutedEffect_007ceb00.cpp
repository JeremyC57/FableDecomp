// CTCElectrocutedEffect::CTCElectrocutedEffect(CThing* thing) -- constructor: CTCBase, a null def, zero timers, a null effect pointer and an empty tint list; load SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF, collect the things the owner carries into the tint list, then reset the effect
// Leaf: ??0CTCElectrocutedEffect
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
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
class CThing;
class CTCBase;
class CBaseObject;
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString { public:
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
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work __copy does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the __copy instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) CBackInserter __fastcall __copy(CThing** first, CThing** last, CBackInserter result, const random_access_iterator_tag& p3, int* p4)
{
    for (long n = long(last - first); n > 0; --n, ++first)
        TmplSink_s7_0(result.container, (long)*first);   // stand-in for *result = *first (push_back of a new intelligent pointer)
    return result;
}
#pragma auto_inline(on)
inline CBackInserter copy(CThing** first, CThing** last, CBackInserter result)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
class CDefPointeeBase;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CSpecialAbilitiesElectrocutedEffectDef;
class CDefinitionManager { public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesElectrocutedEffectDef>* out) const;   // 0x007CEC90
};
void* FindDef_s7r_0();
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_1(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<const CSpecialAbilitiesElectrocutedEffectDef>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (STLport lower_bound over the 8-byte entries)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CTCCarrying { public:                                    // interface 0x46
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
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work Reset does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CTCElectrocutedEffect::Reset @ 0x007CE7A0 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) void CTCElectrocutedEffect::Reset()
{
    NearSink_s7_0(this, 0);
}
#pragma auto_inline(on)

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