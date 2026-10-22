// Behaviour test for CParticleSystem::ReadBinary (0x00AF4660): feeds a byte stream with the name, two flag bytes,
// the scale, two component records and their separators, and checks the system and the components it created.
#include <cstdio>
#include <cstring>
struct CCharString { int m_id; };
struct C3DVector { float X, Y, Z; };
struct CDataInputStream {
    const unsigned char* CurrentSourcePtr;
    long SourceChunkBytesRemaining;
    unsigned long StreamPos;
    int m_names;
    virtual CCharString ReadNullTerminatedString() { CCharString s; s.m_id = ++m_names; return s; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { std::memset(buffer, 0, size); }
    C3DVector Read3DVector() { C3DVector v; v.X = 1.0f; v.Y = 2.0f; v.Z = 3.0f; return v; }
    void Read(void* dst, long size)
    {
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    unsigned char ReadUByte() { unsigned char value; Read(&value, 1); return value; }
    unsigned long ReadULong() { unsigned int value; Read(&value, 4); return value; }
};
struct CPSCBase {
    int m_type; unsigned long m_instance; bool m_enabled; unsigned long m_payload;
    void ReadBinary(CDataInputStream* in) { m_payload = in->ReadULong(); }
};
static CPSCBase g_components[8];
static int g_count = 0;
struct CParticleSystem {
    bool Initialised : 1;
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    C3DVector Scale;
    CCharString Name;
    CPSCBase& AddComponent(const CCharString& typeName, const unsigned long& instanceName, bool enabled, bool)
    {
        CPSCBase& c = g_components[g_count++];
        c.m_type = typeName.m_id; c.m_instance = instanceName; c.m_enabled = enabled;
        return c;
    }
    void ReadBinary(CDataInputStream* in);
};
void CParticleSystem::ReadBinary(CDataInputStream* in)
{
    Name = in->ReadNullTerminatedString();
    Enabled = in->ReadUByte() != 0;
    ScaleParticles = in->ReadUByte() != 0;
    Scale = in->Read3DVector();
    unsigned long count = in->ReadULong();
    for (unsigned long i = 0; i < count; ++i) {
        CCharString typeName = in->ReadNullTerminatedString();
        unsigned long instanceName = in->ReadULong();
        bool enabled = in->ReadUByte() != 0;
        AddComponent(typeName, instanceName, enabled, false).ReadBinary(in);
        unsigned char separator;
        in->Read(&separator, 1);
    }
}
static void Put(unsigned char*& p, unsigned int v) { std::memcpy(p, &v, 4); p += 4; }
int main()
{
    unsigned char buffer[64];
    unsigned char* p = buffer;
    *p++ = 1; *p++ = 0;
    Put(p, 2);
    Put(p, 70); *p++ = 0; Put(p, 500); *p++ = 0x26;
    Put(p, 71); *p++ = 5; Put(p, 501); *p++ = 0x26;
    CDataInputStream in;
    in.CurrentSourcePtr = buffer; in.SourceChunkBytesRemaining = (long)(p - buffer); in.StreamPos = 0; in.m_names = 0;
    CParticleSystem s;
    std::memset(&s, 0, sizeof(s));
    s.ScaleParticles = true; s.Visible = true;
    s.ReadBinary(&in);
    int bad = 0;
    if (s.Name.m_id != 1 || !s.Enabled || s.ScaleParticles || !s.Visible) bad = 1;
    if (s.Scale.X != 1.0f || s.Scale.Z != 3.0f) bad = 2;
    if (g_count != 2) bad = 3;
    if (g_components[0].m_type != 2 || g_components[0].m_instance != 70 || g_components[0].m_enabled || g_components[0].m_payload != 500) bad = 4;
    if (g_components[1].m_type != 3 || g_components[1].m_instance != 71 || !g_components[1].m_enabled || g_components[1].m_payload != 501) bad = 5;
    if (in.SourceChunkBytesRemaining != 0) bad = 6;
    if (bad) { std::printf("00af4660_TEST FAIL\n"); return 1; }
    std::printf("00af4660_TEST PASS\n");
    return 0;
}