#ifndef FABLE_ENGINE_NUISYSTEM_CEDITBOX_H
#define FABLE_ENGINE_NUISYSTEM_CEDITBOX_H
// NUISystem::CEditBox (new, chunk 753): RTTI .?AVCEditBox@NUISystem@@, single direct base CChangingStateComponent
// (rtti_bases: CUIInterface@0, CObserver@4, CActionParam@0x18). Vtables 0x01246B8C (+0, 156 slots), 0x01246B64 (+4: slot 1
// ProcessEvent 0x00540120), 0x01246B5C (+0x18). Overrides read from the primary vtable: 0 ??_G 0x00540DD0, 43 Initialise
// 0x00540620. New virtuals, slot order checked against the FableWin.pdb donor order of the inline ones (GetText,
// AddActionOnEnter, AddActionOnEscape, IsActive, SetPasswordBox, SetMaxLength, SetAllowedCharacters, SetForbiddenCharacters)
// and the bodies:
//   142 GetText 0x00540CF0 (copy of +0x164), 143 SetText 0x00540180, 144 GetTextComponent 0x005402C0, 145 AddCharacter
//   0x0053FE50, 146 KeyPressed 0x00540320, 147 UpdateDisplayText 0x00540980, 148 AddActionOnEnter 0x00540D70, 149
//   AddActionOnEscape 0x00540DA0, 150 SetActive 0x005401B0, 151 IsActive 0x00540D10 (+0x178), 152 SetPasswordBox 0x0053FE30
//   (+0x179), 153 SetMaxLength 0x00540D20 (+0x180), 154 SetAllowedCharacters 0x00540D30 (+0x184), 155 SetForbiddenCharacters
//   0x00540D40 (+0x188).
// Layout: Ego_r.pdb CEditBox (sizeof 0x1CC) with the retail 0x15C base and 4-byte STLport lists (the landed ~CEditBox
// 0x005408F0, SetText 0x00540180, SetActive 0x005401B0): ActionsOnEnter +0x15C, ActionsOnEscape +0x160, Text +0x164,
// CursorPos +0x168, LeftMostCharacter +0x16C, RightMostCharacter +0x170, Adjustment +0x174, Active +0x178, IsPasswordBox
// +0x179, UsesIME +0x17A, DisallowSpaceAsFirstChar +0x17B, DeleteAllOnKeyPress +0x17C, MaxLength +0x180,
// AllowedCharacters +0x184, ForbiddenCharacters +0x188; sizeof 0x18C.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CEditBox : public CChangingStateComponent {
public:
    CEditBox(long);
    virtual ~CEditBox();
    virtual void Initialise();
    virtual void ProcessEvent(EEvent);
    virtual CWideString GetText() { return Text; }                                         // slot 142
    virtual void SetText(const CWideString&);                                              // slot 143
    virtual CComponent* GetTextComponent();                                                // slot 144
    virtual void AddCharacter(wchar_t);                                                    // slot 145
    virtual void KeyPressed(wchar_t);                                                      // slot 146
    virtual void UpdateDisplayText();                                                      // slot 147
    virtual void AddActionOnEnter(CCountedPointer<CAction>&);                              // slot 148
    virtual void AddActionOnEscape(CCountedPointer<CAction>&);                             // slot 149
    virtual void SetActive(bool);                                                          // slot 150
    virtual bool IsActive() { return Active; }                                             // slot 151
    virtual void SetPasswordBox(bool isPasswordBox) { IsPasswordBox = isPasswordBox; }     // slot 152
    virtual void SetMaxLength(long maxLength) { MaxLength = maxLength; }                   // slot 153
    virtual void SetAllowedCharacters(const CWideString& characters) { AllowedCharacters = characters; }       // slot 154
    virtual void SetForbiddenCharacters(const CWideString& characters) { ForbiddenCharacters = characters; }   // slot 155
    void SetAsDeleteAllOnKeyPress(bool);
protected:
    std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > > ActionsOnEnter;    // +0x15c
    std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > > ActionsOnEscape;   // +0x160
    CWideString Text;                                                                      // +0x164
    long CursorPos;                                                                        // +0x168
    long LeftMostCharacter;                                                                // +0x16c
    long RightMostCharacter;                                                               // +0x170
    long Adjustment;                                                                       // +0x174
    bool Active;                                                                           // +0x178
    bool IsPasswordBox;                                                                    // +0x179
    bool UsesIME;                                                                          // +0x17a
    bool DisallowSpaceAsFirstChar;                                                         // +0x17b
    bool DeleteAllOnKeyPress;                                                              // +0x17c
    long MaxLength;                                                                        // +0x180
    CWideString AllowedCharacters;                                                         // +0x184
    CWideString ForbiddenCharacters;                                                       // +0x188
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CEditBox) == 0x18c);
#endif
