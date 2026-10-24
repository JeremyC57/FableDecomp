#include <cstdio>
struct CTarget { unsigned char _pad[0x12c]; float value; };
struct _unattributed { unsigned char _pad[0x20]; CTarget* target; float GetFloatThrough_0x20_0x12c_0079c740() { return target->value; } };
int main() { CTarget t; t.value = 2.5f; _unattributed o; o.target = &t; bool ok = o.GetFloatThrough_0x20_0x12c_0079c740() == 2.5f; t.value = -1.0f; ok = ok && o.GetFloatThrough_0x20_0x12c_0079c740() == -1.0f; std::printf(ok ? "0079c740_TEST PASS\n" : "0079c740_TEST FAIL\n"); return ok ? 0 : 1; }
