// Grade: DIFFER(317v317) -- scheduling only: retail stores the def index into the dead `defName` slot before `lea ecx,[esp+18h]` (&found) for the GetAllThings call; ours hoists the lea above the store
// CGameScriptInterface::GetAllThingsWithDefName(const CCharString& defName, std::vector<CScriptThing>& things) const -- collect the script things of every thing built from the named def into the vector and return how many it now holds (0 when none was found)
// Leaf: ?GetAllThingsWithDefName@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A8890, length 315 bytes (+ int3 padding), `ret 8`.
// Signature from Ego_r.pdb: ?GetAllThingsWithDefName@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z.
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, the search
//   tools' GetAllThings<CThing, vector<CThing*>, CThingFilter_DefIndex> @ 0x008ADBA0, vector<CScriptThing>::reserve
//   @ 0x008ACD30 and _M_insert_overflow @ 0x008AD5F0, CThing::GetScriptThing @ 0x004C7E50, CBaseClass dtor @ 0x0099A2E0,
//   operator delete @ 0x00BFE9BC, free @ 0x00BFEA14 (the thing vector's storage). CScriptThing vtable 0x01238C8C.
// Layout: CGameScriptInterface World +4; the world's thing search tools +0x20; both vectors are 12-byte STLport vectors.
// Shape: the def-index filter is a 4-byte temporary homed in the dead `defName` slot; the empty result returns 0 through
//   its own copy of the vector destructor; the copy loop walks a pointer with the end re-read each pass, push_back inlined
//   (null-tested placement copy, finish += 12, else the overflow call with the __false_type tag in the dead slot) and the
//   GetScriptThing temporary destroyed inline.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharString {
    void* PStringData;
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;

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
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};

struct CThing {
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
template <class T> struct CThingPtrVector {             // STLport vector<CThing*>
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingPtrVector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct __false_type { __false_type() {} };
struct CScriptThingVector {                             // STLport vector<CScriptThing>
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
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
struct CThingFilter_DefIndex {
    long DefIndex;
    CThingFilter_DefIndex(long index) : DefIndex(index) {}
};
struct CThingListNode { CThingListNode* Next; CThingListNode* Prev; CThing* Thing; long DefIndex; };
struct CThingSearchTools {
    CThingListNode* m_head;
    void GetAllThings(const CThingFilter_DefIndex& filter, CThingPtrVector<CThing*>& things);   // 0x008ADBA0
};
#pragma auto_inline(off)
// Same-TU stand-in for GetAllThings<CThing, vector<CThing*>, CThingFilter_DefIndex> 0x008ADBA0 (retail's LTCG knowledge):
// a simplified body that walks the thing list and fills the vector in place without handing its address to another
// function, so VC7.1 knows the vector's start word survives the later calls (retail keeps it in ebp for the final free).
void CThingSearchTools::GetAllThings(const CThingFilter_DefIndex& filter, CThingPtrVector<CThing*>& things)
{
    for (CThingListNode* node = m_head->Next; node != m_head; node = node->Next)
        if (node->DefIndex == filter.DefIndex && things._M_finish != things._M_end_of_storage)
            *things._M_finish++ = node->Thing;
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

struct CGameScriptInterface {
    virtual long GetAllThingsWithDefName(const CCharString& defName, CScriptThingVector& things) const;
    CWorld* World;                                      // +0x04
};

long CGameScriptInterface::GetAllThingsWithDefName(const CCharString& defName, CScriptThingVector& things) const
{
    CThingPtrVector<CThing*> found;
    CThingFilter_DefIndex filter(CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName));
    World->ThingSearchTools->GetAllThings(filter, found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    for (CThing** it = found.begin(); it != found.end(); ++it) {
        CThing* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}
