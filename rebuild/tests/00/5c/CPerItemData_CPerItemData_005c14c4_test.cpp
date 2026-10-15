#include <cstdio>
// CPerItemData copy constructor: the augmentation vector copied (out of line), then the six scalars
static long g_vectorCopies;
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    CLongVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    CLongVector(const CLongVector& other) : m_pFirst(other.m_pFirst), m_pLast(other.m_pLast), m_pEnd(other.m_pEnd) { ++g_vectorCopies; }
};
struct CPerItemData {
    CLongVector Augmentations; long NumberOfKills; long TimeWielded; long BestTrophyWitnessesAheadToDate;
    bool QuestPersistent; long QuestPersistentRefCount; float BestFishWeight;
    CPerItemData() : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(false), QuestPersistentRefCount(0), BestFishWeight(0.0f) { }
    CPerItemData(const CPerItemData& other)
        : Augmentations(other.Augmentations), NumberOfKills(other.NumberOfKills), TimeWielded(other.TimeWielded),
          BestTrophyWitnessesAheadToDate(other.BestTrophyWitnessesAheadToDate), QuestPersistent(other.QuestPersistent),
          QuestPersistentRefCount(other.QuestPersistentRefCount), BestFishWeight(other.BestFishWeight) { }
};
int main()
{
    long store[3] = { 1, 2, 3 };
    CPerItemData a; a.Augmentations.m_pFirst = store; a.Augmentations.m_pLast = store + 3; a.Augmentations.m_pEnd = store + 3;
    a.NumberOfKills = 12; a.TimeWielded = 3400; a.BestTrophyWitnessesAheadToDate = 5; a.QuestPersistent = true; a.QuestPersistentRefCount = 2; a.BestFishWeight = 7.25f;
    CPerItemData b(a);
    bool ok = g_vectorCopies == 1 && b.Augmentations.m_pLast == store + 3 && b.NumberOfKills == 12 && b.TimeWielded == 3400
        && b.BestTrophyWitnessesAheadToDate == 5 && b.QuestPersistent && b.QuestPersistentRefCount == 2 && b.BestFishWeight == 7.25f;
    if (!ok) { std::printf("005c14c4_TEST FAIL\n"); return 1; }
    std::printf("005c14c4_TEST PASS\n");
    return 0;
}