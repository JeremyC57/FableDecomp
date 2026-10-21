// CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& scriptName, std::vector<CScriptThing>& things) const -- collect every thing with the script name and append their script things; returns the output vector's size (0 without touching it when none match)
// Leaf: ?GetAllThingsWithScriptName@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
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
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CThingFilter_HasScriptName {
    CCharString m_name;
    CThingFilter_HasScriptName(const CCharString& name) : m_name(name) {}
};
struct CThing_Retail {
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
struct CThingSearchTools {
    void GetAllThingsWithScriptName(const CThingFilter_HasScriptName& filter, CThingVector& out);   // 0x005ED860
};
CThing_Retail** FindThings_GetAllThingsWithScriptName(CThing_Retail** capacity);   // stand-in callee of the stand-in body below
// Same-TU stand-in (the template instance is emitted in this TU too; the linker kept 0x005ED860): it only writes the
// output vector's words, so VC7.1 knows the vector does not escape and keeps its start pointer in ebp across the loop.
#pragma auto_inline(off)
void CThingSearchTools::GetAllThingsWithScriptName(const CThingFilter_HasScriptName& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_GetAllThingsWithScriptName(out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p;
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CGameScriptInterface {
    virtual long GetAllThingsWithScriptName(const CCharString& scriptName, CScriptThingVector& things) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

long CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& scriptName, CScriptThingVector& things) const
{
    CThingVector found;
    World->ThingSearchTools->GetAllThingsWithScriptName(CThingFilter_HasScriptName(scriptName), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    for (CThing_Retail** it = found._M_start; it != found._M_finish; ++it) {
        CThing_Retail* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}
