// CGameScriptInterface::GetAllThingsWithDefName(const CCharString& defName, std::vector<CScriptThing>& things) const -- script: collect the script thing of every thing built from the named def into the vector; returns the vector's size (0 without touching it when none was found)
// Leaf: ?GetAllThingsWithDefName@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A8890, 317 bytes, `ret 8`.
// Name: Ego_r.pdb ?GetAllThingsWithDefName@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z.
// Body: index = CGameDefinitionManager::Get() (0x0044C6B0, ?Get@CGameDefinitionManager@@SIAAV1@XZ)->GetDefGlobalIndexFromName(defName)
//   (0x009AD410, ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z); World->PThingSearchTools->GetAllThings<CThing,
//   vector<CThing*>, CThingFilter_HasDefGlobalIndex> (0x008ADBA0, Ego_r.pdb template member, const) into a local vector; empty -> 0
//   (the found vector freed, 0x00BFEA14); things.reserve(n) (0x008ACD30); push_back(thing->GetScriptThing()) per thing (0x004C7E50,
//   ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ; overflow 0x008AD5F0; the temporary destroyed inline); return things.size().
// Layout / shape: the model of the landed sibling GetAllThingsWithDefNameByDistanceFrom 0x008A9C40 (the same body without
//   the sort), respelled to the PDB: CThing (not CThing_Retail), STLport vector<CScriptThing> / vector<CThing*> as explicit
//   specialisations of std::vector (real mangled names for reserve / _M_insert_overflow / the template), engine/CWorld.h
//   (PThingSearchTools scoped_ptr word +0x20), engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT, World +0x04, slot 80) and
//   engine/CCountedPointer.h (CScriptThing::PImp); CThingSearchTools / CDefinitionManager headers via the Layout include pattern.
//   What matched: the write-only same-TU stand-in for the search template takes the filter's ADDRESS, so the filter built
//   in the dead defName slot is stored before &found is formed (as retail); the parked stand-in read filter.DefIndex by
//   value and VC7.1 hoisted the `lea ecx,[esp+18h]` above the store.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"                    // CCountedPointer<T> retail model (PImp of CScriptThing)
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
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
// STLport 4.6 vector<CThing*> (malloc-backed storage, freed with free()).
template <> class vector<CThing*, allocator<CThing*> > {
public:
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};
}
class CCharString { public:
    void* PStringData;
};
class CThingFilter_HasDefGlobalIndex { public:
    unsigned long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(unsigned long index) : DefGlobalIndex(index) {}
};
class CThing { public:
    CScriptThing GetScriptThing() const;                // 0x004C7E50
};
class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager { public:

public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CThingSearchTools { public:
    template <class T, class TVector, class TFilter>
    void GetAllThings(const TFilter& filter, TVector& out) const;   // 0x008ADBA0 for <CThing, vector<CThing*>, CThingFilter_HasDefGlobalIndex>
};
CThing** FindThings_GetAllThingsWithDefGlobalIndex(const CThingFilter_HasDefGlobalIndex* filter, CThing** capacity);
// Same-TU stand-in for the search template (see the siblings): passes the filter's address on (a by-value read lets VC
// drop or reschedule the filter store into the dead defName slot) and only writes the output vector's words.
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
#include "engine/CWorld.h"                             // PThingSearchTools (scoped_ptr<CThingSearchTools>) +0x20
#include "engine/CGameScriptInterface.h"               // FABLE_RETAIL_NSCRIPT: World +0x04, slot 80 GetAllThingsWithDefName

long CGameScriptInterface::GetAllThingsWithDefName(const CCharString& defName, std::vector<CScriptThing, std::allocator<CScriptThing> >& things) const
{
    std::vector<CThing*, std::allocator<CThing*> > found;
    World->PThingSearchTools_ptr->GetAllThings<CThing>(CThingFilter_HasDefGlobalIndex(CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName)), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    for (CThing** it = found._M_start; it != found._M_finish; ++it) {
        CThing* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}