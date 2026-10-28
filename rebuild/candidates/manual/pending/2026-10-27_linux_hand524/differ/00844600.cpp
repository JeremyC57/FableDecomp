// CCreatureAction_Read::PerformAction() -- for the local player only: look the action's text tag up in the game text bank (falling back to the tag itself when the bank has no text for it) and show it as a cut-scene subtitle in the player GUI's subtitle renderer (cut-scene main font, flags 0x123, 4 seconds)
// Grade: DIFFER(222v222) -- register choice only: retail keeps this in esi and the text bank in edi (push esi first, push edi after the Local test); ours swaps them (this in edi, bank in esi). Tried: font through CCountedPointer operator* (adds a dead add eax,1614Ch; .Data removes it), a const CDataBank& bank local, a const CWideString& tag local -- all inert.
// Leaf: ?PerformAction@CCreatureAction_Read@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844600, 222 bytes, thiscall, `ret`. Vtable 0x01270AC4 slot 3.
// Identity (proven): RTTI vtable 0x01270AC4 (.?AVCCreatureAction_Read@@) slot 3 (PerformAction); Ego_r.pdb / FableWin.pdb hold
//   ?PerformAction@CCreatureAction_Read@@UAEXXZ; the class's only data member is TextTag (CWideString, Ego_r +0xB4 = retail
//   +0xA8), the string this body converts and shows.
// Callees: CCreatureActionBase::PeekPlayer() const 0x00692920 (protected; Ego_r donor order PeekDefinitionManager, PeekPlayer,
//   GetPlayer, GetWorld = retail 0x00692910/20/30/40), CWideString::ToCharString() const 0x0099B2C0 (FableWin),
//   NGameText::CDataBank::GetTextBySymbol(const CCharString&) const 0x009C95E0 (returns const CWideString), CCharString::
//   ~CCharString() 0x0099EAE0, wcscmp 0x00BFE94C, std::__char_traits_base<unsigned short, unsigned short>::compare
//   0x00403C60 (STLport 4.6 static, the inline wide basic_string == const wchar_t*), CWideString::operator=(const CWideString&)
//   0x0099B7D0 (PDB returns const CWideString&), CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CThing::PeekComponent()
//   const 0x004C7A70 (Ego_r donor order ... PeekThingSearchTools, PeekComponent, GetSampleBank, PeekSampleBank, GetWorld =
//   retail 0x004C7A60/70/80/90/A0), CSubtitleRenderer::SetText(const CFontBank&, const CWideString&, unsigned long, float,
//   bool, bool) 0x00492E4B (PDB), CWideString::~CWideString() 0x0099B510.
// Globals (descriptive names, no PDB data symbols): 0x013B86A0 the main game component pointer (its +0x14 is Ego_r
//   CMainGameComponent::PTextBank), 0x013B8790 the player GUI pointer (its +0x18 is Ego_r CPlayerGui::SubtitleRenderer).
// Layout: Ego_r CPlayer::Local +0x208 (engine/CPlayer.h), CMainGameComponent PTextBank +0x14 / PCutSceneMainFont +0x1614C,
//   CPlayerGui::SubtitleRenderer +0x18 (engine/CPlayerGui.h); the creature action classes are modelled locally because
//   engine/CCreatureActionBase.h is flat.
// Shape: `CWideString::operator==(const wchar_t*)` inline (Ego_r ??8CWideString@@QBE_NPB_W@Z): no data block -> wcscmp, else
//   the inline STLport `size() == length(s) && compare(...) == 0` (the length of L"" folds to 0).
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#include "engine/CPlayer.h"
#define CPlayerGui CPlayerGuiLayout
#include "engine/CPlayerGui.h"
#undef CPlayerGui
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
typedef unsigned short wchar_t;                              // VC7.1 without /Zc:wchar_t
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);   // 0x00BFE94C
extern "C" unsigned int __cdecl wcslen(const wchar_t* s);

namespace std {
template <class _CharT, class _IntT> class __char_traits_base {
public:
    static bool eq(const _CharT& __c1, const _CharT& __c2) { return __c1 == __c2; }
    static int compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60 (out of line)
    static unsigned int length(const _CharT* __s)
    {
        const _CharT _NullChar = _CharT();
        unsigned int __i;
        for (__i = 0; !eq(__s[__i], _NullChar); ++__i)
            {}
        return __i;
    }
};
template <class _CharT> class char_traits : public __char_traits_base<_CharT, _CharT> { };
template <class _Tp> class allocator { };
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {                                         // STLport 4.6 (the CWideString data block)
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const _CharT* data() const { return _M_start; }
};
template <class _CharT, class _Traits, class _Alloc>
__forceinline bool operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __t)
{
    unsigned int __n = _Traits::length(__t);
    return __s.size() == __n && _Traits::compare(__s.data(), __t, __n) == 0;
}
}

class CCharString {
public:
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
class CWideStringData {
public:
    std::basic_string<wchar_t> Data;                         // +0x00
    __forceinline bool operator==(const wchar_t* text) const { return Data == text; }
};
class CWideString {
public:
    CWideString(const CWideString& other);
    ~CWideString();                                          // 0x0099B510
    const CWideString& operator=(const CWideString& other);  // 0x0099B7D0
    CCharString ToCharString() const;                        // 0x0099B2C0
    __forceinline bool operator==(const wchar_t* text) const
    {
        if (PStringData == 0) {
            if (wcscmp(text, L"") == 0)
                return true;
            return false;
        }
        return *PStringData == text;
    }
    CWideStringData* PStringData;                            // +0x00
};
class CFontBank;
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
template <class T> class CCountedPointer {
public:
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Data;
    void* Info;
};
class CMainGameComponent {
public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;         // +0x14
    unsigned char _pad1c[0x1614c - 0x1c];
    CCountedPointer<CFontBank> PCutSceneMainFont;            // +0x1614C
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
class CSubtitleRenderer : public CSubtitleRendererLayout {
public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool a, bool b);   // 0x00492E4B
};
class CPlayerGui : public CPlayerGuiLayout { };
extern CPlayerGui* g_pPlayerGui;                             // 0x013B8790
class CBaseObject;
class CThing {
public:
    const CMainGameComponent& PeekComponent() const;         // 0x004C7A70
};
class CThingCreatureBase : public CThing { };
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void PerformAction() = 0;
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _pad10[0xa8 - 0x10];
protected:
    const CPlayer& PeekPlayer() const;                       // 0x00692920
};
class CCreatureAction_Read : public CCreatureActionBase {
public:
    virtual void PerformAction();
    CWideString TextTag;                                     // +0xA8
};

void CCreatureAction_Read::PerformAction()
{
    if (PeekPlayer().Local) {
        CWideString text = g_pMainGameComponent->PTextBank->GetTextBySymbol(TextTag.ToCharString());
        if (text == L"")
            text = TextTag;
        reinterpret_cast<CSubtitleRenderer&>(g_pPlayerGui->SubtitleRenderer).SetText(
            *PCreature->PeekComponent().PCutSceneMainFont.Data, text, 0x123, 4.0f, false, true);
    }
}
