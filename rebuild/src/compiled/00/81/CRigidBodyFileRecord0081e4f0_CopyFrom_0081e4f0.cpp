// CRigidBodyFileRecord_0081e4f0::CopyFrom(const CRigidBodyFileRecord_0081e4f0& other) -- copy the 16-byte key, the 32-char name (snprintf + forced terminator) and two words; clear the three trailing words
// Leaf: CopyFrom
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081E4F0, length 90 bytes; one caller, 0x00825633 inside the active-file rigid body
// constructor CAFile_ConstructRigidBody (0x00825590). The chunk label "CTCPhysicsRigidBody::UpdateInterpolation" is propagated:
// the body is a packed record copy, not physics-component code. The record type is unidentified, so it is named by address.
// Callees: _snprintf @ 0x00BFEB96 (__cdecl, "%s" @ 0x012724BC).
// Shape: the 16-byte block at +5 is a POD struct assignment through two `lea` bases; the name is copied with
// `_snprintf(m_name, 32, "%s", other.m_name)` and its last byte (+0x34) cleared from the `xor eax,eax` register that also feeds
// the three trailing zero stores; the two words at +0x35 / +0x39 are separate scalar members (direct offset moves).
// Layout: packed (members at odd offsets), descriptive member names.
extern "C" int __cdecl _snprintf(char* buffer, unsigned int count, const char* format, ...);

struct SKey16 { unsigned long m_words[4]; };

#pragma pack(push, 1)
struct CRigidBodyFileRecord_0081e4f0 {
    void*         m_vtable;                    // +0x00
    unsigned char m_flag;                      // +0x04
    SKey16        m_key;                       // +0x05
    char          m_name[0x20];                // +0x15 .. +0x34
    long          m_valueA;                    // +0x35
    long          m_valueB;                    // +0x39
    long          m_count0;                    // +0x3d
    long          m_count1;                    // +0x41
    long          m_count2;                    // +0x45
    void CopyFrom(const CRigidBodyFileRecord_0081e4f0& other);
};
#pragma pack(pop)

void CRigidBodyFileRecord_0081e4f0::CopyFrom(const CRigidBodyFileRecord_0081e4f0& other)
{
    m_key = other.m_key;
    _snprintf(m_name, sizeof(m_name), "%s", other.m_name);
    m_name[sizeof(m_name) - 1] = 0;
    m_valueA = other.m_valueA;
    m_valueB = other.m_valueB;
    m_count0 = 0;
    m_count1 = 0;
    m_count2 = 0;
}