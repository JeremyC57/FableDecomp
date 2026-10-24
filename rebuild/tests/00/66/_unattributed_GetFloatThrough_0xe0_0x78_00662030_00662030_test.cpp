#include <cstdio>
struct CTarget { unsigned char _pad[0x78]; float value; };
struct _unattributed { unsigned char _pad[0xe0]; CTarget* target; float GetFloatThrough_0xe0_0x78_00662030() { return target->value; } };
int main() { CTarget t; t.value = 2.5f; _unattributed o; o.target = &t; bool ok = o.GetFloatThrough_0xe0_0x78_00662030() == 2.5f; t.value = -1.0f; ok = ok && o.GetFloatThrough_0xe0_0x78_00662030() == -1.0f; std::printf(ok ? "00662030_TEST PASS\n" : "00662030_TEST FAIL\n"); return ok ? 0 : 1; }
