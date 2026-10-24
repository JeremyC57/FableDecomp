#include <cstdio>
struct CTarget { unsigned char _pad[0xf4]; float value; };
struct _unattributed { unsigned char _pad[0x58]; CTarget* target; float GetFloatThrough_0x58_0xf4_006d6530() { return target->value; } };
int main() { CTarget t; t.value = 2.5f; _unattributed o; o.target = &t; bool ok = o.GetFloatThrough_0x58_0xf4_006d6530() == 2.5f; t.value = -1.0f; ok = ok && o.GetFloatThrough_0x58_0xf4_006d6530() == -1.0f; std::printf(ok ? "006d6530_TEST PASS\n" : "006d6530_TEST FAIL\n"); return ok ? 0 : 1; }
