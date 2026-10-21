#include <cstdio>
#include "engine/CFadeInFadeOutBase.h"
struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    void Update(float timeDelta);
};
void CFadeInFadeOutBase_Methods::Update(float timeDelta)
{
    if (Opening) {
        OpenTimer += timeDelta;
        if (OpenTimer >= FadeInTime) Opening = false;
    } else if (Closing) {
        CloseTimer -= timeDelta;
        if (CloseTimer <= 0.0f) {
            if (CloseTimer < 0.0f) CloseTimer = 0.0f;
            Active = false;
            Closing = false;
            Opening = false;
        }
    }
}

static int n_fail;
static void check(bool ok, int line) { if (!ok) { ++n_fail; printf("FAIL line %d\n", line); } }
int main()
{
    CFadeInFadeOutBase_Methods f;
    f.Active = true; f.FadeInTime = 1.0f; f.FadeOutTime = 1.0f; f.Opening = true; f.Closing = true;
    f.OpenTimer = 0.0f; f.CloseTimer = 0.5f;
    f.Update(0.5f);
    check(f.Opening && f.OpenTimer == 0.5f && f.CloseTimer == 0.5f, __LINE__);
    f.Update(0.5f);
    check(!f.Opening && f.OpenTimer == 1.0f && f.Closing && f.Active, __LINE__);
    f.Update(0.25f);
    check(f.CloseTimer == 0.25f && f.Closing && f.Active, __LINE__);
    f.Update(1.0f);
    check(f.CloseTimer == 0.0f && !f.Closing && !f.Active && !f.Opening, __LINE__);
    f.Active = true; f.Closing = true; f.CloseTimer = 0.5f;
    f.Update(0.5f);
    check(f.CloseTimer == 0.0f && !f.Closing && !f.Active, __LINE__);
    f.Active = true; f.Closing = false; f.Opening = false; f.CloseTimer = 3.0f; f.OpenTimer = 2.0f;
    f.Update(1.0f);
    check(f.Active && f.CloseTimer == 3.0f && f.OpenTimer == 2.0f, __LINE__);
    if (!n_fail) printf("00434870_TEST PASS\n");
    return 0;
}