// CGameScriptInterface::GetAllThingsInLevel(const CCharString& levelName, std::vector<CScriptThing>& things) const -- script: resolve the level's map number, force the map loaded, collect every thing in that map and append the script thing of each living thing whose handle is not null; returns the vector's size
// Leaf: ?GetAllThingsInLevel@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A8AF0, 366 bytes, `ret 8`.
// Name: Ego_r.pdb ?GetAllThingsInLevel@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
//   (the catalog name; sibling of the landed GetAllThings* family 0x008A8570 / 0x008A86C0 / 0x008A8890 / 0x008A89D0 / 0x008A9C40).
// Callees: CWorld slot 12 GetWorldMap (+0x30, Ego_r ?GetWorldMap@CWorld@@UAEAAVCWorldMap@@XZ) three times;
//   CWorldMap::GetMapNumberFromScriptName(const CCharString&) const 0x004FB880; the world map's primary-vtable slot 10 (+0x28)
//   with the map number (named ForceLoad(long) from the Ego_r CWorldMap (long) virtuals -- UNPROVEN slot identity, see NOTES.md);
//   CThingSearchTools::GetAllThings<CThing, vector<CThing*>, CThingFilter_InMap> const 0x008ADC30 (Ego_r.pdb template member);
//   CThing::GetScriptThing() const 0x004C7E50; CScriptThing::IsNull (slot 77, 0x004AB150) expanded on the local and
//   devirtualised onto the imp (`call [edx+134h]`); vector<CScriptThing>::_M_insert_overflow 0x008AD5F0; ~CBaseClass
//   0x0099A2E0, operator delete 0x00BFE9BC, free 0x00BFEA14 (the found vector's storage).
// Layout: CThing Dead +0x91 bit 0 (Ego_r +0xA1); CThingFilter_InMap {WorldMap, MapNo} (Ego_r.pdb, 8 bytes; ctor
//   (const CWorldMap&, long)); CScriptThing from engine/CScriptThing.h; CWorld / CWorldMap are local models because
//   engine/CWorld.h and engine/CWorldMap.h flatten the vptr (header debt).
// Shape: the match lever (agent638's 0x008A8890 lesson): the same-TU stand-in for the search template takes the filter's
//   ADDRESS, and the filter is a temporary in the call, so its words are homed in the slot the script-thing local reuses
//   (frame 0x18) and the map-number store sits after the found-vector push as in retail. The parked source read the filter
//   by value from a named local (the store and the ThingSearchTools load traded places).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"                    // CCountedPointer<T> retail model (PImp of CScriptThing)
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
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
// CScriptThing::IsNull (slot 77, 0x004AB150): retail expands it on the local (devirtualised): null PImp, or the imp's IsNull.
inline bool CScriptThing::IsNull() const { return !PImp.Data || PImp.Data->IsNull(); }
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                             // +0x91 bit 0 (Ego_r +0xA1)
    bool IsAlive() const { return Dead ? false : true; }   // Ego_r.pdb ?IsAlive@CThing@@QBE_NXZ (inline here)
    CScriptThing GetScriptThing() const;                // 0x004C7E50
};
// CWorldMap: local model (engine/CWorldMap.h flattens the vptr): primary-vtable slot 10 (+0x28) takes the map number.
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void ForceLoad(long mapNumber);             // slot 10 (+0x28)
    long GetMapNumberFromScriptName(const CCharString& name) const;   // 0x004FB880
};
class CThingFilter_InMap { public:
    const CWorldMap* WorldMap;                          // +0x00
    long             MapNo;                             // +0x04
    CThingFilter_InMap(const CWorldMap& worldMap, long mapNo) : WorldMap(&worldMap), MapNo(mapNo) {}
};
class CThingSearchTools { public:
    template <class T, class TVector, class TFilter>
    void GetAllThings(const TFilter& filter, TVector& out) const;   // 0x008ADC30 for <CThing, vector<CThing*>, CThingFilter_InMap>
};
CThing** FindThings_GetAllThingsInMap(const CThingFilter_InMap* filter, CThing** capacity);
// Same-TU stand-in for the search template (as the landed siblings): passes the filter's address on and only writes the
// output vector's words.
#pragma auto_inline(off)
template <>
void CThingSearchTools::GetAllThings<CThing, std::vector<CThing*, std::allocator<CThing*> >, CThingFilter_InMap>(
    const CThingFilter_InMap& filter, std::vector<CThing*, std::allocator<CThing*> >& out) const
{
    CThing** p = FindThings_GetAllThingsInMap(&filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
// CWorld: local model (engine/CWorld.h flattens the vptr): slot 12 (+0x30) GetWorldMap, PThingSearchTools +0x20.
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap& GetWorldMap();                   // slot 12 (+0x30)
    unsigned char      _pad04[0x1c];
    CThingSearchTools* PThingSearchTools_ptr;           // +0x20 scoped_ptr<CThingSearchTools>::ptr
};
#include "engine/CGameScriptInterface.h"               // FABLE_RETAIL_NSCRIPT: World +0x04

long CGameScriptInterface::GetAllThingsInLevel(const CCharString& levelName, std::vector<CScriptThing, std::allocator<CScriptThing> >& things) const
{
    long mapNumber = World->GetWorldMap().GetMapNumberFromScriptName(levelName);
    World->GetWorldMap().ForceLoad(mapNumber);
    std::vector<CThing*, std::allocator<CThing*> > found;
    World->PThingSearchTools_ptr->GetAllThings<CThing>(CThingFilter_InMap(World->GetWorldMap(), mapNumber), found);
    for (CThing** it = found._M_start; it != found._M_finish; ++it) {
        CThing* thing = *it;
        if (thing->IsAlive()) {
            CScriptThing scriptThing = thing->GetScriptThing();
            if (!scriptThing.IsNull())
                things.push_back(scriptThing);
        }
    }
    return things.size();
}