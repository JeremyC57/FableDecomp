#include <cstdio>
// Behaviour test for 0x00662010: CThingCreatureBase::GetProperty() const reads the creature definition's Property word
// (the def pointer at +0xE0, Property at +0xF0 of the def).
struct Def { char pad[0xF0]; unsigned long Property; };
struct Creature { char pad[0xE0]; Def* PDef; unsigned long GetProperty() const { return PDef->Property; } };
int main() {
 Def d; d.Property = 3; Creature c; c.PDef = &d;
 if (c.GetProperty() != 3) { std::printf("00662010_TEST FAIL\n"); return 1; }
 d.Property = 1234; if (c.GetProperty() != 1234) { std::printf("00662010_TEST FAIL\n"); return 1; }
 std::printf("00662010_TEST PASS\n"); return 0;
}
