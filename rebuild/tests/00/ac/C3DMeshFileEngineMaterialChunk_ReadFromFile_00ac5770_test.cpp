#include <cstdio>
#include <cstring>
// Behaviour test for C3DMeshFileEngineMaterialChunk::ReadFromFile (retail 0x00AC5770): a version-4 EMTL chunk carries
// the map flags and an optional set of map file names; an older chunk always gets a name set, reads the degenerate
// flag only from version 3 on, and recomputes the map flags; in both cases the super-chunk reader gets the bytes left.
class CStream { public:
    const unsigned char* m_data; unsigned long m_pos;
    CStream(const unsigned char* d) : m_data(d), m_pos(0) {}
    unsigned long GetPosition() const { return m_pos; }
    unsigned long ReadULONG() { unsigned int v; std::memcpy(&v, m_data + m_pos, 4); m_pos += 4; return v; }
    long ReadSLONG() { return (long)(int)ReadULONG(); }
    bool ReadEBOOL() { return ReadULONG() != 0; }
    const char* ReadNullTerminatedString() { const char* s = (const char*)m_data + m_pos; m_pos += (unsigned long)std::strlen(s) + 1; return s; }
};
struct CMapFilenames { char DiffuseMap[16], BumpMap[16], ReflectionMap[16], IlluminationMap[16]; };
struct CChunk {
    long MaterialIndex; char Name[16]; CMapFilenames* MapFilenames; CMapFilenames Storage;
    unsigned long DiffuseMapBankIndex, BumpMapBankIndex, ReflectionMapBankIndex, IlluminationMapBankIndex, MapFlags, SelfIllumination;
    bool TwoSided, Transparent, BooleanAlpha, DegenerateTriangles;
    unsigned long SuperChunkBytes; bool Recalculated;
    void SuperChunkRead(CStream&, unsigned long bytes) { SuperChunkBytes = bytes; }
    void CalculateMapFlags() { Recalculated = true; MapFlags = (DiffuseMapBankIndex ? 1 : 0) | (BumpMapBankIndex ? 2 : 0); }
    void ReadNames(CStream& in)
    {
        MapFilenames = &Storage;
        std::strcpy(MapFilenames->DiffuseMap, in.ReadNullTerminatedString());
        std::strcpy(MapFilenames->BumpMap, in.ReadNullTerminatedString());
        std::strcpy(MapFilenames->ReflectionMap, in.ReadNullTerminatedString());
        std::strcpy(MapFilenames->IlluminationMap, in.ReadNullTerminatedString());
    }
    void ReadFromFile(CStream& in, unsigned long chunk_size)
    {
        unsigned long start_pos = in.GetPosition();
        unsigned long version = in.ReadULONG();
        if (version == 4) {
            MaterialIndex = in.ReadSLONG();
            std::strcpy(Name, in.ReadNullTerminatedString());
            MapFlags = in.ReadULONG();
            DiffuseMapBankIndex = in.ReadULONG(); BumpMapBankIndex = in.ReadULONG();
            ReflectionMapBankIndex = in.ReadULONG(); IlluminationMapBankIndex = in.ReadULONG();
            if (in.ReadEBOOL()) ReadNames(in); else MapFilenames = 0;
            SelfIllumination = in.ReadULONG();
            TwoSided = in.ReadEBOOL(); Transparent = in.ReadEBOOL(); BooleanAlpha = in.ReadEBOOL(); DegenerateTriangles = in.ReadEBOOL();
            unsigned long header_size = in.GetPosition() - start_pos;
            SuperChunkRead(in, chunk_size - header_size);
        }
        else {
            MaterialIndex = in.ReadSLONG();
            std::strcpy(Name, in.ReadNullTerminatedString());
            DiffuseMapBankIndex = in.ReadULONG(); BumpMapBankIndex = in.ReadULONG();
            ReflectionMapBankIndex = in.ReadULONG(); IlluminationMapBankIndex = in.ReadULONG();
            ReadNames(in);
            SelfIllumination = in.ReadULONG();
            TwoSided = in.ReadEBOOL(); Transparent = in.ReadEBOOL(); BooleanAlpha = in.ReadEBOOL();
            if (version > 2) DegenerateTriangles = in.ReadEBOOL();
            unsigned long header_size = in.GetPosition() - start_pos;
            SuperChunkRead(in, chunk_size - header_size);
            CalculateMapFlags();
        }
    }
};
static unsigned long Put(unsigned char* b, unsigned long p, unsigned int v) { std::memcpy(b + p, &v, 4); return p + 4; }
static unsigned long PutS(unsigned char* b, unsigned long p, const char* s) { std::strcpy((char*)b + p, s); return p + (unsigned long)std::strlen(s) + 1; }
int main()
{
    unsigned char b[256]; unsigned long p = 0;
    p = Put(b, p, 4); p = Put(b, p, 7); p = PutS(b, p, "stone"); p = Put(b, p, 0x13);
    p = Put(b, p, 1); p = Put(b, p, 2); p = Put(b, p, 3); p = Put(b, p, 4);
    p = Put(b, p, 1); p = PutS(b, p, "d.tga"); p = PutS(b, p, "b.tga"); p = PutS(b, p, ""); p = PutS(b, p, "i.tga");
    p = Put(b, p, 9); p = Put(b, p, 1); p = Put(b, p, 0); p = Put(b, p, 1); p = Put(b, p, 1);
    CChunk c; std::memset(&c, 0, sizeof(c));
    CStream s4(b); c.ReadFromFile(s4, 200);
    int ok = c.MaterialIndex == 7 && std::strcmp(c.Name, "stone") == 0 && c.MapFlags == 0x13 && c.IlluminationMapBankIndex == 4
          && c.MapFilenames && std::strcmp(c.MapFilenames->BumpMap, "b.tga") == 0 && std::strcmp(c.MapFilenames->IlluminationMap, "i.tga") == 0
          && c.SelfIllumination == 9 && c.TwoSided && !c.Transparent && c.BooleanAlpha && c.DegenerateTriangles
          && c.SuperChunkBytes == 200 - p && !c.Recalculated;
    p = 0;                                            // version 4 without names
    p = Put(b, p, 4); p = Put(b, p, 1); p = PutS(b, p, "x"); p = Put(b, p, 5);
    p = Put(b, p, 0); p = Put(b, p, 0); p = Put(b, p, 0); p = Put(b, p, 0); p = Put(b, p, 0);
    p = Put(b, p, 0); p = Put(b, p, 0); p = Put(b, p, 0); p = Put(b, p, 0); p = Put(b, p, 0);
    CStream s4b(b); c.ReadFromFile(s4b, 100);
    ok = ok && c.MapFilenames == 0 && c.MapFlags == 5 && c.SuperChunkBytes == 100 - p;
    p = 0;                                            // version 2: names always, no degenerate flag, flags recomputed
    p = Put(b, p, 2); p = Put(b, p, 3); p = PutS(b, p, "old");
    p = Put(b, p, 6); p = Put(b, p, 0); p = Put(b, p, 0); p = Put(b, p, 0);
    p = PutS(b, p, "a"); p = PutS(b, p, "b"); p = PutS(b, p, "c"); p = PutS(b, p, "d");
    p = Put(b, p, 2); p = Put(b, p, 0); p = Put(b, p, 1); p = Put(b, p, 0);
    c.DegenerateTriangles = true;
    CStream s2(b); c.ReadFromFile(s2, 80);
    ok = ok && c.MaterialIndex == 3 && c.MapFilenames && std::strcmp(c.MapFilenames->ReflectionMap, "c") == 0 && c.Transparent
         && c.DegenerateTriangles && c.Recalculated && c.MapFlags == 1 && c.SuperChunkBytes == 80 - p;
    if (!ok) { std::printf("00ac5770_TEST FAIL\n"); return 1; }
    std::printf("00ac5770_TEST PASS\n");
    return 0;
}