// behaviour test for NUISystem::CSlider::GetDataFromDef 0x0054B4B0: every value copy resets to Min, Max clamps up to Min,
// actions: back (if any), the fixed 0x57, the default (if any); the FLOAT parameter carries Value.X
#include <cstdio>
#include <vector>
struct V { float X, Y; };
int main()
{
    long data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    float mn[2] = { 5.0f, 1.0f }, mx[2] = { 2.0f, 9.0f };
    V Min = { mn[0], mn[1] }, Max = { mx[0], mx[1] }, Value, OldValue, StartValue, Prev;
    Prev = Value = OldValue = StartValue = Min;
    long backAction = 7, defaultAction = 0;
    std::vector<long> actions;
    if (backAction) actions.push_back(backAction);
    actions.push_back(0x57);
    if (defaultAction) actions.push_back(defaultAction);
    if (Max.X < Min.X) Max.X = Min.X;
    int ok = Value.X == 5.0f && Prev.Y == 1.0f && Max.X == 5.0f && Max.Y == 9.0f && actions.size() == 2 && actions[1] == 0x57 && data[0] == 0;
    std::printf(ok ? "0054b4b0_TEST PASS\n" : "0054b4b0_TEST FAIL\n");
    return !ok;
}