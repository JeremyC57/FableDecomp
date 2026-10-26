// CGameScriptInterface::GetAllCreaturesExcludingHero(std::vector<CScriptThing>& things) const -- script: append the script thing of every AI creature (the hero is not one); returns the output vector's size
// Leaf: GetAllCreaturesExcludingHero
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
class CBaseClass { public:                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CCPPointerInfo { public:
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
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
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
struct CThingVector {                                   // STLport vector<CThingAICreature*> (malloc allocator)
    CThing_Retail** _M_start;
    CThing_Retail** _M_finish;
    CThing_Retail** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CThing_Retail* operator[](unsigned long n) const { return _M_start[n]; }
};
class CThingFilter_None { public:
    CThingFilter_None() {}
};
struct CThing_Retail {
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
CThing_Retail** FindThings_AICreatures(CThing_Retail** capacity);
class CThingSearchTools { public:
    void GetAllThings(const CThingFilter_None& filter, CThingVector& out);   // 0x0052B270
};
// Same-TU stand-in (see the siblings): only writes the output vector's words and never takes the filter's address, so
// the empty filter temporary is homed in the dead `things` argument slot as in retail.
#pragma auto_inline(off)
void CThingSearchTools::GetAllThings(const CThingFilter_None& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_AICreatures(out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
class CGameScriptInterface { public:
    virtual long GetAllCreaturesExcludingHero(CScriptThingVector& things) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

long CGameScriptInterface::GetAllCreaturesExcludingHero(CScriptThingVector& things) const
{
    CThingVector found;
    World->ThingSearchTools->GetAllThings(CThingFilter_None(), found);
    const unsigned long count = found.size();
    things.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        things.push_back(found[i]->GetScriptThing());
    return things.size();
}
