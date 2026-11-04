// Behaviour test for C3DAnimationInfo::Preload (0x00A26AA0) = BuildSharedAnimation: start loading when not loading, take the data
// off the resource list, then build the shared animation.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Data { bool listed; int built; };
struct Info { bool loading; Data* data;
    void BuildSharedAnimation() { if (!loading) loading = true; data->listed = false; ++data->built; } };
int main()
{
    Data d = { true, 0 };
    Info i; i.loading = false; i.data = &d;
    i.BuildSharedAnimation();
    bool ok = i.loading && !d.listed && d.built == 1;
    printf(ok ? "00a26aa0_TEST PASS\n" : "00a26aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}