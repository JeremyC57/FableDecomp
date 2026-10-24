// behaviour test for 0x00AE2D90 CParticleProgram::AddExportParticle: append (name, particle) to the export map and mark
// it dirty; a full vector grows through the overflow path
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct SEntry { const char* first; unsigned long second; };
struct CExportMap {
    SEntry* Start; SEntry* Finish; SEntry* EndOfStorage; bool Dirty;
    void InsertOverflow(const SEntry& x)
    {
        unsigned int n = (unsigned int)(Finish - Start);
        unsigned int len = n ? 2 * n : 1;
        SEntry* p = (SEntry*)std::malloc(len * sizeof(SEntry));
        for (unsigned int i = 0; i < n; ++i) p[i] = Start[i];
        p[n] = x;
        std::free(Start);
        Start = p; Finish = p + n + 1; EndOfStorage = p + len;
    }
    void push_back(const SEntry& x) { if (Finish != EndOfStorage) { *Finish = x; ++Finish; } else InsertOverflow(x); }
    void Add(const char* key, unsigned long value) { SEntry e; e.first = key; e.second = value; push_back(e); Dirty = true; }
};
struct CParticleProgram {
    CExportMap ExportParticles;
    void AddExportParticle(const char* name, unsigned long particle) { ExportParticles.Add(name, particle); }
};
int main()
{
    CParticleProgram p;
    p.ExportParticles.Start = p.ExportParticles.Finish = p.ExportParticles.EndOfStorage = 0;
    p.ExportParticles.Dirty = false;
    p.AddExportParticle("tip", 4);
    p.AddExportParticle("root", 0);
    p.AddExportParticle("mid", 2);
    if (!p.ExportParticles.Dirty || p.ExportParticles.Finish - p.ExportParticles.Start != 3) { std::printf("00ae2d90_TEST FAIL\n"); return 1; }
    if (std::strcmp(p.ExportParticles.Start[1].first, "root") != 0 || p.ExportParticles.Start[2].second != 2) { std::printf("00ae2d90_TEST FAIL\n"); return 1; }
    std::free(p.ExportParticles.Start);
    std::printf("00ae2d90_TEST PASS\n");
    return 0;
}