// CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom(const CScriptThing& from, const CCharString& defName, std::vector<CScriptThing>& things) const -- script: collect every thing whose definition is defName, sort them nearest-first from `from`'s position and append their script things; returns the output vector's size (0 without touching it when none match)
// Leaf: ?GetAllThingsWithDefNameByDistanceFrom@CGameScriptInterface@@UBEJABVCScriptThing@@ABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x008A9C40, 358 bytes, `ret 0Ch`.
// Body: index = CGameDefinitionManager::Get() (0x0044C6B0)->GetDefGlobalIndexFromName(defName) (0x009AD410);
// World->ThingSearchTools->GetAllThings<CThing, vector<CThing*>, CThingFilter_HasDefGlobalIndex> (0x008ADBA0) into a
// local vector; empty -> 0. things.reserve(n) (0x008ACD30); std::sort(begin, end, CThingCompare_Nearest(from.GetPos()))
// (0x007A4260, __fastcall template, comparator {C3DVector Pos} by value; GetPos is CScriptThing vslot 6, +0x18,
// returning const C3DVector&); push_back(thing->GetScriptThing()) per thing; return things.size().
// Layout: models of the landed sibling GetAllThingsWithScriptName 0x008A8570 (CScriptThing = CBaseClass vptr + counted
// PImp, vector<CThing*> over malloc, World +0x04, ThingSearchTools +0x20). CThingFilter_HasDefGlobalIndex {ulong
// DefGlobalIndex} and CThingCompare_Nearest {C3DVector Pos} from struct_layouts_egor.tsv.
// Shape: the sibling's write-only same-TU stand-in for the search template keeps the found vector unescaped (its start
// and finish live in ebx/ebp and the temporary script thing reuses its stack words); the filter is built in the dead
// defName slot and must escape in the stand-in (retail stores it before computing &found); C3DVector needs a user
// memberwise copy ctor (retail copies X, Y, Z load/store pairwise into the comparator argument). Probes: v/gd/*.
// Opt: /O2 (16-aligned entry).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct CBaseClass {                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CCPPointerInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThing : CBaseClass {                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
};
struct __false_type { __false_type() {} };
struct CScriptThingVector {                             // STLport vector<CScriptThing> (12-byte elements)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    void reserve(unsigned long n);                      // 0x008ACD30
    void _M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x008AD5F0
    void push_back(const CScriptThing& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CScriptThing(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CThing_Retail;
struct CThingVector {                                   // STLport vector<CThing*> (malloc allocator)
    CThing_Retail** _M_start;
    CThing_Retail** _M_finish;
    CThing_Retail** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CCharString {
    void* PStringData;
};
struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CScriptThingVirtuals : CBaseClass {              // CScriptThing's virtual interface (slots 1-6)
    virtual bool IsDead() const;
    virtual const CCharString& GetName() const;
    virtual void _slot3();
    virtual void _slot4();
    virtual void _slot5();
    virtual const C3DVector& GetPos() const;            // +0x18
};
struct CScriptThingFrom : CScriptThingVirtuals {
    CCountedScriptImp PImp;
};
struct CThingFilter_HasDefGlobalIndex {
    unsigned long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(unsigned long index) : DefGlobalIndex(index) {}
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
namespace std {
template <class It, class Cmp> void __fastcall sort(It first, It last, Cmp comp);   // 0x007A4260 for CThing**
}
struct CThing_Retail {
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
CGameDefinitionManager* __fastcall GetGameDefinitionManager();   // 0x0044C6B0
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_HasDefGlobalIndex& filter, CThingVector& out);   // 0x008ADBA0
};
CThing_Retail** FindThings_GetAllThingsWithDefGlobalIndex(const CThingFilter_HasDefGlobalIndex* filter, CThing_Retail** capacity);
// Same-TU stand-in (see the sibling): passes the filter's address on (a by-value read lets VC drop or reschedule the
// filter store into the dead defName slot) and only writes the output vector's words.
#pragma auto_inline(off)
void CThingSearchTools::GetAllThings(const CThingFilter_HasDefGlobalIndex& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_GetAllThingsWithDefGlobalIndex(&filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CGameScriptInterface {
    virtual long GetAllThingsWithDefNameByDistanceFrom(const CScriptThingFrom& from, const CCharString& defName, CScriptThingVector& things) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

long CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom(const CScriptThingFrom& from, const CCharString& defName, CScriptThingVector& things) const
{
    CThingVector found;
    World->ThingSearchTools->GetAllThings(CThingFilter_HasDefGlobalIndex(GetGameDefinitionManager()->GetDefGlobalIndexFromName(defName)), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    std::sort(found._M_start, found._M_finish, CThingCompare_Nearest(from.GetPos()));
    for (CThing_Retail** it = found._M_start; it != found._M_finish; ++it) {
        CThing_Retail* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}