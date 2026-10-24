#include <cstdio>

// Model of 0x0040BB30 CUserProfileManager::ProcessPendingTutorials: when the global switch and the profile's
// tutorial system (new-profile flag when UseNewProfileValues) are on and a category is pending, pop it and give
// it (force = false) unless the profile already had it. The queue is a small array stand-in for std::queue.
enum ETutorialCategory { TUTORIAL_NONE = 0, TUTORIAL_COMBAT = 3, TUTORIAL_WILL = 5, TUTORIAL_TRADE = 9 };
static bool g_tutorialsEnabled = true;
struct CTutorialQueue {
    ETutorialCategory Items[8];
    int Head, Tail;
    CTutorialQueue() : Head(0), Tail(0) { }
    bool empty() const { return Head == Tail; }
    ETutorialCategory& front() { return Items[Head]; }
    void pop() { ++Head; }
    void push(ETutorialCategory c) { Items[Tail++] = c; }
};
class CUserProfileManager {
public:
    virtual ~CUserProfileManager() { }
    bool UseNewProfileValues;
    bool TutorialSystemEnabled;
    CTutorialQueue PendingTutorialCategories;
    bool TutorialSystemEnabledNewProfile;
    ETutorialCategory Given[8];
    int GivenCount;
    bool GiveForced;
    ETutorialCategory AlreadyGiven;
    CUserProfileManager() : UseNewProfileValues(false), TutorialSystemEnabled(true), TutorialSystemEnabledNewProfile(false),
                            GivenCount(0), GiveForced(false), AlreadyGiven(TUTORIAL_WILL) { }
    bool IsTutorialSystemEnabled() const { return UseNewProfileValues ? TutorialSystemEnabledNewProfile : TutorialSystemEnabled; }
    bool IsTutorialCategoryGiven(ETutorialCategory category) const { return category == AlreadyGiven; }
    bool GiveTutorialForCategory(ETutorialCategory category, bool force) { Given[GivenCount++] = category; GiveForced |= force; return true; }
    void ProcessPendingTutorials()
    {
        if (g_tutorialsEnabled && IsTutorialSystemEnabled() && !PendingTutorialCategories.empty()) {
            ETutorialCategory category = PendingTutorialCategories.front();
            PendingTutorialCategories.pop();
            if (!IsTutorialCategoryGiven(category))
                GiveTutorialForCategory(category, false);
        }
    }
};

static int Fail()
{
    std::printf("0040bb30_TEST FAIL\n");
    return 1;
}

int main()
{
    CUserProfileManager profile;
    profile.PendingTutorialCategories.push(TUTORIAL_COMBAT);
    profile.PendingTutorialCategories.push(TUTORIAL_WILL);
    profile.PendingTutorialCategories.push(TUTORIAL_TRADE);
    profile.ProcessPendingTutorials();             // one per call
    if (profile.GivenCount != 1 || profile.Given[0] != TUTORIAL_COMBAT || profile.PendingTutorialCategories.Head != 1)
        return Fail();
    profile.ProcessPendingTutorials();             // WILL already given: popped, not given
    if (profile.GivenCount != 1 || profile.PendingTutorialCategories.Head != 2)
        return Fail();
    g_tutorialsEnabled = false;
    profile.ProcessPendingTutorials();             // global switch off: nothing popped
    if (profile.PendingTutorialCategories.Head != 2)
        return Fail();
    g_tutorialsEnabled = true;
    profile.UseNewProfileValues = true;            // new-profile flag (off) wins
    profile.ProcessPendingTutorials();
    if (profile.PendingTutorialCategories.Head != 2)
        return Fail();
    profile.TutorialSystemEnabledNewProfile = true;
    profile.ProcessPendingTutorials();
    if (profile.GivenCount != 2 || profile.Given[1] != TUTORIAL_TRADE || profile.GiveForced)
        return Fail();
    profile.ProcessPendingTutorials();             // empty queue: nothing happens
    if (profile.GivenCount != 2 || profile.PendingTutorialCategories.Head != 3)
        return Fail();
    std::printf("0040bb30_TEST PASS\n");
    return 0;
}