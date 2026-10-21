// CGameScriptInterface::GetAllThingsInLevel(const CCharString& levelName, std::vector<CScriptThing>& things) const -- resolve the level's map number, touch the map, collect every thing in that map and append the script things of the live, script-visible ones; returns the output vector's size
// Leaf: ?GetAllThingsInLevel@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Grade: DIFFER(ours v retail) -- scheduling only: the filter's map-number store and the ThingSearchTools load trade places around the found-vector push
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A8AF0, length 364 bytes (`ret 8`).
// Callees: the world's virtual slot 12 (+0x30, its CWorldMap), CWorldMap::GetMapNumberFromScriptName @ 0x004FB880, the
//   map's virtual slot 10 (+0x28) with the map number, CThingSearchTools::GetAllThings<CThing, vector<CThing*>,
//   CThingFilter_InMap> @ 0x008ADC30 on World->ThingSearchTools (+0x20; template label propagated), CThing::GetScriptThing
//   @ 0x004C7E50, the script-thing imp's virtual slot 77 (+0x134, a bool that hides it from scripts),
//   vector<CScriptThing>::_M_insert_overflow @ 0x008AD5F0, ~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC,
//   free @ 0x00BFEA14.
// Layout: CThing dead flags +0x91 bit 0; the in-map filter is {CWorldMap*, map number}; CScriptThing = CBaseClass vptr +
//   counted PImp {Data, Info} (the landed CreateObject 0x00894820 model).
// Shape: a same-TU stand-in for the GetAllThings instance keeps the found vector's start in ebp (the 0x008A8570 lesson);
//   the live test is the inline `m_dead ? false : true` (`not dl ; test dl,1`); the script thing is a named local in
//   the filter's frame words, pushed back by copy.
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
struct CCharString;
struct CScriptThingImp_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual bool IsHiddenFromScripts();                 // slot 77 (+0x134)
};
struct CThing_Retail {
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                           // +0x91 bit 0
    unsigned char m_91_1 : 7;
    bool IsAlive() const { return m_dead ? false : true; }
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
struct CWorldMap_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual void TouchMap(long mapNumber);              // slot 10 (+0x28)
    long GetMapNumberFromScriptName(const CCharString& name);   // 0x004FB880
};
struct CThingFilter_InMap {
    CWorldMap_Retail* m_pMap;
    long              m_mapNumber;
    CThingFilter_InMap(CWorldMap_Retail* map, long mapNumber) : m_pMap(map), m_mapNumber(mapNumber) {}
};
struct CThingSearchTools {
    void GetAllThingsInMap(const CThingFilter_InMap& filter, CThingVector& out);   // 0x008ADC30
};
CThing_Retail** FindThings_GetAllThingsInMap(CWorldMap_Retail* map, long mapNumber, CThing_Retail** capacity);   // stand-in callee of the stand-in body below
// Same-TU stand-in (the template instance is emitted in this TU too; the linker kept 0x008ADC30): it only reads the filter and touches the
// output vector's words, so VC7.1 knows the vector does not escape.
#pragma auto_inline(off)
void CThingSearchTools::GetAllThingsInMap(const CThingFilter_InMap& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_GetAllThingsInMap(filter.m_pMap, filter.m_mapNumber, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
struct CWorld_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap_Retail* GetWorldMap();            // slot 12 (+0x30)
    unsigned char      _pad04[0x1c];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CGameScriptInterface {
    virtual long GetAllThingsInLevel(const CCharString& levelName, CScriptThingVector& things) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

long CGameScriptInterface::GetAllThingsInLevel(const CCharString& levelName, CScriptThingVector& things) const
{
    long mapNumber = World->GetWorldMap()->GetMapNumberFromScriptName(levelName);
    World->GetWorldMap()->TouchMap(mapNumber);
    CThingVector found;
    CThingFilter_InMap filter(World->GetWorldMap(), mapNumber);
    World->ThingSearchTools->GetAllThingsInMap(filter, found);
    for (CThing_Retail** it = found._M_start; it != found._M_finish; ++it) {
        CThing_Retail* thing = *it;
        if (thing->IsAlive()) {
            CScriptThing scriptThing = thing->GetScriptThing();
            CScriptThingImp_Retail* imp = (CScriptThingImp_Retail*)scriptThing.PImp.Data;
            if (imp && !imp->IsHiddenFromScripts())
                things.push_back(scriptThing);
        }
    }
    return things.size();
}
