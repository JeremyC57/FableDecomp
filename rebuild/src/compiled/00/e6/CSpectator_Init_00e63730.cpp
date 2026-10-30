// CSpectator::Init() -- the chicken-kicking spectator script picks its number from whichever "Spectator1"/"Spectator2" marker it stands on (else 3)
// Leaf: Init
// Retail 0x00E63730, 352 bytes. Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, the script
// interface's GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer), IsDistanceBetweenThingsUnder
// 0x00CBE2FF (__fastcall, distance 1.0f pushed before the nested call), the CScriptThing temporary's inline destructor
// (vtable 0x01238C8C, counted pimpl release + operator delete 0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0), IsDone 0x00F35B30.
// Layout: Ego_r.pdb CSpectator (the CV_ChickenKickingScript one, 0x20 bytes): CEntityScriptBase {vptr, Game +4,
// CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18, SpectatorNumber +0x1C. Model as the landed CKG_Sheriff::Init
// 0x00D1DFF0. Strings: "Spectator1" 0x012E7450, "Spectator2" 0x012E7418.
// Shape: each marker test is a named bool filled inside its own block (name + temporary destroyed before the test);
// every arm is `if (IsActiveThreadTerminating()) return; SpectatorNumber = n;` -- VC hoists the second pair's shared IsDone call above
// the branch on the second result.
namespace NScript { class CPredicateBase; }
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
template <class T> class CCountedPointer { public:                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern const char g_spectator1[];                  // 0x012E7450 "Spectator1"
extern const char g_spectator2[];                  // 0x012E7418 "Spectator2"
struct CV_ChickenKickingScript;
struct CQ_SunnyvaleMasterData;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

struct CSpectator : public CEntityScriptBase {
        CV_ChickenKickingScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;          // +0x18
    long                     SpectatorNumber;      // +0x1c
private:
    virtual void Init();   // PDB ?Init@<entity>@..@@EAEXXZ: every entity override is private (slot 2)
public:
};

void CSpectator::Init()
{
    bool nearFirst;
    {
        CCharString name(g_spectator1, -1);
        nearFirst = IsDistanceBetweenThingsUnder(Me, Game->GetThingWithScriptName(name), 1.0f);
    }
    if (nearFirst) {
        if (IsActiveThreadTerminating())
            return;
        SpectatorNumber = 1;
        return;
    }
    bool nearSecond;
    {
        CCharString name(g_spectator2, -1);
        nearSecond = IsDistanceBetweenThingsUnder(Me, Game->GetThingWithScriptName(name), 1.0f);
    }
    if (nearSecond) {
        if (IsActiveThreadTerminating())
            return;
        SpectatorNumber = 2;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    SpectatorNumber = 3;
}