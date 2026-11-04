// Behaviour test for CAnimatedMeshCreateParams default ctor (0x00C0A790, parked DIFFER row): a model of the row -- no mesh (-1), unit scales, cleared flags, range 20, one instance.
#include <stdio.h>
struct P { long Mesh; float ScaleX, ScaleY, ScaleZ; bool Flag; long Count; float Range; long Instances;
    P() : Mesh(-1), ScaleX(1.0f), ScaleY(1.0f), ScaleZ(1.0f), Flag(false), Count(0), Range(20.0f), Instances(1) {} };
static int Check() { P p; return p.Mesh == -1 && p.ScaleY == 1.0f && !p.Flag && p.Count == 0 && p.Range == 20.0f && p.Instances == 1; }
int main()
{
    int ok = Check();
    printf(ok ? "00c0a790_TEST PASS\n" : "00c0a790_TEST FAIL\n");
    return ok ? 0 : 1;
}
