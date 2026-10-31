// CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& scriptName, std::vector<CScriptThing>& things) const -- collect every thing with the script name and append their script things; returns the output vector's size (0 without touching it when none match)
// Leaf: ?GetAllThingsWithScriptName@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Respell (chunk 638 follow-up, 2026-10-31): PDB spellings shared with the landed 0x008A8890 -- std::vector<CScriptThing> /
//   std::vector<T*> STLport specialisations (reserve 0x008ACD30, protected _M_insert_overflow 0x008AD5F0), the const
//   CThingSearchTools template members, CThing::GetScriptThing() const 0x004C7E50, engine/CCountedPointer.h, engine/CWorld.h,
//   engine/CGameScriptInterface.h (the slot declaration now carries the leaf's mangled name).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A8570, length 323 bytes (`ret 8`).
// Callees: CCharString copy ctor/dtor @ 0x0099EC30 / 0x0099EAE0 (the filter's name), CThingSearchTools::GetAllThings
//   <CThing, vector<CThing*>, CThingFilter_HasScriptName> @ 0x005ED860 on World->ThingSearchTools (+0x20; the manifest
//   template label is propagated), vector<CScriptThing>::reserve @ 0x008ACD30 and _M_insert_overflow @ 0x008AD5F0,
//   CThing::GetScriptThing @ 0x004C7E50, ~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC, free @ 0x00BFEA14.
// Layout: CScriptThing = CBaseClass vptr + counted PImp {Data, Info} (the landed CreateObject 0x00894820 model); the
//   thing vector is an STLport vector<CThing*> over the malloc allocator.
// Shape: the filter (a CCharString copy) is built in the dead scriptName slot; push_back inlined (null-tested placement
//   copy, finish += 12, else the overflow with the __false_type tag in the dead output slot); the temporary script thing
//   is destroyed inline each pass.
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
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CThingFilter_HasScriptName { public:
    CCharString Name;                                   // +0x00 (Ego_r.pdb)
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) {}
};
class CThing { public:
    CScriptThing GetScriptThing() const;                // 0x004C7E50
};
class CThingSearchTools { public:
    template <class T, class TVector, class TFilter>
    void GetAllThings(const TFilter& filter, TVector& out) const;   // 0x005ED860 for <CThing, vector<CThing*>, CThingFilter_HasScriptName>
};
CThing** FindThings_GetAllThingsWithScriptName(CThing** capacity);   // stand-in callee of the stand-in body below
// Same-TU stand-in (the template instance is emitted in this TU too; the linker kept 0x005ED860): it only writes the
// output vector's words, so VC7.1 knows the vector does not escape and keeps its start pointer in ebp across the loop.
#pragma auto_inline(off)
template <>
void CThingSearchTools::GetAllThings<CThing, std::vector<CThing*, std::allocator<CThing*> >, CThingFilter_HasScriptName>(
    const CThingFilter_HasScriptName& filter, std::vector<CThing*, std::allocator<CThing*> >& out) const
{
    CThing** p = FindThings_GetAllThingsWithScriptName(out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
#include "engine/CWorld.h"                             // PThingSearchTools_ptr (scoped_ptr<CThingSearchTools>) +0x20
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#include "engine/CGameScriptInterface.h"               // FABLE_RETAIL_NSCRIPT: World +0x04 and the GetAll* slots

long CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& scriptName, std::vector<CScriptThing, std::allocator<CScriptThing> >& things) const
{
    std::vector<CThing*, std::allocator<CThing*> > found;
    World->PThingSearchTools_ptr->GetAllThings<CThing>(CThingFilter_HasScriptName(scriptName), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    for (CThing** it = found._M_start; it != found._M_finish; ++it) {
        CThing* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}