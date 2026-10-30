// CV_AmbushScamScript::WatchForExpressions(CScriptThing buddy) -- every script frame until the thread ends: when the hero performs EXPRESSION_FOLLOW at the buddy flag ExpressionDetectedFollow and BuddyHelped, else when he performs EXPRESSION_WAIT flag ExpressionDetectedWait
// Levers: ~CScriptThing __forceinline; in-body `#pragma inline_depth(0)` around both in-loop terminating returns; inline MsgExpressionPerformedTo in `if (!PImp.Data) return false; return PImp.Data->...;` form (the `&&` form tipped VC7.1 into caching constants, mov bl,1)
// Leaf: WatchForExpressions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E32080, 384 bytes, `ret 0Ch` (the 12-byte CScriptThing by value).
// Callees: CCharString() @ 0x0099E4B0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, the inlined
//          CBasicString<char>::operator== calling Compare @ 0x00411570 (`__fastcall`; same-TU stand-in as the landed
//          CV_ChapelOfEvilScript::WatchForExpressions 0x00E6A000), CScriptThing::MsgExpressionPerformedTo (inline; the
//          game-side thing's slot 29, +0x74), ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete
//          @ 0x00BFE9BC, ~CBaseClass @ 0x0099A2E0), CGameScriptInterface slot 7 NewScriptFrame (+0x1c).
// Layout: Ego_r.pdb CV_AmbushScamScript - 0x14: ExpressionDetectedFollow +0x62, ExpressionDetectedWait +0x63,
//          BuddyHelped +0x6a. CScriptThing model as the landed CQ_HangingTreeGoodScript (0x00D74F80).
// Shape: one CCharString local for the loop; `while (!terminating)` with an if / else-if over two `Msg && ==` tests;
//          the terminating returns inside the loop call the out-of-line ~CScriptThing, the loop end expands it.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (same-TU stand-in of the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

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
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
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
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
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
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgExpressionPerformedTo(CCharString& expression) const { if (!PImp.Data) return false; return PImp.Data->MsgExpressionPerformedTo(expression); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

namespace NScript {
class CV_AmbushScamScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x62 - 0x48];                   // +0x48 QuestFinished .. +0x61 AllAmbushersDead
    bool ExpressionDetectedFollow;                       // +0x62
    bool ExpressionDetectedWait;                         // +0x63
    bool HeroAttackOnBuddy;                              // +0x64
    bool HeroAttackOnTricksterBeforeAmbush;              // +0x65
    bool HeroAttackOnTricksterAfterAmbush;               // +0x66
    bool HeroAttackOnTricksterKillingBlow;               // +0x67
    bool HeroTalkToTrickster;                            // +0x68
    bool HeroAttachOnTricksterWarning;                   // +0x69
    bool BuddyHelped;                                    // +0x6a
    void WatchForExpressions(CScriptThing buddy);
};
} // namespace NScript
using NScript::CV_AmbushScamScript;

void NScript::CV_AmbushScamScript::WatchForExpressions(CScriptThing buddy)
{
    CCharString expression;
    while (!IsActiveThreadTerminating()) {
        if (buddy.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_FOLLOW") {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            ExpressionDetectedFollow = true;
            BuddyHelped = true;
        } else if (buddy.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_WAIT") {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            ExpressionDetectedWait = true;
        }
        GSI->NewScriptFrame();
    }
}