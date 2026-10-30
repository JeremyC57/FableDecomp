// NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine::Init() -- finds the Knothole Glade gate, plays the KNG_HORN criteria sound on it and overrides the music with set 0x17
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E15830, 182 bytes (padded to 192), plain `ret`. Slot 3 of vtable 0x012E0A4C.
//   PDB name ?Init@CWB_WhiteBalverine@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ.
// Callees: through the entity script's interface pointer (+4): CGameScriptInterface slot 72 GetThingWithScriptName
//   (+0x120), slot 689 PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) (+0xac4, 0x008A09B0, result
//   dropped), slot 696 OverrideMusic(EMusicSetType, bool, bool) (+0xae0, 0x00891220); CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0; the CScriptThing local's inline destructor (vtable 0x01238C8C, counted release
//   with operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0). Strings "KG_Gate" 0x012C31A8, "KNG_HORN" 0x012CB198.
// Shape: the gate is a named CScriptThing local built in place from the by-value result and destroyed at the end; both
//   names are implicit conversion temporaries sharing one frame word; one zero register (esi) serves the `false`
//   pushes and the destructor.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                     // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                            // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_WhiteBalverineKnotholeGladeScript { public:
    struct CWB_WhiteBalverine {
        virtual void v00();
        CGameScriptInterface* Game;                // +0x04
        virtual void Init();
    };
};
void CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine::Init()
{
    CScriptThing gate = Game->GetThingWithScriptName("KG_Gate");
    Game->PlayCriteriaSoundOnThing(gate, "KNG_HORN");
    Game->OverrideMusic((EMusicSetType)0x17, false, false);
}
}