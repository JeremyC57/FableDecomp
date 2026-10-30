// GiveGameInfo(const std::vector<CCharString>& info) -- show each game-info text page as a hero yes/no question with a "TEXT_GUI_SKIP" answer inside a movie sequence with every non-scripted entity paused, yielding script frames until the page is answered or the script thread is stopping
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?GiveGameInfo@@YIXABV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBF6DC, 290 bytes, `__fastcall` global (the vector in ecx), plain `ret`; frame-pointer
//   size TU like its script-library neighbours (0x00CBF679 MoveAllThingsInVectorToPosition, 0x00CBEB7E IsCutsceneSkipped).
//   The next function starts at once (no padding). No callers in the retail image (like 0x00CBF679).
// Identity (proven, two sources): FableWin.pdb ?GiveGameInfo@@YIXABV?$vector@VCCharString@@...@@@Z is the name set's only
//   global taking a const vector<CCharString>&, and the body walks a 4-byte-element vector (`sar 2`, `lea edx,[edx+ebx*4]`)
//   whose elements are passed as GiveHeroYesNoQuestion's first CCharString; the "TEXT_GUI_SKIP" literal 0x012C18E8 is the
//   skip answer. The chunk label "UI_PlayMovieWithSkipOption" is a descriptive guess.
// Callees: CScriptGameResourceObjectMovieBase ctor 0x006E7B60 / dtor 0x006E7B80 (the out-of-line COMDAT copies), CCharString
//   ctor 0x0099EBF0 / dtor 0x0099EAE0, CWideScreenMagicPauseEntities ctor 0x00CBE09A (out of line; its destructor, slot 379
//   with false, is inline here); CGameScriptInterface (g_pGameScriptInterface 0x0143E8F8) slots 7 NewScriptFrame, 39
//   MsgIsQuestionAnsweredYesOrNo, 114 GiveHeroYesNoQuestion, 370 StartMovieSequence; the running process 0x013D2838
//   (StopRunning +5). Literals "" 0x0122D70E (kept in esi), "TEXT_GUI_SKIP" 0x012C18E8.
// Shape: the wait loop is `while (Msg...() < 0) { if (runner && runner->StopRunning) break; NewScriptFrame(); }` (its test
//   sits at the bottom); each CCharString argument is a named local in its own block (GOTCHAS: a bare temporary
//   passes the ctor's returned `this`), declared 4th, 3rd, 2nd argument as retail builds them.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline ~CCountedPointer()
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
#include "engine/CScriptGameResourceObjectMovieBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

#include "engine/CSuspendableProcessMicrothread.h"     // retail CASuspendableProcess: Running +4, StopRunning +5
extern CSuspendableProcessMicrothread* g_pScriptRunner; // 0x013D2838

class CWideScreenMagicPauseEntities {
public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase* game);              // 0x00CBE09A
    ~CWideScreenMagicPauseEntities() { Game->PauseAllNonScriptedEntities(false); }   // 0x00CBE0B3 (inline here)
    CGameScriptInterfaceBase* Game;         // +0x00
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef unsigned int size_type;
    typedef const _Tp& const_reference;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    const_reference operator[](size_type __n) const { return *(this->_M_start + __n); }
};
}

void __fastcall GiveGameInfo(const std::vector<CCharString>& info)
{
    CScriptGameResourceObjectMovieBase movie;
    {
        CCharString name("");
        g_pGameScriptInterface->StartMovieSequence(name, movie);
    }
    CWideScreenMagicPauseEntities pause(g_pGameScriptInterface);
    for (unsigned int i = 0; i < info.size(); ++i) {
        g_pGameScriptInterface->NewScriptFrame();
        {
            CCharString answer3("");
            CCharString answer2("");
            CCharString answer1("TEXT_GUI_SKIP");
            g_pGameScriptInterface->GiveHeroYesNoQuestion(info[i], answer1, answer2, answer3, true);
        }
        while (g_pGameScriptInterface->MsgIsQuestionAnsweredYesOrNo() < 0) {
            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                break;
            g_pGameScriptInterface->NewScriptFrame();
        }
    }
}