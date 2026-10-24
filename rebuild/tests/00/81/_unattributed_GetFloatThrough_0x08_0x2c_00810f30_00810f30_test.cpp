#include <cstdio>
struct CTarget { unsigned char _pad[0x2c]; float value; };
struct _unattributed { unsigned char _pad[0x08]; CTarget* target; float GetFloatThrough_0x08_0x2c_00810f30() { return target->value; } };
int main() { CTarget t; t.value = 2.5f; _unattributed o; o.target = &t; bool ok = o.GetFloatThrough_0x08_0x2c_00810f30() == 2.5f; t.value = -1.0f; ok = ok && o.GetFloatThrough_0x08_0x2c_00810f30() == -1.0f; std::printf(ok ? "00810f30_TEST PASS\n" : "00810f30_TEST FAIL\n"); return ok ? 0 : 1; }
