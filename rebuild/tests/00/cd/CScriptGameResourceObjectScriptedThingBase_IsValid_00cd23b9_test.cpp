#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
class CScriptGameResourceObjectScriptedThingBase {
public:
    void* PImp_Data; void* PImp_Info;
    bool IsValid() const { return PImp_Data != 0; }
};
int main()
{
    int ok = 1;
    CScriptGameResourceObjectScriptedThingBase h; h.PImp_Data = 0; h.PImp_Info = &h;
    ok = !h.IsValid();
    h.PImp_Data = &h; ok = ok && h.IsValid();
    if (!ok) { std::printf("00cd23b9_TEST FAIL\n"); return 1; }
    std::printf("00cd23b9_TEST PASS\n");
    return 0;
}