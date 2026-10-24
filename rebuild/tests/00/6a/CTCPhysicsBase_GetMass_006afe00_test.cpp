#include <cstdio>
struct CTarget { unsigned char _pad[0x38]; float value; };
struct CTCPhysicsBase { unsigned char _pad[0x38]; CTarget* target; float GetMass() { return target->value; } };
int main() { CTarget t; t.value = 2.5f; CTCPhysicsBase o; o.target = &t; bool ok = o.GetMass() == 2.5f; t.value = -1.0f; ok = ok && o.GetMass() == -1.0f; std::printf(ok ? "006afe00_TEST PASS\n" : "006afe00_TEST FAIL\n"); return ok ? 0 : 1; }
