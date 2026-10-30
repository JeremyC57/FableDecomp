// CV_BookCollectingScript::CV_BookCollectingScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012E5EFC, empty BookReactions vector, the Boy/Girl entity handles and three empty book vectors
// Leaf: ??0CV_BookCollectingScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E54310, length 147 bytes (padded to 160), `ret 8`.
// Catalog label "CV_BanditCampPathScript" is wrong: the factory 0x00E57A50 (`push 0B8h`, this ctor) is registered
// at 0x00CD849C under "S_VBC" / "V_BookCollecting", and the PDB CV_BookCollectingScript layout fits the stores.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x2).
// Layout (retail; PDB -0x14 up to the first vector, retail STLport vectors are 12 bytes): BookReactions +0x4c,
// seh_Boy +0x58, seh_Girl +0x68 (CScriptGameResourceObjectScriptedThingBase, vtable 0x0127094C), bools/longs
// +0x78..+0x93 (not initialised), BookDef +0x94, BookDonated +0xa0, BookOwned +0xac (retail size 0xB8).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
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
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};
class CConversation;
class CCharString;
struct CBitWord;
namespace NScript {
class CV_BookCollectingScript : public CQuestScriptObject {
public:   // vtable 0x012E5EFC
    bool ReadingBook;                              // +0x48
    CRetailVector<CConversation> BookReactions;    // +0x4c
    CScriptGameResourceObjectScriptedThingBase seh_Boy;    // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Girl;   // +0x68
    bool DoneIntro;                                // +0x78
    bool HatRewarded;                              // +0x79
    bool KeyRewarded;                              // +0x7a
    long GossipState;                              // +0x7c
    bool HasInformation;                           // +0x80
    long BooksDonated;                             // +0x84
    long GoodBooksDonated;                         // +0x88
    long LastBookRequested;                        // +0x8c
    long BooksInGame;                              // +0x90
    CRetailVector<CCharString> BookDef;            // +0x94
    CRetailVector<CBitWord> BookDonated;           // +0xa0 (vector<bool>, three words in retail)
    CRetailVector<CBitWord> BookOwned;             // +0xac
    virtual void v00();
    CV_BookCollectingScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_BookCollectingScript;
NScript::CV_BookCollectingScript::CV_BookCollectingScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}