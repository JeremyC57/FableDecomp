// behaviour test for 0x0056E5CA
#include <stdio.h>
class CTCInventoryExperience {
public:
    unsigned char _pad00[0x1bc];
    long          statIndicatorPos;                      // +0x1bc
    long          statIndicatorMax;                      // +0x1c0
    void IncrementStatIndicator();
};

void CTCInventoryExperience::IncrementStatIndicator()
{
    if (statIndicatorPos + 1 < statIndicatorMax)
        ++statIndicatorPos;
}
int main()
{
    CTCInventoryExperience e; e.statIndicatorPos = 0; e.statIndicatorMax = 3; int ok = 1;
    e.IncrementStatIndicator(); ok &= e.statIndicatorPos == 1;
    e.IncrementStatIndicator(); ok &= e.statIndicatorPos == 2;
    e.IncrementStatIndicator(); ok &= e.statIndicatorPos == 2;
    printf(ok ? "0056e5ca_TEST PASS\n" : "0056e5ca_TEST FAIL\n");
    return ok ? 0 : 1;
}