// Leaf: ?MakeRandomComment@CLostTraderTrader@
// Grade: RELOCATION_MATCH 528/528 on the corrected span 0x00EA4DE0+528 (code + embedded jump table; bounds audit) -- catalog row 704 B ran into 0x00EA4FF0
// CLostTraderTrader::MakeRandomComment() (0x00EA4DE0) -- the lost trader's comment to the hero: the first six calls step
//   through the comments in order, later ones pick one at random (rand() % 5); each starts a conversation between Me and the
//   hero and adds the chosen line
// Retail: Fable.exe @ 0x00EA4DE0, 507 bytes incl. the 5-entry jump table at 0x00EA4FDC, plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30, rand 0x00BFEB16, CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 70
//   GetHero, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation. Strings
//   "TEXT_QST_042_LOSTTRADER_QUICK" 0x012EDDC4, "_FOLLOW" 0x012EE014, "_SCARY" 0x012EDFF4, "_STUPID" 0x012EDE94,
//   "_BROTHER" 0x012EDE74, "_BANTER" 0x012EDFD4.
// Layout: entity-script base {vptr, Game +4, Me +8}; the comment counter at +0x28 (descriptive).
// Shape: switch jump table (5 cases + default).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (the row's local CBase)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CLostTraderTrader : CEntityScriptBase {
    unsigned char _pad14[0x14];
    unsigned long CommentCount;                         // +0x28
    void MakeRandomComment();
};

void CLostTraderTrader::MakeRandomComment()
{
    unsigned long index = CommentCount++;
    if (index > 5) {
        if (IsActiveThreadTerminating())
            return;
        index = rand() % 5;
        CommentCount = 6;
    }
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    switch (index) {
    case 0:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_QUICK", false, Me, Game->GetHero());
        break;
    case 1:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_FOLLOW", false, Me, Game->GetHero());
        break;
    case 2:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_SCARY", false, Me, Game->GetHero());
        break;
    case 3:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_STUPID", false, Me, Game->GetHero());
        break;
    case 4:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_BROTHER", false, Me, Game->GetHero());
        break;
    default:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_BANTER", false, Me, Game->GetHero());
        break;
    }
}
