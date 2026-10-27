// Test for 0x00A14E50 CNavigatorManager::SaveNavigationMapToFile: walks the quad-tree map list and saves only the first
// map whose Index matches, then stops; no match saves nothing.
#include <stdio.h>
class CAFile { public: int Saves; int LastId; };
class CNavQuadTree { public: int Id; void SaveToFile(CAFile& f) { ++f.Saves; f.LastId = Id; } };
class CNavQuadTreeMap { public: int Index; CNavQuadTree* PQuadTree; CNavQuadTreeMap* Next; };
class CNavigatorManager {
public:
    void SaveNavigationMapToFile(int index, CAFile& file) const
    {
        for (const CNavQuadTreeMap* it = Head; it != 0; it = it->Next)
            if (it->Index == index) { it->PQuadTree->SaveToFile(file); return; }
    }
    CNavQuadTreeMap* Head;
};
int main()
{
    CNavQuadTree t1, t2, t3; t1.Id = 1; t2.Id = 2; t3.Id = 3;
    CNavQuadTreeMap m3 = { 5, &t3, 0 }, m2 = { 5, &t2, &m3 }, m1 = { 4, &t1, &m2 };
    CNavigatorManager nm; nm.Head = &m1;
    CAFile f; f.Saves = 0; f.LastId = 0;
    nm.SaveNavigationMapToFile(5, f);
    bool ok = f.Saves == 1 && f.LastId == 2;
    nm.SaveNavigationMapToFile(9, f);
    if (f.Saves != 1) ok = false;
    puts(ok ? "00a14e50_TEST PASS" : "00a14e50_TEST FAIL");
    return ok ? 0 : 1;
}