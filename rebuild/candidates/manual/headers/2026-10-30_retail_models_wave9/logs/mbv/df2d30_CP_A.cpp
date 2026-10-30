// CFireHeartHolder::Main() -- until the fire heart has been taken: each frame, when the hero uses the holder, play the (unnamed) movie sequence, pause the world, mark the heart taken, take the fire heart object from the hero, remove the holder's minimap marker and unpause; then idle until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF2D30, length 349 bytes (`ret`).
// Callees: the entity script's IsDone @ 0x00F35B30, CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CScriptThing slot 20
//   MsgIsUsedBy(const CCharString&) (0x004AAB10), the movie handle's root ctor/dtor @ 0x0099A380 / 0x0099A430 and its PImp
//   counted-pointer dtor @ 0x006E7AB0 (vtables 0x01260EF4 / 0x0126008C); CGameScriptInterface slots 7 NewScriptFrame,
//   125 TakeObjectFromHero, 352 MiniMapRemoveMarker, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities.
// Layout (the CEntityScriptBase prefix of the landed entity scripts): vptr, Game +4, CScriptThing Me +8, the owning quest
//   script +0x14 (fire heart taken flag +0x50).
// Shape: the pause/unpause pair goes through one named copy of Game (edi); the movie handle is a block local whose
//   implicit destructor releases PImp out of line and then runs the inline base destructor (vptr store + root dtor).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#if 1
#define FABLE_ENGINE_CCOUNTEDPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 9): the engine's reference-counted owning pointer and
// its shared count block. Not polymorphic (no vtable, no RTTI); every member is inline in the release build.
//
// CCPPointerInfo (Ego_r.pdb, 12 bytes): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*). DeleteFunc is
//   __fastcall: every retail release loads `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040, 0x008AB2B0)
//   and the deleters it points at are `SIXPAX@Z` (static __fastcall void(void*)) CCountedPointer<T>::DeleteData in
//   FableWin.pdb (343 instantiations); Ego_r.pdb's `_func___cdecl_void_void_ptr` is the PDB's generic spelling.
//   Ctor: FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z = (unsigned long, void (__fastcall*)(void*), void*); retail
//   Reset 0x008AB2B0 expands it after `new(0xc)`: [eax]=1, [eax+4]=deleter, [eax+8]=Data (member order).
//
// CCountedPointer<T> (Ego_r.pdb, 8 bytes for every T): Data +0x0 (T*), Info +0x4 (CCPPointerInfo*). Member set = FableWin.pdb
//   (the debug build emits them; Ego_r keeps out of line only ShareData / ReleaseData / Reset / DeleteData, the rest inline):
//   ??0 (), (T*), (const&), member template ??$?0 (const CCountedPointer<U>&); ??1; ??4 (const&), member template ??$?4;
//   ??C operator-> QBE, ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
//   ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (debug check).
// Bodies, each from the retail COMDATs (identical bytes for every T, folded per T by the linker):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase): `if (Info) { dec [Info]; reload Info;
//     cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }` then Data = 0, Info = 0.
//   ShareData = 0x00CE1040 (CCountedPointer<CScriptThing>, Ego_r.pdb name): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }` (no zeroing between the release and the stores).
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   Reset(T*) = 0x008AB2B0: <release>; Info = 0; Data = data; if (data) Info = new CCPPointerInfo(1, DeleteData, Data)
//     (the block is read back from Data; a failed new leaves Info 0).
//   The (T*) ctor is the same allocation without the release (0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded inline in every CScriptThing copy 0x004ABE90).
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from the true bodies (wave 6/8
//   lessons; wave 9 HEADERS.md lists the landed shapes this header does not reproduce).
// DeleteData is declared only: retail only ever stores its address (the `new CCPPointerInfo` block); each address is the
//   T's own deleter (0x00CDEE00 = DeleteData<CEntityScriptBase>: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]`).
#include "rebuild_abi.h"

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        ReleaseData();
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
    void ReleaseData()
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
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, Data);
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
    static void FABLE_FASTCALL DeleteData(void* data);                    // per-T deleter, address stored in the count block
};

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectMovieBase.h"

#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQuestScript_Retail {
    unsigned char _pad00[0x50];
    bool          FireHeartTaken;                       // +0x50
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CFireHeartHolder : public CEntityScriptBase {
public:
    void*                   __vftable;                  // +0x00
    CGameScriptInterface*   Game;                       // +0x04
    CScriptThing            Me;                         // +0x08
    CQuestScript_Retail*    Quest;                      // +0x14
    void Main();
};

void CFireHeartHolder::Main()
{
    while (!Quest->FireHeartTaken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bool used;
        {
            CCharString hero("SCRIPT_NAME_HERO");
            used = Me.MsgIsUsedBy(hero);
        }
        if (used) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            {
                CCharString name("");
                Game->StartMovieSequence(name, movie);
            }
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Quest->FireHeartTaken = true;
            {
                CCharString fireHeart("OBJECT_LPDD_FIREHEART_01");
                Game->TakeObjectFromHero(fireHeart);
            }
            Game->MiniMapRemoveMarker(Me);
            game->PauseAllNonScriptedEntities(false);
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}