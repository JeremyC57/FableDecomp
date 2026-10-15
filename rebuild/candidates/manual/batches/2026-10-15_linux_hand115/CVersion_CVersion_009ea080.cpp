// CVersion::CVersion(unsigned char minor, unsigned char major, long build, long revision) -- store the four version fields
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EA080, length 32 bytes. No callees.
// Shape: no frame; the two bytes are loaded (dl = arg1, cl = arg2) before the stores, the stores follow the
// member declaration order (+0 = arg2, +1 = arg1, +4 = arg3, +8 = arg4), `ret 10h` and `this` returned in eax.
// Layout: 12 bytes, no vtable (no engine header for CVersion); member names descriptive.
struct CVersion {
    unsigned char m_major;      // +0x00 (second argument)
    unsigned char m_minor;      // +0x01 (first argument)
    long          m_build;      // +0x04
    long          m_revision;   // +0x08
    CVersion(unsigned char minor, unsigned char major, long build, long revision);
};

CVersion::CVersion(unsigned char minor, unsigned char major, long build, long revision)
    : m_major(major), m_minor(minor), m_build(build), m_revision(revision)
{
}
