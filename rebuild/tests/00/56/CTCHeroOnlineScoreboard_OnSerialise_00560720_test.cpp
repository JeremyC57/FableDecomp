// Behaviour test for CTCHeroOnlineScoreboard::OnSerialise (0x00560720): saving writes the map as two parallel vectors
// in key order, loading writes each (activity, score) pair back into the map (overwriting existing scores, keeping
// unrelated entries) -- through a copy of both branches over a stand-in persist context that stores the vectors.
#include <cstdio>
#include <map>
#include <vector>

enum Activity { ACTIVITY_A = 1, ACTIVITY_B = 4, ACTIVITY_C = 9 };

struct Context {
    int Mode;
    std::vector<Activity> savedActivities;
    std::vector<float> savedScores;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char*, std::vector<Activity>& v) { if (IsSaving()) savedActivities = v; else v = savedActivities; }
    void Transfer(const char*, std::vector<float>& v) { if (IsSaving()) savedScores = v; else v = savedScores; }
};

static void Serialise(std::map<Activity, float>& board, Context& context)
{
    if (context.IsSaving()) {
        std::vector<Activity> activities;
        std::vector<float> scores;
        for (std::map<Activity, float>::iterator it = board.begin(); it != board.end(); ++it) {
            float score = (*it).second;
            Activity activity = (*it).first;
            activities.push_back(activity);
            scores.push_back(score);
        }
        context.Transfer("ActivitiesVec", activities);
        context.Transfer("ScoresVec", scores);
    } else {
        std::vector<Activity> activities;
        std::vector<float> scores;
        context.Transfer("ActivitiesVec", activities);
        context.Transfer("ScoresVec", scores);
        std::vector<float>::iterator score = scores.begin();
        for (std::vector<Activity>::iterator activity = activities.begin(); activity != activities.end();
             ++activity, ++score) {
            float value = *score;
            Activity key = *activity;
            board[key] = value;
        }
    }
}

int main()
{
    std::map<Activity, float> board;
    board[ACTIVITY_C] = 3.5f;
    board[ACTIVITY_A] = 12.25f;
    Context context;
    context.Mode = 3;
    Serialise(board, context);
    if (context.savedActivities.size() != 2 || context.savedActivities[0] != ACTIVITY_A ||
        context.savedActivities[1] != ACTIVITY_C || context.savedScores[0] != 12.25f || context.savedScores[1] != 3.5f) {
        std::printf("00560720_TEST FAIL\n");
        return 1;
    }
    std::map<Activity, float> loaded;
    loaded[ACTIVITY_A] = 1.0f;
    loaded[ACTIVITY_B] = 7.0f;
    context.Mode = 0;
    Serialise(loaded, context);
    if (loaded.size() != 3 || loaded[ACTIVITY_A] != 12.25f || loaded[ACTIVITY_B] != 7.0f || loaded[ACTIVITY_C] != 3.5f) {
        std::printf("00560720_TEST FAIL\n");
        return 1;
    }
    std::printf("00560720_TEST PASS\n");
    return 0;
}