#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

static const char* g_name; static float* g_value; static float g_default = -1;
class CPersistContext { public: void Transfer(const char* n, float& v, const float& d) { g_name = n; g_value = &v; g_default = d; v = 3.5f; } };
class CDrunkennessDef { public: virtual void Transfer(CPersistContext& c); unsigned char p[0x24]; float DrunkennessThresholdMult; };
void CDrunkennessDef::Transfer(CPersistContext& context) { context.Transfer("DrunkennessThresholdMult", DrunkennessThresholdMult, 0.0f); }
int main() { CDrunkennessDef d; d.DrunkennessThresholdMult = 1.0f; CPersistContext c; d.Transfer(c);
    if (std::strcmp(g_name, "DrunkennessThresholdMult") || g_value != &d.DrunkennessThresholdMult || g_default != 0.0f || d.DrunkennessThresholdMult != 3.5f) { std::printf("004dc96f_TEST FAIL\n"); return 1; }
    std::printf("004dc96f_TEST PASS\n"); return 0; }