// CWideString::Contains(const CWideString& text) const -- true when the other wide string occurs anywhere in this one (wcsstr over the two texts, unassigned strings read as the shared empty string)
// Leaf: ?Contains@CWideString@@QBE_NABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099AF20, 70 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: Ego_r.pdb and FableWin.pdb list ??BCWideString@@QBEPB_WXZ, ?GetAsCharArray@CWideString@@QBEPB_WXZ,
//   ?Contains@CWideString@@QBE_NABV1@@Z in that order = retail 0x0099AF00 / 0x0099AF10 (both landed) / 0x0099AF20; the
//   landed NUISystem::CText::FitInWindow 0x0054F8E0 already declares Contains at this address. The catalog label
//   "std::_Iterator_base12::_Getcont" is propagated and wrong.
// Callee: wcsstr @ 0x00BFEB2E (MSVCR71 thunk, cdecl).
// Shape: `wcsstr(GetAsCharArray(), text.GetAsCharArray()) != 0` with both accessors inline (null storage -> the empty
//   wide literal 0x0129A8E0); VC evaluates the arguments right to left (the other text first) and tail-duplicates the
//   call + `neg/sbb/neg` bool into both arms of this text's null test.
// wchar_t: the rebuild compiles without /Zc:wchar_t (the <stddef.h> unsigned short typedef, mangled `G`), as the landed
//   CWideString accessors; the leaf has no wchar_t in it.
#include <stddef.h>
extern "C" wchar_t* __cdecl wcsstr(const wchar_t* string, const wchar_t* pattern);   // 0x00BFEB2E

class CWideStringData { public:
    wchar_t*      text;                         // +0x00
    unsigned long unknown04;
    unsigned long unknown08;
    long          owners;
};
extern wchar_t g_FableEmptyWideString_0129A8E0[1];   // 0x0129A8E0

class CWideString {
public:
    const wchar_t* GetAsCharArray() const
    {
        if (storage_ == 0)
            return g_FableEmptyWideString_0129A8E0;
        return storage_->text;
    }
    bool Contains(const CWideString& text) const;
private:
    CWideStringData* storage_;                  // +0x00
};

bool CWideString::Contains(const CWideString& text) const
{
    return wcsstr(GetAsCharArray(), text.GetAsCharArray()) != 0;
}