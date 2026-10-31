// CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom(const CScriptThing& from, const CCharString& defName, std::vector<CScriptThing>& things) const -- script: collect every thing whose definition is defName, sort them nearest-first from `from`'s position and append their script things; returns the output vector's size (0 without touching it when none match)
// Leaf: ?GetAllThingsWithDefNameByDistanceFrom@CGameScriptInterface@@UBEJABVCScriptThing@@ABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Respell (chunk 638 follow-up, 2026-10-31): PDB spellings shared with the landed 0x008A8890 -- std::vector<CScriptThing> /
//   std::vector<T*> STLport specialisations (reserve 0x008ACD30, protected _M_insert_overflow 0x008AD5F0), the const
//   CThingSearchTools template members, CThing::GetScriptThing() const 0x004C7E50, engine/CCountedPointer.h, engine/CWorld.h,
//   engine/CGameScriptInterface.h (the slot declaration now carries the leaf's mangled name).
// Retail: Fable.exe @ 0x008A9C40, 358 bytes, `ret 0Ch`.
// Body: index = CGameDefinitionManager::Get() (0x0044C6B0)->GetDefGlobalIndexFromName(defName) (0x009AD410);
// World->PThingSearchTools_ptr->GetAllThings<CThing, vector<CThing*>, CThingFilter_HasDefGlobalIndex> (0x008ADBA0) into a
// local vector; empty -> 0. things.reserve(n) (0x008ACD30); std::sort(begin, end, CThingCompare_Nearest(from.GetPos()))
// (0x007A4260, __fastcall template, comparator {C3DVector Pos} by value; GetPos is CScriptThing vslot 6, +0x18,
// returning const C3DVector&); push_back(thing->GetScriptThing()) per thing; return things.size().
// Layout: models of the landed sibling GetAllThingsWithScriptName 0x008A8570 (CScriptThing = CBaseClass vptr + counted
// PImp, vector<CThing*> over malloc, World +0x04, PThingSearchTools_ptr +0x20). CThingFilter_HasDefGlobalIndex {ulong
// DefGlobalIndex} and CThingCompare_Nearest {C3DVector Pos} from struct_layouts_egor.tsv.
// Shape: the sibling's write-only same-TU stand-in for the search template keeps the found vector unescaped (its start
// and finish live in ebx/ebp and the temporary script thing reuses its stack words); the filter is built in the dead
// defName slot and must escape in the stand-in (retail stores it before computing &found); C3DVector needs a user
// memberwise copy ctor (retail copies X, Y, Z load/store pairwise into the comparator argument). Probes: v/gd/*.
// Opt: /O2 (16-aligned entry).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"                    // CCountedPointer<T> retail model (PImp of CScriptThing)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
namespace std {
struct __false_type { __false_type() {} };
// STLport 4.6 vector<CScriptThing> (12-byte elements): the members and the two out-of-line instantiations this row calls.
template <> class vector<CScriptThing, allocator<CScriptThing> > {
public:
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    void reserve(unsigned int n);                       // 0x008ACD30
    void push_back(const CScriptThing& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CScriptThing(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
protected:
    void _M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&,
                            unsigned int fill_len, bool atend);   // 0x008AD5F0
};
// STLport 4.6 vector<T*> (malloc-backed storage, freed with free()).
template <class T> class vector<T*, allocator<T*> > {
public:
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    T* operator[](unsigned int n) const { return _M_start[n]; }
};
}
class CCharString { public:
    void* PStringData;
};
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
class CThingFilter_HasDefGlobalIndex { public:
    unsigned long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(unsigned long index) : DefGlobalIndex(index) {}
};
class CThingCompare_Nearest { public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
namespace std {
template <class It, class Cmp> void __fastcall sort(It first, It last, Cmp comp);   // 0x007A4260 for CThing** (comparator type unproven)
}
class CThing { public:
    CScriptThing GetScriptThing() const;                // 0x004C7E50
};
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager { public:

public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
   // 0x0044C6B0
class CThingSearchTools { public:
    template <class T, class TVector, class TFilter>
    void GetAllThings(const TFilter& filter, TVector& out) const;   // 0x008ADBA0 for <CThing, vector<CThing*>, CThingFilter_HasDefGlobalIndex>
};
CThing** FindThings_GetAllThingsWithDefGlobalIndex(const CThingFilter_HasDefGlobalIndex* filter, CThing** capacity);
// Same-TU stand-in (see the sibling): passes the filter's address on (a by-value read lets VC drop or reschedule the
// filter store into the dead defName slot) and only writes the output vector's words.
#pragma auto_inline(off)
template <>
void CThingSearchTools::GetAllThings<CThing, std::vector<CThing*, std::allocator<CThing*> >, CThingFilter_HasDefGlobalIndex>(
    const CThingFilter_HasDefGlobalIndex& filter, std::vector<CThing*, std::allocator<CThing*> >& out) const
{
    CThing** p = FindThings_GetAllThingsWithDefGlobalIndex(&filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
#include "engine/CWorld.h"                             // PThingSearchTools_ptr (scoped_ptr<CThingSearchTools>) +0x20
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#include "engine/CGameScriptInterface.h"               // FABLE_RETAIL_NSCRIPT: World +0x04 and the GetAll* slots

long CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom(const CScriptThing& from, const CCharString& defName, std::vector<CScriptThing, std::allocator<CScriptThing> >& things) const
{
    std::vector<CThing*, std::allocator<CThing*> > found;
    World->PThingSearchTools_ptr->GetAllThings<CThing>(CThingFilter_HasDefGlobalIndex(CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName)), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    std::sort(found._M_start, found._M_finish, CThingCompare_Nearest(from.GetPos()));
    for (CThing** it = found._M_start; it != found._M_finish; ++it) {
        CThing* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}