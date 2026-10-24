// NGameText::ConstructString(CWideString& output, const CWideString& replacement) -- replace the "%1" placeholder of a game-text string with a string; false when the text has no %1
// Leaf: ?ConstructString@NGameText@@YI_NAAVCWideString@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7980, 76 bytes (+ int3 padding), __fastcall (output in ecx,
// replacement in edx, plain `ret`).
// Name: FableWin.pdb `?ConstructString@NGameText@@YI_NAAVCWideString@@ABV2@@Z`.
// Callees: CWideString::CWideString(const unsigned short*) @ 0x0099B6B0 (literal L"%1" @ 0x0129B450),
// CWideString::Contains(const CWideString&) const @ 0x0099AF20, CWideString::Replace(const CWideString&,
// const CWideString&) @ 0x0099C1E0, CWideString::~CWideString @ 0x0099B510.
// Shape: the key is a named local built from the literal; the positive `if (Contains) { Replace; return true; }`
// keeps the found arm on the fall-through (the `!Contains` early return inverts the blocks), the miss returns false, and VC duplicates the key's destructor into both exits (`lea ecx,[esp+8]` is hoisted above the
// `je` because both arms need it). Without /Zc:wchar_t the literal is `unsigned short`.

class CWideString {
public:
    CWideString(const unsigned short* text);                                   // 0x0099B6B0
    ~CWideString();                                                             // 0x0099B510
    bool Contains(const CWideString& text) const;                               // 0x0099AF20
    void Replace(const CWideString& search, const CWideString& replacement);    // 0x0099C1E0
private:
    void* PData;
};

namespace NGameText {

extern const unsigned short g_FirstPlaceholder[];      // L"%1" @ 0x0129B450

bool __fastcall ConstructString(CWideString& output, const CWideString& replacement)
{
    CWideString key(g_FirstPlaceholder);
    if (output.Contains(key)) {
        output.Replace(key, replacement);
        return true;
    }
    return false;
}

}