// CRecord_006f3000::CRecord_006f3000(const CRecord_006f3000& other) -- memberwise copy: a long, a flag, a long, two flags, a CCharString, two retail vectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F3000, length 79 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, the 12-byte retail vector
// copy constructor @ 0x006F0810 (x2; the one the NShape::CShape point list uses).
// The manifest label "CBoast::CBoast" is propagated: CBoast's copy constructor is @ 0x004B73A0 and
// its layout (ID/Cost/Reward/strings) does not fit these bytes. This 0x2c-byte record cannot be
// named from the body (0x006Fxxxx shape module); members are typed by their copies: +0 long,
// +4 byte, +8 long, +0xc byte, +0xd byte, +0x10 CCharString, +0x14 vector, +0x20 vector.
// The copy constructor is written out of line so the TU carries the symbol.
struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector(const CRetailVector& other);  // 0x006F0810
};

struct C3DVector { float X, Y, Z; };

struct CRecord_006f3000 {
    long                     m_id;              // +0x00
    unsigned char            m_flag04;          // +0x04
    long                     m_value;           // +0x08
    unsigned char            m_flag0c;          // +0x0c
    unsigned char            m_flag0d;          // +0x0d
    CCharString              m_name;            // +0x10
    CRetailVector<C3DVector> m_points0;         // +0x14
    CRetailVector<C3DVector> m_points1;         // +0x20
    CRecord_006f3000(const CRecord_006f3000& other);
};

CRecord_006f3000::CRecord_006f3000(const CRecord_006f3000& other)
    : m_id(other.m_id), m_flag04(other.m_flag04), m_value(other.m_value), m_flag0c(other.m_flag0c)
    , m_flag0d(other.m_flag0d), m_name(other.m_name), m_points0(other.m_points0), m_points1(other.m_points1)
{
}
