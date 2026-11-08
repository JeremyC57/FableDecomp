// CString::Left(int nCount) const -- MFC-style CString::Left: clamp a negative count to 0; a count at or past the length returns a copy of *this, else AllocCopy(dest, nCount, 0, 0) into an empty string returned by value
// Leaf: ?Left@CString@@QBE?AV1@H@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0E1F0, 208 bytes, thiscall `ret 8` (hidden result + count), EH-framed (dest, and the
//   result-constructed flag in the one-dword `push ecx` frame). The oracle's 368-byte extent also holds two more members of the
//   same class, neither called nor referenced: 0x00C0E2C0 SpanIncluding (ASSERT(lpszCharSet), `Left(strspn(m_pchData, set))`,
//   the MSVCR71 strspn import 0x0143FFD8) and 0x00C0E310 SpanExcluding (strcspn 0x0143FFF4): function starts to add to
//   the manifest (graded against the first 208 bytes).
// Identity: libpk's copy of MFC 6 strcore.cpp (LIBRARY_LINKING.md section 12); the text is MFC's Left, the sibling of the landed
//   Right 0x00C0E0F0 / Mid 0x00C0DF50, and SpanIncluding / SpanExcluding call it (`call 0xc0e1f0`), which MFC's do.
//   Manifest label "CCharString::LeftSubstring" is propagated.
// Callees: GetData's ASSERT (g_pfnAssertHook 0x0143BB2C with "m_pchData != NULL\n" 0x012B1EC0, CDebugBreak 0x00C16750),
//   CString(const CString&) 0x00C0DAB0, AllocCopy 0x00C0BF90, ~CString 0x00C0BA40, the nil data _afxPchNil 0x0139C900 -- all
//   from the shared engine/CString.h (opt-ins BUILDERS / ALLOCCOPY, plus LEFT from this chunk's overlay: the declaration and
//   MFC's inline default constructor).
// Shape: as the landed Mid, `dest` lives in its own block: VC7.1 then homes it (and the ASSERT's CDebugBreak temporary) in the
//   dead count argument slot ([esp+20h]) and keeps a one-dword frame. Declared at function scope it gets a frame slot of its
//   own (`sub esp,8`, 210v208).
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_LEFT
#include "engine/CString.h"

CString CString::Left(int nCount) const
{
    if (nCount < 0)
        nCount = 0;
    if (nCount >= GetData()->nDataLength)
        return *this;
    {
        CString dest;
        AllocCopy(dest, nCount, 0, 0);
        return dest;
    }
}