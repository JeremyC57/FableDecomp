// CTCInGameMenu::CloseInGameMenuPC(bool) -- books the paused time, deletes the menu's UI components, forgets the video dialogs and, when asked, plays the menu-close sound
// Leaf: CloseInGameMenuPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047DBB8, 177 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: GFGetTime() @ 0x009E1BC0 (double), the components' virtual deleting destructors (slot 0, `push 1`),
//          std::vector<CComponent*>::erase(first, last) @ 0x00485AA7 (manifest "SelectEntryFromData" is propagated),
//          NUISystem::CManager::GetInstance() @ 0x0041E5F2 (x2), CManager::GetIconsDef() @ 0x0041D8E7 (slot 42,
//          CDefPointer<CUIIconsDef> by hidden pointer), the manager's slot 40 @ 0x0041D0A8 (+0xA0; plays a UI sound,
//          manifest "CASound_PlayMessage"; name unproven).
// Shape: `AccumulativePausedTime += GFGetTime() - CurStartPauseTime`; the delete loop and the inline clear() name the
// member directly (`Components.size()`, `Components[i]`). VC7.1 still forms `lea esi,[edi+58h]` once, but it re-reads
// _M_start for each element as retail does. A pointer or reference local CSEs begin across the loop condition and
// swaps this/vector into esi/edi. One zero register (ebx) for the
// flag test and the stores. Both manager fetches come first (the def owner, then the player); the icons def is a
// temporary homed in the dead flag slot and released through the ref-count base's inline accessors
// (`dec [eax] ; cmp [eax],0`).
// Layout: retail CTCInGameMenu (12-byte containers; engine/CTCInGameMenu.h carries PDB offsets): InGameMenuOpen
// +0x0C, Components +0x58, CurStartPauseTime +0x68, AccumulativePausedTime +0x70, PVideoOptionsNotAcceptedDialog
// +0xB0, PVideoOptionsChangedDialog +0xB4; CUIIconsDef's sound entry at +0x1A8 is not identified.
#pragma optimize("y", off)
#pragma optimize("s", on)

double __fastcall GFGetTime();                       // 0x009E1BC0

struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CUISound;
struct CUIIconsDef : CDefBase {
    unsigned char _pad_0x08[0x1A0];
    CUISound*     CloseMenuSound_1a8;                // +0x1A8 (not identified)
};
struct CIconsDefPointer {                            // CDefPointer<const NUISystem::CUIIconsDef>
    CUIIconsDef* Object;
    CIconsDefPointer() {}
    __forceinline ~CIconsDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CUIIconsDef* operator->() const { return Object; }
};

namespace NUISystem {
class CComponent { public:
    virtual ~CComponent();                           // slot 0 (scalar deleting destructor)
};
class CManager { public:
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28(); virtual void m29();
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33(); virtual void m34();
    virtual void m35(); virtual void m36(); virtual void m37(); virtual void m38(); virtual void m39();
    virtual void PlayUISound_0041d0a8(CUISound* const& sound, bool flag);   // slot 40 (+0xA0)
    virtual void m41();
    virtual CIconsDefPointer GetIconsDef();          // slot 42 (+0xA8)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}

struct CComponentVector {                            // retail 12-byte STLport vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    NUISystem::CComponent** begin() { return _M_start; }
    NUISystem::CComponent** end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return *(begin() + n); }
    NUISystem::CComponent** erase(NUISystem::CComponent** first, NUISystem::CComponent** last);   // 0x00485AA7
    void clear() { erase(begin(), end()); }
};

struct CYesNo;

class CTCInGameMenu { public:
    void*            __vftable;                      // +0x00
    unsigned char    _pad_0x04[0x08];
    bool             InGameMenuOpen;                 // +0x0C
    unsigned char    _pad_0x0d[0x4B];
    CComponentVector Components;                     // +0x58 (PDB +0x68)
    long             LoadGameIndex;                  // +0x64
    double           CurStartPauseTime;              // +0x68
    double           AccumulativePausedTime;         // +0x70
    unsigned char    _pad_0x78[0x38];
    CYesNo*          PVideoOptionsNotAcceptedDialog; // +0xB0
    CYesNo*          PVideoOptionsChangedDialog;     // +0xB4
    void CloseInGameMenuPC(bool playSound);
};

void CTCInGameMenu::CloseInGameMenuPC(bool playSound)
{
    AccumulativePausedTime += GFGetTime() - CurStartPauseTime;
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    PVideoOptionsNotAcceptedDialog = 0;
    PVideoOptionsChangedDialog = 0;
    InGameMenuOpen = false;
    if (playSound) {
        NUISystem::CManager* owner = NUISystem::CManager::GetInstance();
        NUISystem::CManager::GetInstance()->PlayUISound_0041d0a8(owner->GetIconsDef()->CloseMenuSound_1a8, true);
    }
}