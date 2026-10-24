#include <cstdio>
#include <new>
struct MirroredCriminalStatus {
    long  Crimes[10];
    long  LastTimeSeenByGuard;
    long  LastTimeExited;
    bool  ExitedUsingTeleporter;
    float EntrancePosX;
    float EntrancePosY;
    float EntrancePosZ;
    float EntranceAngle;
    MirroredCriminalStatus()
    {
        for (long i = 0; i < 10; ++i)
            Crimes[i] = 0;
        EntrancePosX = 0.0f;
        EntrancePosY = 0.0f;
        EntrancePosZ = 0.0f;
        LastTimeSeenByGuard = 0;
        ExitedUsingTeleporter = false;
        LastTimeExited = 0;
        EntranceAngle = 0.0f;
    }
};
int main()
{
    unsigned char raw[sizeof(MirroredCriminalStatus)];
    long k = 0;
    for (k = 0; k < (long)sizeof(raw); ++k) raw[k] = 0xAB;
    MirroredCriminalStatus* s = new (raw) MirroredCriminalStatus;
    int bad = 0;
    for (k = 0; k < 10; ++k) if (s->Crimes[k] != 0) bad = 1;
    if (s->LastTimeSeenByGuard != 0 || s->LastTimeExited != 0 || s->ExitedUsingTeleporter) bad = 1;
    if (s->EntrancePosX != 0.0f || s->EntrancePosY != 0.0f || s->EntrancePosZ != 0.0f || s->EntranceAngle != 0.0f) bad = 1;
    if (bad) { std::printf("0057a5dd_TEST FAIL\n"); return 1; }
    std::printf("0057a5dd_TEST PASS\n");
    return 0;
}