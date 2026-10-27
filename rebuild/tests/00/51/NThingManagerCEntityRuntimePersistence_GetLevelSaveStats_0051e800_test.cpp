// Behaviour test for 0051e800 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
struct CArrayU8 { unsigned char* Start; unsigned char* Finish; unsigned char* End; int Size() const { return (int)(Finish - Start); } };
struct CEntityScriptInfo { void* Script; CArrayU8 SaveData; int UncompressedSize; bool Valid; };
struct CArrayInfo { CEntityScriptInfo* Start; CEntityScriptInfo* Finish; CEntityScriptInfo* End; int Size() const { return (int)(Finish - Start); } const CEntityScriptInfo& operator[](int i) const { return Start[i]; } };
struct CEntityRuntimePersistence { CArrayInfo LevelPersistentEntityScripts; CArrayInfo GamePersistentEntityScripts;
    void GetLevelSaveStats(unsigned int& n, unsigned int& size, unsigned int& unc) const {
        n = 0; size = 0; unc = 0; int count = GamePersistentEntityScripts.Size();
        for (int i = 0; i < count; ++i) { const CEntityScriptInfo& e = GamePersistentEntityScripts[i];
            if (e.Valid) { ++n; size += e.SaveData.Size(); unc += e.UncompressedSize; } } } };
int main() {
    unsigned char buf[100]; CEntityScriptInfo e[3];
    e[0].SaveData.Start = buf; e[0].SaveData.Finish = buf + 10; e[0].UncompressedSize = 40; e[0].Valid = true;
    e[1].SaveData.Start = buf; e[1].SaveData.Finish = buf + 99; e[1].UncompressedSize = 500; e[1].Valid = false;
    e[2].SaveData.Start = buf; e[2].SaveData.Finish = buf + 25; e[2].UncompressedSize = 60; e[2].Valid = true;
    CEntityRuntimePersistence p; p.LevelPersistentEntityScripts.Start = p.LevelPersistentEntityScripts.Finish = 0;
    p.GamePersistentEntityScripts.Start = e; p.GamePersistentEntityScripts.Finish = e + 3;
    unsigned int n = 99, size = 99, unc = 99; p.GetLevelSaveStats(n, size, unc);
    if (n != 2 || size != 35 || unc != 100) { std::printf("0051e800_TEST FAIL\n"); return 1; }
    p.GamePersistentEntityScripts.Finish = e; p.GetLevelSaveStats(n, size, unc);
    if (n != 0 || size != 0 || unc != 0) { std::printf("0051e800_TEST FAIL\n"); return 1; }
    { std::printf("0051e800_TEST PASS\n"); return 0; }
}