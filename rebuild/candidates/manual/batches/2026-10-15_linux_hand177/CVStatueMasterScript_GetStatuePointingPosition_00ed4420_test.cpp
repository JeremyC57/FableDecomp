#include <cstdio>
// Behaviour model of CV_StatueMasterScript::GetStatuePointingPosition: HHMM -> day fraction, fractional part matched
// against AnglesToFaceList[1..3] within +-0.04.
struct Statue {
    long time;
    float AnglesToFaceList[4];
    float GetTimeOfDayFraction()
    {
        long hours = time / 100;
        long minutes = time - hours * 100;
        return ((1.0f / 60.0f) * (float)minutes + (float)hours) * (1.0f / 24.0f);
    }
    long GetStatuePointingPosition()
    {
        float day = GetTimeOfDayFraction();
        float frac = day - (float)(long)day;
        for (long i = 1; i < 4; ++i)
            if (AnglesToFaceList[i] - 0.04f < frac && AnglesToFaceList[i] + 0.04f > frac)
                return i;
        return 0;
    }
};
int main()
{
    Statue s;
    s.AnglesToFaceList[0] = 0.0f; s.AnglesToFaceList[1] = 0.25f; s.AnglesToFaceList[2] = 0.5f; s.AnglesToFaceList[3] = 0.75f;
    s.time = 600;  long r1 = s.GetStatuePointingPosition();   // 0.25
    s.time = 1200; long r2 = s.GetStatuePointingPosition();   // 0.5
    s.time = 1830; long r3 = s.GetStatuePointingPosition();   // 0.7708
    s.time = 300;  long r4 = s.GetStatuePointingPosition();   // 0.125
    if (r1 != 1 || r2 != 2 || r3 != 3 || r4 != 0) { std::printf("00ed4420_TEST FAIL\n"); return 1; }
    std::printf("00ed4420_TEST PASS\n");
    return 0;
}
