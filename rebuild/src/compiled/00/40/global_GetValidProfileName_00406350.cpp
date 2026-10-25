// GetValidProfileName(const CWideString& name) -- copy a profile name with every character Windows forbids in a file name (\ / : * ? " < > |) replaced by '_'
// Leaf: ?GetValidProfileName@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00406350, 629 bytes (640 with padding), `__fastcall` with the hidden CWideString
//   result in ecx and `name` in edx, `ret`. FableWin.pdb: `?GetValidProfileName@@YI?AVCWideString@@ABV1@@Z`.
// Callers: 0x00406E3A, 0x00407069 (CUserProfileManager), 0x0055D341/0x0055DB59, 0x0058178E/0x00581B62, 0x0058FA93,
//   0x0071B868, 0x0071D449.
// Callees: CWideString(const CWideString&) 0x0099B720, CWideString(const unsigned short*) 0x0099B6B0,
//   CWideString::Replace(const CWideString& search, const CWideString& replacement) 0x0099C1E0, ~CWideString 0x0099B510.
// Shape: a named result copy at the bottom of a 12-byte frame; each Replace is a `__forceinline` helper with two NAMED
//   string locals (the "_" replacement first, then the searched character) in the two upper frame words, both built
//   before the `lea ; push` of their addresses (unnamed temporaries bound to the reference parameters push the
//   constructor's eax instead), destroyed in reverse, and the result is returned through the copy constructor into the hidden slot (VC7.1 has no NRVO) before
//   the local is destroyed. The wide literals are the 2-character pool strings 0x0122DE58..0x0122DE7C.
class CWideString {
public:
    CWideString(const unsigned short* text);                                  // 0x0099B6B0
    CWideString(const CWideString& other);                                    // 0x0099B720
    ~CWideString();                                                           // 0x0099B510
    void Replace(const CWideString& search, const CWideString& replacement);  // 0x0099C1E0
private:
    void* m_pData;
};

static __forceinline void ReplaceWithUnderscore(CWideString& text, const unsigned short* character)
{
    CWideString replacement(L"_");
    CWideString search(character);
    text.Replace(search, replacement);
}

CWideString __fastcall GetValidProfileName(const CWideString& name)
{
    CWideString result(name);
    ReplaceWithUnderscore(result, L"\\");
    ReplaceWithUnderscore(result, L"/");
    ReplaceWithUnderscore(result, L":");
    ReplaceWithUnderscore(result, L"*");
    ReplaceWithUnderscore(result, L"?");
    ReplaceWithUnderscore(result, L"\"");
    ReplaceWithUnderscore(result, L"<");
    ReplaceWithUnderscore(result, L">");
    ReplaceWithUnderscore(result, L"|");
    return result;
}