// Behaviour test for 004fc170 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CMapInfo { public: int Id; unsigned char Pad[0x44]; };
struct CArrayMapInfo { CMapInfo* Start; CMapInfo* Finish; CMapInfo* End; const CMapInfo& operator[](unsigned int n) const { return *(Start + n); } };
class CWorldMap { public: CArrayMapInfo Maps; const CMapInfo& GetMapWithIndex(int i) const { return Maps[i]; } };
int main() {
    CMapInfo m[3]; for (int i = 0; i < 3; ++i) m[i].Id = 10 + i;
    CWorldMap w; w.Maps.Start = m; w.Maps.Finish = m + 3; w.Maps.End = m + 3;
    if (&w.GetMapWithIndex(2) != &m[2] || w.GetMapWithIndex(1).Id != 11 || sizeof(CMapInfo) != 0x48) { std::printf("004fc170_TEST FAIL\n"); return 1; }
    { std::printf("004fc170_TEST PASS\n"); return 0; }
}