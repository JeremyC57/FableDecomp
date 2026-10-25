// CString::Append(double) -- libpk string: append a number formatted with two decimals ("%.2f")
// Leaf: ?Append@CString@@QAEXN@Z
// Flags: /GS
// Retail: Fable.exe @ 0x00C0DD60, 72 bytes, `ret 8` (/GS cookie 0x0139C8A8). Descriptive name: the libpk audio library
// (0x00C08AE0..) has no PDB or RTTI. Label correction: the manifest row "CWorldMap::ApplyScriptedMapBrushFloat" is a
// propagation; the only caller 0x00C0DED0 is the `operator+=(double)` wrapper (`fld ; sub esp,8 ; fstp ; call ;
// mov eax,esi`), next to 0x00C0DEC0 = `operator+=(const char*)` calling 0x00C0DD40.
// Callees: sprintf (MSVCR71 import through 0x01440128), CString::Append(const char*) 0x00C0DD40 (`ConcatInPlace(
// SafeStrlen(text), text)`: 0x00C0BCC0 / 0x00C0C270; the landed source of 0x00C0DD40 calls it
// "CWorldMap::ApplyScriptedMapBrush", also a propagated label), __security_check_cookie 0x00BFE9F9.
// Shape: a 64-byte character buffer under the /GS cookie (0x44 frame); the double is re-pushed with
// `fld qword ; sub esp,8 ; fstp qword [esp]` and the format string literal 0x012B21D4 is "%.2f".
extern "C" __declspec(dllimport) int __cdecl sprintf(char* buffer, const char* format, ...);

class CString {
public:
    char* m_pchData;                            // +0x00
    void Append(const char* text);              // 0x00C0DD40
    void Append(double value);
};

void CString::Append(double value)
{
    char buffer[64];
    sprintf(buffer, "%.2f", value);
    Append(buffer);
}