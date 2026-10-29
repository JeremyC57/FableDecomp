// CSubtitleRenderer::InitPlayerMode() -- swap the main player's subtitle player mode: remove the mode added last time, then add the one the display flags ask for (and remember it); the two button-prompt modes also fill an empty button 1 text with TEXT_GUI_NEXT, or empty button 1 / button 2 texts with TEXT_GUI_CONFIRM / TEXT_GUI_CANCEL
// Leaf: ?InitPlayerMode@CSubtitleRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048F939, 359 bytes, thiscall `ret`; frame-pointer size TU (as the landed CSubtitleRenderer rows).
// Identity (proven): (1) Ego_r.pdb names ?InitPlayerMode@CSubtitleRenderer@@AAEXXZ (private, no arguments), the only
//   CSubtitleRenderer method about player modes; (2) the body reads and writes Ego_r PlayerModeAdded (EPlayerMode, Ego_r +0x80,
//   retail +0x90), DisplayFlags (Ego_r +0x6C, retail +0x7C), Button1Text / Button2Text (Ego_r +0x54 / +0x58, retail +0x64 /
//   +0x68) -- every retail offset is the PDB one plus 0x10, the STLport deque in TextInfoQueue (0x28 vs Dinkumware 0x18);
//   (3) its callees are CPlayer::AddMode / RemoveMode on CPlayerManager::GetMainPlayer() and the landed private
//   CSubtitleRenderer::GetTextFromTextTag 0x0048F6BD with the L"TEXT_GUI_NEXT" / L"TEXT_GUI_CONFIRM" / L"TEXT_GUI_CANCEL"
//   tags (0x0123817C / 0x012381C0 / 0x012381E4); its only caller is 0x00492B4F.
// Label: the catalog's "CSubtitleRenderer_ProcessPlayerModes" is behavioural.
// Callees: CPlayerManager::GetMainPlayer() 0x00449970 (on g_pMainGameComponent 0x013B86A0 ->PPlayerManager +0x1C),
//   CPlayer::RemoveMode(EPlayerMode) 0x00634190, CPlayer::AddMode(EPlayerMode, bool) 0x006345C0, wcscmp 0x00BFE94C (import),
//   the STLport operator==(const wstring&, const wchar_t*) 0x00404280 (cdecl), CWideString(const unsigned short*) 0x0099B6B0,
//   CSubtitleRenderer::GetTextFromTextTag 0x0048F6BD, ~CWideString 0x0099B510. L"" 0x0122D70C.
// Shape: `ButtonNText == L""` is the PDB's CWideString::operator==(const wchar_t*) const (??8CWideString@@QBE_NPB_W@Z),
//   inline here: without a data block wcscmp(L"", text), else the out-of-line STLport wstring compare. It must be
//   __forceinline with the null case written as `if (wcscmp(..) == 0) return true; return false;` -- a `return
//   wcscmp(..) == 0` materialises the bool (`neg ; sbb al,al ; inc al`), and the caller-side two-test `goto` form of the
//   landed SetButton1OverrideText swaps ebx/edi across the whole body. The tag is an implicit CWideString conversion in
//   the frame word.
// Layout: engine/CSubtitleRenderer.h / CPlayerManager.h / CPlayer.h (renamed on include so the real classes can carry the
//   methods); the header keeps Button1Text / Button2Text as their data pointers and PlayerModeAdded as 4 bytes, so they
//   are read through reinterpret_cast at each use (a reference local hoists the `lea` and swaps registers).
// Enum values: EPlayerMode enumerator names are not in the dumped name sets; the modes are written as their retail values.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
#define CPlayerManager CPlayerManagerLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
typedef unsigned short wchar_t;                         // VC7.1 without /Zc:wchar_t
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);   // 0x00BFE94C

namespace std {
template <class _CharT> class char_traits { };       // STLport 4.6: a class template (mangles V)
template <class _Tp> class allocator { };
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {                                    // STLport 4.6 (the CWideString data block)
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
};
template <class _CharT, class _Traits, class _Alloc>
bool __cdecl operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __t);   // 0x00404280 (out of line)
}

class CWideStringData {                                 // Ego_r.pdb (STLport basic_string in retail)
public:
    std::basic_string<wchar_t> Data;                    // +0x00
};
class CWideString {
public:
    CWideStringData* PStringData;                       // +0x00
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
    __forceinline bool operator==(const wchar_t* text) const   // Ego_r ??8CWideString@@QBE_NPB_W@Z (inline in retail)
    {
        if (PStringData == 0) {
            if (wcscmp(text, L"") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

enum EPlayerMode { };                                   // Ego_r.pdb enum (enumerator names not recovered)
class CPlayer : public CPlayerLayout {                  // engine/CPlayer.h (retail 0x22C)
public:
    void AddMode(EPlayerMode mode, bool flag);          // 0x006345C0
    void RemoveMode(EPlayerMode mode);                  // 0x00634190
};
class CPlayerManager : public CPlayerManagerLayout {    // engine/CPlayerManager.h
public:
    CPlayer& GetMainPlayer();                           // 0x00449970
};
template <class T> class scoped_ptr {                   // Ego_r.pdb, 4 bytes
public:
    T* ptr;
    T* operator->() const { return ptr; }
};
class CMainGameComponent {
public:
    unsigned char              _pad00[0x1C];
    scoped_ptr<CPlayerManager> PPlayerManager;          // +0x1C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

class CSubtitleRenderer : public CSubtitleRendererLayout {   // engine/CSubtitleRenderer.h (retail offsets: Button1Text +0x64,
public:                                                 //   Button2Text +0x68, DisplayFlags +0x7C, PlayerModeAdded +0x90)
private:
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text);   // 0x0048F6BD
    void InitPlayerMode();
};

void CSubtitleRenderer::InitPlayerMode()
{
    CPlayer& player = g_pMainGameComponent->PPlayerManager->GetMainPlayer();
    if (reinterpret_cast<EPlayerMode&>(PlayerModeAdded) != 0) {
        player.RemoveMode(reinterpret_cast<EPlayerMode&>(PlayerModeAdded));
        reinterpret_cast<EPlayerMode&>(PlayerModeAdded) = (EPlayerMode)0;
    }
    if (DisplayFlags & 0x80) {
        player.AddMode((EPlayerMode)0x1A, false);
        reinterpret_cast<EPlayerMode&>(PlayerModeAdded) = (EPlayerMode)0x1A;
    }
    else if (DisplayFlags & 0x1000) {
        player.AddMode((EPlayerMode)0x2A, false);
        reinterpret_cast<EPlayerMode&>(PlayerModeAdded) = (EPlayerMode)0x2A;
    }
    else if (DisplayFlags & 0x400) {
        player.AddMode((EPlayerMode)0x27, false);
        reinterpret_cast<EPlayerMode&>(PlayerModeAdded) = (EPlayerMode)0x27;
    }
    else if (DisplayFlags & 0x2) {
        player.AddMode((EPlayerMode)0x0E, false);
        reinterpret_cast<EPlayerMode&>(PlayerModeAdded) = (EPlayerMode)0x0E;
        if (reinterpret_cast<CWideString&>(Button1Text_PStringData) == L"")
            GetTextFromTextTag(L"TEXT_GUI_NEXT", reinterpret_cast<CWideString&>(Button1Text_PStringData));
    }
    else if (DisplayFlags & 0x4) {
        player.AddMode((EPlayerMode)0x0F, false);
        reinterpret_cast<EPlayerMode&>(PlayerModeAdded) = (EPlayerMode)0x0F;
        if (reinterpret_cast<CWideString&>(Button1Text_PStringData) == L"")
            GetTextFromTextTag(L"TEXT_GUI_CONFIRM", reinterpret_cast<CWideString&>(Button1Text_PStringData));
        if (reinterpret_cast<CWideString&>(Button2Text_PStringData) == L"")
            GetTextFromTextTag(L"TEXT_GUI_CANCEL", reinterpret_cast<CWideString&>(Button2Text_PStringData));
    }
}