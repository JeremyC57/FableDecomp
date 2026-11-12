// Behaviour test for CAIStateGroup_Gaze::FindFreeParticularGazePos (0x00918640): a copy of the model's logic on plain
// structs -- the owner-list branch (alive, gaze type, not exclusively used, optional inside test), the village branch,
// the early `return 0` paths, and the GFRandom pick (seed*0x24A1+0x24DF, ror 13, % count) from a std::vector.
#include <stdio.h>
#include <stddef.h>
#include <vector>
// The row's class through the shared header (FABLE_RETAIL_CAISTATEGROUP_GAZE): layout checked against Ego_r.pdb.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_GAZE
#include "engine/CAIStateGroup_Gaze.h"

struct Thing { int alive; unsigned long defIndex; int exclusive; int inside; };
static unsigned long Ror13(unsigned long v) { return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = (*seed * 0x24A1 + 0x24DF) & 0xFFFFFFFFUL;
    *seed = Ror13(*seed);
    return range ? *seed % range : 0;
}
struct Model {
    int hasBuilding, hasOwnerTC, hasVillageTC, hasVillage;
    std::vector<Thing*> ownerList, villageList;
    unsigned long seed;
    Thing* Find(long gazeType, bool inside)
    {
        std::vector<Thing*> found;
        if (hasBuilding) {
            if (hasOwnerTC) {
                for (size_t i = 0; i < ownerList.size(); ++i) {
                    Thing* t = ownerList[i];
                    if (t != 0 && t->alive && t->defIndex == (unsigned long)gazeType && !t->exclusive && (!inside || t->inside))
                        found.push_back(t);
                }
            } else
                return 0;
        } else {
            if (!hasVillageTC) return 0;
            if (!hasVillage) return 0;
            for (size_t i = 0; i < villageList.size(); ++i) {
                Thing* t = villageList[i];
                if (t != 0 && t->alive && t->defIndex == (unsigned long)gazeType && !t->exclusive)
                    found.push_back(t);
            }
        }
        if (found.size() > 0)
            return found[GFRandom(found.size(), &seed)];
        return 0;
    }
};

int main()
{
    Thing a = {1, 5, 0, 1}, b = {1, 5, 0, 0}, c = {0, 5, 0, 1}, d = {1, 6, 0, 1}, e = {1, 5, 1, 1};
    Model m; m.hasBuilding = 1; m.hasOwnerTC = 1; m.hasVillageTC = 1; m.hasVillage = 1; m.seed = 1;
    m.ownerList.push_back(&a); m.ownerList.push_back(&b); m.ownerList.push_back(&c);
    m.ownerList.push_back(&d); m.ownerList.push_back(&e); m.ownerList.push_back(0);
    int ok = 1;
    if (sizeof(CAIStateGroup_Gaze) != 0x30 || offsetof(CAIStateGroup_Gaze, GazeMarker) != 0x14
        || offsetof(CAIStateGroup_Gaze, ChanceOfRunning) != 0x1c || offsetof(CAIStateGroup_Gaze, PrimaryMarkerDef) != 0x28
        || offsetof(CAIStateGroup_Gaze, SecondaryMarkerDef) != 0x2c) ok = 0;
    // inside: only a qualifies, seed still advances
    if (m.Find(5, true) != &a) ok = 0;
    unsigned long s1 = Ror13((1 * 0x24A1 + 0x24DF) & 0xFFFFFFFFUL);
    if (m.seed != s1) ok = 0;
    // not inside: a and b qualify; expected pick from the LCG
    unsigned long s2 = Ror13((s1 * 0x24A1 + 0x24DF) & 0xFFFFFFFFUL);
    Thing* exp = (s2 % 2) ? &b : &a;
    if (m.Find(5, false) != exp) ok = 0;
    // no match: 0 and the seed untouched
    unsigned long before = m.seed;
    if (m.Find(9, false) != 0 || m.seed != before) ok = 0;
    // owner TC missing: 0
    m.hasOwnerTC = 0; if (m.Find(5, false) != 0) ok = 0;
    // village branch
    m.hasBuilding = 0; m.villageList.push_back(&d); m.villageList.push_back(&e);
    if (m.Find(6, false) != &d) ok = 0;
    m.hasVillage = 0; if (m.Find(6, false) != 0) ok = 0;
    m.hasVillage = 1; m.hasVillageTC = 0; if (m.Find(6, false) != 0) ok = 0;
    // deterministic filter cases: the rejected marker is the ONLY one of its gaze type -> 0, seed untouched
    {
        Thing dead = {0, 20, 0, 1}, excl = {1, 21, 1, 1}, outside = {1, 22, 0, 0};
        Thing vdead = {0, 30, 0, 1}, vexcl = {1, 31, 1, 1}, vout = {1, 32, 0, 0};
        Model f; f.hasBuilding = 1; f.hasOwnerTC = 1; f.hasVillageTC = 1; f.hasVillage = 1; f.seed = 7;
        f.ownerList.push_back(&dead); f.ownerList.push_back(&excl); f.ownerList.push_back(&outside);
        f.villageList.push_back(&vdead); f.villageList.push_back(&vexcl); f.villageList.push_back(&vout);
        if (f.Find(20, false) != 0 || f.seed != 7) ok = 0;          // dead only
        if (f.Find(21, false) != 0 || f.seed != 7) ok = 0;          // exclusively used only
        if (f.Find(22, true) != 0 || f.seed != 7) ok = 0;           // outside only, inside required
        if (f.Find(22, false) != &outside) ok = 0;                  // outside accepted without the inside test
        f.hasBuilding = 0;
        if (f.Find(30, false) != 0) ok = 0;                         // village: dead only
        if (f.Find(31, false) != 0) ok = 0;                         // village: exclusively used only
        if (f.Find(32, false) != &vout) ok = 0;                     // village ignores the inside flag
        if (f.Find(32, true) != &vout) ok = 0;
    }
    if (ok) { printf("00918640_TEST PASS\n"); return 0; }
    printf("00918640_TEST FAIL\n"); return 1;
}