// behaviour test for 0x0041CE9D
#include <stdio.h>
namespace NUISystem {
class CErrorMessage {
public:
    float total;
    CErrorMessage() : total(0) {}
    virtual ~CErrorMessage() {}
    virtual void Update(float time) { total += time; }
};
class CManager {
public:
    unsigned char  _pad00[0xa0];
    CErrorMessage* ErrorMessage;                         // +0xa0
    void UpdateErrorMessage(float time);
};

void CManager::UpdateErrorMessage(float time)
{
    ErrorMessage->Update(time);
}
}
using namespace NUISystem;
int main()
{
    CErrorMessage e; CManager m; m.ErrorMessage = &e; int ok = 1;
    m.UpdateErrorMessage(0.5f); m.UpdateErrorMessage(0.25f);
    ok &= e.total == 0.75f;
    printf(ok ? "0041ce9d_TEST PASS\n" : "0041ce9d_TEST FAIL\n");
    return ok ? 0 : 1;
}