// _unattributed::ZeroWordAndDefaultDateAndTime(long) -- constructor of a 32-byte record: a zeroed leading word and a default-constructed CDateAndTime at +4; the one stack argument is unused
// Leaf: ??0CUnattributed_0071f3d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F3D0, length 23 bytes.
// Callees: CDateAndTime::CDateAndTime() @ 0x009E9AB0 (landed; zeroes seven words).
// Identity: the manifest label CProfile::CProfile is propagated. No reference to the body was found; its TU
// neighbour constructs pair<CCharString const, pair<long, CDateAndTime> > (0x0071F4E0), so this is most
// likely the pair<long, CDateAndTime> part; the unused argument (`ret 4`) is kept as a long. Landed as
// _unattributed with descriptive names.
struct CDateAndTime {                                       // 28 bytes
    unsigned long m_words[7];
    CDateAndTime();                                         // 0x009E9AB0
};

struct CUnattributed_0071f3d0 {
    long         m_first;                                   // +0x00
    CDateAndTime m_second;                                  // +0x04
    CUnattributed_0071f3d0(long unused);
};

CUnattributed_0071f3d0::CUnattributed_0071f3d0(long)
    : m_first(0), m_second()
{
}
