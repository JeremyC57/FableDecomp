#ifndef FABLE_ENGINE_NUISYSTEM_CICONTEXT_H
#define FABLE_ENGINE_NUISYSTEM_CICONTEXT_H
// NUISystem::CIconText (new, chunk 779, row 0x00542A30 Draw): RTTI .?AVCIconText@NUISystem@@, sole direct base
// NUISystem::CChangingStateComponent (the landed dtor 0x005424B0 calls ~CChangingStateComponent 0x0052CCF0, the landed ctor
// 0x00542330 calls CChangingStateComponent(long)). Vtables 0x012470CC (+0, 152 slots), 0x012470A8 (+4 CObserver), 0x012470A0
// (+0x18 CActionParam). Overrides read from the primary vtable: 0 ??_G 0x00543C70, 1 Update 0x005419D0, 2 Draw 0x00542A30,
// 43 Initialise 0x005419C0, 48 ChangeState(unsigned long) 0x005419E0, 111 GetDynamicSize 0x00543A60, 142 GetValue 0x00543AC0,
// 143 SetValue(unsigned long) 0x00542140, 144 SetValue(const CWideString&) 0x00541F20, 145 SetValue(const CCharString&) 0x00541E60,
// 146 GetFont 0x00543AD0, 147 SetTextWindow 0x00543AE0, 148 ClearIcons 0x00542730, 149 AddIcon 0x00542780, 150 InitIcons
// 0x005430F0, 151 GetAlignment 0x00543B10 (landed rows and catalog; Ego_r.pdb / FableWin.pdb ?..@CIconText@NUISystem@@ name set).
// The SetValue overloads are declared in reverse slot order: VC7.1 lays an overload group out in reverse declaration order
// (engine/CText.h, engine/CIEngine.h).
// Layout: Ego_r.pdb CIconText (sizeof 0x1EC) minus 0x30 (retail CComponent 0x134 and 12-byte STLport vectors): Text +0x15c,
// DisplayText +0x160, PFont +0x164, Unavailable +0x16c, TextHandle +0x170, DropShadowHandle +0x178, IconHandles +0x180, Icons
// +0x18c, Window +0x198, PositionOffset +0x1a8, NoRenderNextFrame +0x1b0; sizeof 0x1b4. Retail bytes: the landed ctor 0x00542330
// and dtor 0x005424B0, Draw 0x00542A30 (`mov byte ptr [esi+1B0h],0`, `lea edi,[esi+170h]`, `[esi+18Ch]` / `[esi+190h]` Icons
// begin / end with the 28-byte stride `imul 92492493h`).
// CEmbeddedIcon (Ego_r.pdb, sizeof 0x1c): Graphic +0, CharIndex +4, Flags +8, Size +0xc, Offset +0x14.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/C2DBoxF.h"
class CFontBank;
namespace NUISystem {
enum ETextAlignement;                                                // MS forward enum (engine/NUISystem_CUIDef.h retail branch)
enum EIconFlags;                                                     // MS forward enum (Ego_r.pdb NUISystem::EIconFlags)
class CIconText : public CChangingStateComponent {
public:
    struct CEmbeddedIcon {
        long Graphic;                                                // +0x00
        long CharIndex;                                              // +0x04
        EIconFlags Flags;                                            // +0x08
        C2DVector Size;                                              // +0x0c
        C2DVector Offset;                                            // +0x14
    };
    CIconText(long);                                                 // 0x00542330
    CIconText(const CIconText&);
    virtual ~CIconText();                                            // 0x005424B0
    virtual void Update(float);                                      // slot 1, 0x005419D0
    virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*);   // slot 2, 0x00542A30
    virtual void Initialise();                                       // slot 43, 0x005419C0
    virtual void ChangeState(unsigned long);                         // slot 48, 0x005419E0
    virtual float CanScroll(bool, C2DBoxF&);                         // slot 88 (Ego_r ?CanScroll@CIconText@NUISystem@@UAEM_NAAVC2DBoxF@@@Z)
    virtual void MakeUnavailable();                                  // slot 92, 0x00543A80
    virtual void MakeAvailableDynamically();                         // slot 95, 0x00543AB0
    virtual C2DVector GetDynamicSize() const;                        // slot 111, 0x00543A60
    virtual const CWideString& GetValue();                           // slot 142, 0x00543AC0
    virtual void SetValue(const CCharString&);                       // slot 145, 0x00541E60
    virtual void SetValue(const CWideString&);                       // slot 144, 0x00541F20
    virtual void SetValue(unsigned long);                            // slot 143, 0x00542140
    virtual CCountedPointer<CFontBank>& GetFont();                   // slot 146, 0x00543AD0
    virtual void SetTextWindow(C2DBoxF&);                            // slot 147, 0x00543AE0
    virtual void ClearIcons();                                       // slot 148, 0x00542730
    virtual void AddIcon(long, long, EIconFlags);                    // slot 149, 0x00542780
    virtual void InitIcons();                                        // slot 150, 0x005430F0
    virtual ETextAlignement GetAlignment() const;                    // slot 151, 0x00543B10
    void GetDataFromDef();                                           // Ego_r ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ
protected:
    long FitInWindow(CWideString&, const C2DBoxF&);                  // Ego_r ?FitInWindow@CIconText@NUISystem@@IAEJAAVCWideString@@ABVC2DBoxF@@@Z
    CWideString Text;                                                // +0x15c
    CWideString DisplayText;                                         // +0x160
    CCountedPointer<CFontBank> PFont;                                // +0x164
    bool Unavailable;                                                // +0x16c
    CEnginePrimitiveHandle TextHandle;                               // +0x170
    CEnginePrimitiveHandle DropShadowHandle;                         // +0x178
    std::vector<CEnginePrimitiveHandle> IconHandles;                 // +0x180
    std::vector<CEmbeddedIcon> Icons;                                // +0x18c
    C2DBoxF Window;                                                  // +0x198
    C2DVector PositionOffset;                                        // +0x1a8
    unsigned char NoRenderNextFrame;                                 // +0x1b0
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CIconText) == 0x1b4);
#endif
