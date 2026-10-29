// Behaviour test for CTCQuestCompletionUI::Copy(const CTCBase&) @ 0x005E49E0: the three start-of-quest maps (renown, gold,
// experience) are emptied and refilled key by key from the source TC's maps, then CTCBase::Copy copies the base state.
#include <stdio.h>
static int g_baseCopies = 0; static const void* g_baseCopySource = 0;
class LongULongMap { public:                                // minimal ordered map<long, unsigned long>
    struct entry { int first; unsigned int second; };
    entry items[16]; int count;
    LongULongMap() : count(0) { }
    typedef const entry* const_iterator;
    const_iterator begin() const { return items; }
    const_iterator end() const { return items + count; }
    void clear() { count = 0; }
    unsigned int& operator[](int key)
    {
        int i = 0;
        while (i < count && items[i].first < key) ++i;
        if (i < count && items[i].first == key) return items[i].second;
        for (int j = count; j > i; --j) items[j] = items[j - 1];
        ++count; items[i].first = key; items[i].second = 0;
        return items[i].second;
    }
};
class CTCBase { public:
    int ParentThing;
    virtual void Copy(const CTCBase& other) { g_baseCopies++; g_baseCopySource = &other; ParentThing = other.ParentThing; }
    virtual ~CTCBase() { }
};
class CTCQuestCompletionUI : public CTCBase { public:
    LongULongMap StartQuestExperience;
    LongULongMap StartQuestGold;
    LongULongMap StartQuestRenown;
    virtual void Copy(const CTCBase& other);
};
void CTCQuestCompletionUI::Copy(const CTCBase& other)
{
    const CTCQuestCompletionUI& source = (const CTCQuestCompletionUI&)other;
    StartQuestRenown.clear();
    StartQuestGold.clear();
    StartQuestExperience.clear();
    LongULongMap::const_iterator it;
    for (it = source.StartQuestRenown.begin(); it != source.StartQuestRenown.end(); ++it)
        StartQuestRenown[it->first] = it->second;
    for (it = source.StartQuestGold.begin(); it != source.StartQuestGold.end(); ++it)
        StartQuestGold[it->first] = it->second;
    for (it = source.StartQuestExperience.begin(); it != source.StartQuestExperience.end(); ++it)
        StartQuestExperience[it->first] = it->second;
    CTCBase::Copy(other);
}
static int fail(int line) { printf("005e49e0_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    CTCQuestCompletionUI src, dst;
    src.ParentThing = 77; dst.ParentThing = 1;
    src.StartQuestRenown[3] = 30; src.StartQuestRenown[1] = 10;
    src.StartQuestGold[5] = 500;
    src.StartQuestExperience[2] = 20; src.StartQuestExperience[4] = 40; src.StartQuestExperience[6] = 60;
    dst.StartQuestRenown[9] = 99; dst.StartQuestGold[8] = 88; dst.StartQuestExperience[7] = 77;
    CTCBase& asBase = dst;
    asBase.Copy(src);
    if (dst.StartQuestRenown.count != 2 || dst.StartQuestRenown[1] != 10 || dst.StartQuestRenown[3] != 30) return fail(__LINE__);
    if (dst.StartQuestGold.count != 1 || dst.StartQuestGold[5] != 500) return fail(__LINE__);
    if (dst.StartQuestExperience.count != 3 || dst.StartQuestExperience[2] != 20 || dst.StartQuestExperience[6] != 60)
        return fail(__LINE__);
    if (g_baseCopies != 1 || g_baseCopySource != &src || dst.ParentThing != 77) return fail(__LINE__);
    CTCQuestCompletionUI empty;
    dst.Copy(empty);
    if (dst.StartQuestRenown.count != 0 || dst.StartQuestGold.count != 0 || dst.StartQuestExperience.count != 0 || g_baseCopies != 2)
        return fail(__LINE__);
    printf("005e49e0_TEST PASS\n");
    return 0;
}