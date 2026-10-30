// Levers: for(;;) with the terminating return first; in-body `#pragma inline_depth(0)` around the region-hit terminate return gives the out-of-line ~CScriptThing; the CCharString == is the nested CBasicString<char>::operator== (frame result byte) with `strcmp(text, "")`.
// CV_ChapelOfEvilScript::WatchForRegionEntry() -- each time a region loads, if it is DarkwoodChapel teleport the acolytes
// Leaf: ?WatchForRegionEntry@CV_ChapelOfEvilScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6B750, 424 bytes, plain `ret`.
// Label: the manifest's CQ_BountyHuntScript::WatchForEnableFisherCreekHutDoor is propagated and wrong -- the body calls
//   CV_ChapelOfEvilScript::TeleportAcolytes 0x00E6B900 (next function) on "DarkwoodChapel" 0x012DF1D0, a no-argument
//   region watcher: Ego_r.pdb ?WatchForRegionEntry@CV_ChapelOfEvilScript (the only matching void() member).
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CBaseClass() 0x0099A2D0 (the local CScriptThing, vtable
//   0x01238C8C), IsActiveThreadTerminating 0x00CB7940, interface slot 22 MsgOnRegionLoaded(CCharString&) (+0x58) and slot 7
//   NewScriptFrame, Compare 0x00411570 (the inlined CCharString == const char*), ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC, ~CScriptThing 0x004AA840.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
inline bool CScriptThing::MsgExpressionPerformedTo(CCharString& expression) const { if (!PImp.Data) return false; return PImp.Data->MsgExpressionPerformedTo(expression); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_ChapelOfEvilScript : public CQuestScriptBase {
public:
    void TeleportAcolytes(const CScriptThing& thing);       // 0x00E6B900
    void WatchForRegionEntry();
};
} // namespace NScript
using NScript::CV_ChapelOfEvilScript;

void NScript::CV_ChapelOfEvilScript::WatchForRegionEntry()
{
    CCharString region;
    CScriptThing thing;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (region == "DarkwoodChapel") {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            TeleportAcolytes(thing);
        }
        Game->NewScriptFrame();
    }
}