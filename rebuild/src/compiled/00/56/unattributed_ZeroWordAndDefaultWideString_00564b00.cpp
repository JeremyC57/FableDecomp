// _unattributed::ZeroWordAndDefaultWideString() -- default constructor of an 8-byte record: a zeroed leading word and a default-constructed CWideString at +4
// Leaf: ??0CUnattributed_00564b00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00564B00, length 21 bytes.
// Callees: CWideString::CWideString() @ 0x0099AED0 (landed; the manifest's CCharString label on it is propagated).
// Identity: the manifest label CProfile::CProfile is propagated (the real CProfile is 0x48 bytes with the
// name at +0x30). No reference to the body was found; its TU neighbour is pair<unsigned long, CAnimationEntry>
// (0x00564B20), so this is most likely a std::pair<long, CWideString> default constructor. Landed as
// _unattributed with descriptive names.
// Shape: `lea ecx,[esi+4]` scheduled before the zero store of the first member.
typedef unsigned short wchar_t;                             // VC7.1 without /Zc:wchar_t

struct CWideString {
    wchar_t* m_pString;
    CWideString();                                          // 0x0099AED0
};

struct CUnattributed_00564b00 {
    long        m_first;                                    // +0x00
    CWideString m_second;                                   // +0x04
    CUnattributed_00564b00();
};

CUnattributed_00564b00::CUnattributed_00564b00()
    : m_first(0), m_second()
{
}