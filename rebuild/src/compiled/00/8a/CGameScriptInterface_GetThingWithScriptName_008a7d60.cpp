// CGameScriptInterface::GetThingWithScriptName(const CCharString& scriptName) const -- the script thing of the first world thing carrying the script name, or the null script thing when none does
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A7D60, length 149 bytes (padded to 160).
// Callees: CCharString copy constructor @ 0x0099EC30 (into the dead name argument slot), the world's (+0x20) script-name
//          search @ 0x005ED860 (manifest "GetAllThings<...>" is propagated), CCharString::~CCharString @ 0x0099EAE0,
//          CBaseClass::CBaseClass @ 0x0099A2D0, CThing::GetScriptThing @ 0x004C7E50, free @ 0x00BFEA14 (the vector storage).
// Vtable: CScriptThing 0x01238C8C.
// Shape: the result vector is a 12-byte frame local zeroed from the shared `xor ebx,ebx`; the name is copied into the
// argument's own slot by a `__forceinline` helper taking the CCharString BY VALUE; the null script thing is built
// inline into the hidden pointer; the vector storage is released by hand-inlined `free`.
// Leaf: GetThingWithScriptName
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C3DVector { public: float x, y, z; };
struct SCountBlock;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
template <class T> class CCountedPointer { public:                          // the counted pointer member: an inline constructor zeroes both words after the vtable store
    CScriptThingImp* Data;                         // +4
    SCountBlock*     Info;                         // +8
    CCountedPointer() : Data(0), Info(0) {}
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
struct CThing_Retail {
    unsigned char  _pad00[0x74];
    CCharString    ScriptName;                     // +0x74
    unsigned char  _pad78[0x91 - 0x78];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    CScriptThing GetScriptThing();                 // 0x004C7E50 (manifest "GFPredicateAnd<...>" is propagated)
};
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager_Retail {
    static CDefinitionManager_Retail& Get();       // 0x0044C6B0
};
class CWorld; class CMainGameComponent; class CDisplayEngine; struct CPlayerManager_Retail; class CThingSearchTools;
extern "C" void free(void* p);                     // 0x00BFEA14
template <class T> struct vector {                 // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    T* begin() { return _M_start; }
};
struct CThingScriptNameSearch_Retail {             // the world's thing collection at +0x20
    void FindThingsWithScriptName(const CCharString& name, vector<CThing_Retail*>& things);   // 0x005ED860
};
extern "C" void* __cdecl malloc(unsigned int size); // 0x00BFEA0E
CThing_Retail* FirstWorldThing();                  // stand-in callees of the stand-in body below
CThing_Retail* NextWorldThing(CThing_Retail* previous);
bool HasScriptName(CThing_Retail* thing, const CCharString& name);
// Stand-in body (the real one is 0x005ED860): it touches `things` only inline, so VC7.1 sees that the vector's address does
// not escape the call (the landed 0x007FD580 idiom).
#pragma auto_inline(off)
__declspec(noinline) void CThingScriptNameSearch_Retail::FindThingsWithScriptName(const CCharString& name, vector<CThing_Retail*>& things)
{
    for (CThing_Retail* thing = FirstWorldThing(); thing; thing = NextWorldThing(thing)) {
        if (!HasScriptName(thing, name))
            continue;
        if (things._M_finish == things._M_end_of_storage) {
            unsigned long size = things._M_finish - things._M_start;
            unsigned long len = size ? 2 * size : 8;
            CThing_Retail** start = (CThing_Retail**)malloc(len * sizeof(CThing_Retail*));
            for (unsigned long i = 0; i < size; ++i)
                start[i] = things._M_start[i];
            if (things._M_start)
                free(things._M_start);
            things._M_start = start;
            things._M_finish = start + size;
            things._M_end_of_storage = start + len;
        }
        *things._M_finish++ = thing;
    }
}
#pragma auto_inline(on)
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingScriptNameSearch_Retail* Things;         // +0x20
};
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual CScriptThing GetThingWithScriptName(const CCharString& scriptName) const;
    __forceinline void FindThingsWithScriptName(CCharString name, vector<CThing_Retail*>& things) const
    {
        World->Things->FindThingsWithScriptName(name, things);
    }
};

CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& scriptName) const
{
    vector<CThing_Retail*> things;
    FindThingsWithScriptName(scriptName, things);
    if (things.empty())
        return CScriptThing();
    return (*things.begin())->GetScriptThing();
}