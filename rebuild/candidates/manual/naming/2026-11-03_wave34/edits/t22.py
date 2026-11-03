# T22: 0x0051A240 is CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile(CAFile&, CASuspendableProcess*)
# (Ego_r 0x004E6DD0; struct_layouts_egor: CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3> = { ByteLength +0, SerialiseVersionNumber +4 },
# the 5-byte base of CFileFormatLevel). cx12 4333.
P = "rebuild/src/compiled/00/50/"
NAV = P + "CWorldMap_CreateNavMap_00500230.cpp"
DIM = P + "CWorldMap_GetLevelDimensions_00506bb0.cpp"
CHK = P + "CWorldMap_GetLevelStaticMapChecksum_00506c40.cpp"
BASE = """class CFileFormatLevel;
class COldFileFormatLevel3;
template <class TNew, class TOld> class CReplaceSerialise { public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    void ReadFromFile(CAFile& file, CASuspendableProcess* process);  // 0x0051A240 (<CFileFormatLevel,COldFileFormatLevel3>)
};

class CFileFormatLevel : public CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>
{ public:
"""
OLD2 = """class CFileFormatLevel
{ public:
    unsigned long ByteLength;                          // +0x00 CReplaceSerialise
    unsigned char SerialiseVersionNumber;              // +0x04
"""
EDITS = [
    (NAV, """class CFileFormatLevel { public:
    unsigned long ByteLength;                                    // +0x00 CReplaceSerialise
    unsigned char SerialiseVersionNumber;                        // +0x04
""", BASE),
    (NAV, "    void ReadFromFile(CAFile* file, CASuspendableProcess* process);   // 0x0051A240\n", ""),
    (NAV, "level.ReadFromFile(&file, &process);", "level.ReadFromFile(file, &process);"),
    (DIM, OLD2, BASE),
    (DIM, "    void ReadFromFile(CAFile* file, CASuspendableProcess* process);  // 0x0051A240\n", ""),
    (DIM, "level.ReadFromFile(&file, 0);", "level.ReadFromFile(file, 0);"),
    (CHK, OLD2, BASE),
    (CHK, "    void ReadFromFile(CAFile* file, CASuspendableProcess* process);  // 0x0051A240\n", ""),
    (CHK, "level.ReadFromFile(file.Data, 0);", "level.ReadFromFile(*file.Data, 0);"),
]
