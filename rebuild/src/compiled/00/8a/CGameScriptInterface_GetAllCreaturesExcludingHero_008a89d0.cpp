// CGameScriptInterface::GetAllCreaturesExcludingHero(std::vector<CScriptThing>& things) const -- script: append the script thing of every AI creature (the hero is not one); returns the output vector's size
// Leaf: ?GetAllCreaturesExcludingHero@CGameScriptInterface@@UBEJAAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Respell (chunk 638 follow-up, 2026-10-31): PDB spellings shared with the landed 0x008A8890 -- std::vector<CScriptThing> /
//   std::vector<T*> STLport specialisations (reserve 0x008ACD30, protected _M_insert_overflow 0x008AD5F0), the const
//   CThingSearchTools template members, CThing::GetScriptThing() const 0x004C7E50, engine/CCountedPointer.h, engine/CWorld.h,
//   engine/CGameScriptInterface.h (the slot declaration now carries the leaf's mangled name).
// Retail: Fable.exe @ 0x008A89D0, 276 bytes, `ret 4`. Catalog row: only the first 273 bytes (it stops before the final
// `ret 4`), so no compiled body can equal the catalog oracle; this source is a RELOCATION_MATCH against the full 276-byte body.
// Body: CThingSearchTools::GetAllThings<CThingAICreature, vector<CThingAICreature*>, CThingFilter_None> (0x0052B270, the
// landed None-filter instance: no CThingFilter_HasTC::operator() 0x006A80A0 call; the Bind2<None,HasTC> instance is
// 0x0052B300) on World->ThingSearchTools (+0x20) into a local vector (empty filter temporary in the dead `things` slot); things.reserve(count) (0x008ACD30, no early-out); then an index loop pushing back
// creature->GetScriptThing() (0x004C7E50; push_back inlined, overflow 0x008AD5F0; the temporary destroyed inline);
// return things.size(); the found vector is freed (0x00BFEA14).
// Layout / shape: the models of the landed siblings GetAllThingsWithScriptName 0x008A8570 and
// GetAllCreaturesInAreaWithScriptName 0x008A86C0 (write-only same-TU stand-in for the search template so the found
// vector stays unescaped and its start pointer lives in ebp across the loop).
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
class CThingFilter_None { public:
    CThingFilter_None() {}
};
class CThing { public:
    CScriptThing GetScriptThing() const;                // 0x004C7E50
};
class CThingAICreature : public CThing { };             // (CThingAICreature's bases above CThing sit at offset 0)
CThingAICreature** FindThings_AICreatures(CThingAICreature** capacity);
class CThingSearchTools { public:
    template <class T, class TVector, class TFilter>
    void GetAllThings(const TFilter& filter, TVector& out) const;   // 0x0052B270 for <CThingAICreature, vector<CThingAICreature*>, CThingFilter_None>
};
// Same-TU stand-in (see the siblings): only writes the output vector's words and never takes the filter's address, so
// the empty filter temporary is homed in the dead `things` argument slot as in retail.
#pragma auto_inline(off)
template <>
void CThingSearchTools::GetAllThings<CThingAICreature, std::vector<CThingAICreature*, std::allocator<CThingAICreature*> >, CThingFilter_None>(
    const CThingFilter_None& filter, std::vector<CThingAICreature*, std::allocator<CThingAICreature*> >& out) const
{
    CThingAICreature** p = FindThings_AICreatures(out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
#include "engine/CWorld.h"                             // PThingSearchTools_ptr (scoped_ptr<CThingSearchTools>) +0x20
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#include "engine/CGameScriptInterface.h"               // FABLE_RETAIL_NSCRIPT: World +0x04 and the GetAll* slots

long CGameScriptInterface::GetAllCreaturesExcludingHero(std::vector<CScriptThing, std::allocator<CScriptThing> >& things) const
{
    std::vector<CThingAICreature*, std::allocator<CThingAICreature*> > found;
    World->PThingSearchTools_ptr->GetAllThings<CThingAICreature>(CThingFilter_None(), found);
    const unsigned long count = found.size();
    things.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        things.push_back(found[i]->GetScriptThing());
    return things.size();
}
