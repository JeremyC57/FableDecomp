#include <cstdio>
class CThingShotDef { public: unsigned char _pad[0x54]; float HitThingDamage; };
struct CTCShotDefinitionThrow { unsigned char _pad[0x38]; CThingShotDef* PShotDef; unsigned char _pad2[8]; float DamageMultiplier; float GetShotDamage() { return DamageMultiplier * PShotDef->HitThingDamage; } };
int main() { CThingShotDef d; d.HitThingDamage = 10.0f; CTCShotDefinitionThrow s; s.PShotDef = &d; s.DamageMultiplier = 1.5f; bool ok = s.GetShotDamage() == 15.0f; s.DamageMultiplier = 0.0f; ok = ok && s.GetShotDamage() == 0.0f; std::printf(ok ? "007bee10_TEST PASS\n" : "007bee10_TEST FAIL\n"); return ok ? 0 : 1; }
