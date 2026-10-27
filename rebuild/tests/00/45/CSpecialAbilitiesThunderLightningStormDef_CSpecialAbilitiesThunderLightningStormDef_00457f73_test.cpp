// behaviour test for 0x00457F73 CSpecialAbilitiesThunderLightningStormDef::CSpecialAbilitiesThunderLightningStormDef() (model: the CDefString members start unset)
#include <cstdio>
#include <cstring>
#include <new>
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } };
struct Def { int NumBeams; float StartRadius; float EndRadius; float SkyBeamStartTime; float MainBeamsStartTime; float BeamRotationStartSpeed; float BeamRotationEndSpeed; float TotalLifetime; float Damage; CDefString InitSound; CDefString LoopingSound; };
int main()
{
    static double raw[32]; std::memset(raw, 0x11, sizeof(raw));
    Def* d = new (raw) Def;
    int ok = 1;
    if (d->InitSound.TablePos != -1) ok = 0;
    if (d->LoopingSound.TablePos != -1) ok = 0;
    if (!ok) { std::printf("00457f73_TEST FAIL\n"); return 1; }
    std::printf("00457f73_TEST PASS\n");
    return 0;
}