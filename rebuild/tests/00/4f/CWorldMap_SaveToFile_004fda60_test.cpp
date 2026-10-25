// Behaviour test (0x004fda60): CWorldMap::SaveToFile builds the maps text then the regions text into one string and saves it
#include <cstdio>
#include <cstring>
class CAFile { public: char Written[64]; };
class CCharString {
public:
    char Text[64];
    CCharString() { Text[0] = 0; }
    void Append(const char* s) { std::strcat(Text, s); }
    void SaveToFile(CAFile& file) const { std::strcpy(file.Written, Text); }
};
class CWorldMap {
public:
    virtual ~CWorldMap() { }
    void SaveMapsToString(CCharString& text) { text.Append("maps;"); }
    void SaveRegionsToString(CCharString& text) { text.Append("regions;"); }
    virtual void SaveToFile(CAFile& file)
    {
        CCharString text;
        SaveMapsToString(text);
        SaveRegionsToString(text);
        text.SaveToFile(file);
    }
};
int main()
{
    CAFile file; file.Written[0] = 0;
    CWorldMap wm;
    wm.SaveToFile(file);
    if (std::strcmp(file.Written, "maps;regions;") != 0) { std::printf("004fda60_TEST FAIL\n"); return 1; }
    std::printf("004fda60_TEST PASS\n");
    return 0;
}