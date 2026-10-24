#include <cstdio>
struct CTarget { unsigned char _pad[0x44]; float value; };
struct _unattributed { unsigned char _pad[0x14]; CTarget* target; float GetFloatThrough_0x14_0x44_00751b70() { return target->value; } };
int main() { CTarget t; t.value = 2.5f; _unattributed o; o.target = &t; bool ok = o.GetFloatThrough_0x14_0x44_00751b70() == 2.5f; t.value = -1.0f; ok = ok && o.GetFloatThrough_0x14_0x44_00751b70() == -1.0f; std::printf(ok ? "00751b70_TEST PASS\n" : "00751b70_TEST FAIL\n"); return ok ? 0 : 1; }
