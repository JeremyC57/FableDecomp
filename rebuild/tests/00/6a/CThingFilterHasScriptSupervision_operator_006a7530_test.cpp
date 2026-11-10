// Behaviour test for 0x006A7530 (filter: thing's IsUnderScriptSupervision() == wanted). Self-contained: CThing is a stub.
#include <cstdio>
static int g_ret = 0;
class CThing { public: bool IsUnderScriptSupervision() const { return g_ret != 0; } };
class CThingFilter_HasScriptSupervision { public: bool m_wanted; };
static bool Op(CThingFilter_HasScriptSupervision* filter, CThing* thing) { return thing->IsUnderScriptSupervision() == filter->m_wanted; }
int main(){
    CThing t; CThingFilter_HasScriptSupervision f;
    f.m_wanted = true;  g_ret = 1; bool a = Op(&f,&t);
    f.m_wanted = true;  g_ret = 0; bool b = Op(&f,&t);
    f.m_wanted = false; g_ret = 0; bool c = Op(&f,&t);
    f.m_wanted = false; g_ret = 1; bool d = Op(&f,&t);
    if (a && !b && c && !d) { std::printf("006a7530_TEST PASS\n"); return 0; }
    std::printf("006a7530_TEST FAIL\n"); return 1;
}
