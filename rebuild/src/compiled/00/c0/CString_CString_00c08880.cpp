// CString::CString() -- MFC-style default constructor: point m_pchData at the shared empty string (g_pchNil)
// Leaf: ??0CString@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C08880, 11 bytes, thiscall, returns this.
// Identity (probable): the MFC Init() body `m_pchData = afxEmptyString`, here the libpk nil pointer g_pchNil (0x0139C900,
//   the same global the landed CAutoMapEntry 0x00C3BAA0 / 0x00C3BAF0 constructors store); `__ehvec_ctor` arrays pair it with
//   ~CString 0x00C0BA40 (landed). Label correction: the catalog's "Object_SetVtable" reads a data pointer, not a vtable.
extern char* g_pchNil;                          // 0x0139C900 shared empty CString data

class CString {
public:
    CString();
    char* m_pchData;                            // +0x00
};

CString::CString()
{
    m_pchData = g_pchNil;
}