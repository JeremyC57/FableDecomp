#include <cstdio>
#include <cstddef>
// Behaviour test for 0x007EA270 CTCTargeted::SetTargetingFlags: stores the word at +0xC, nothing else.
// Links against the source object.
class CThing;
class CTCBase {
public:
    CTCBase() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
private:
    virtual void Vptr() { }
};
class CTCTargeted : public CTCBase {
public:
    void SetTargetingFlags(unsigned long flags);
    unsigned long TargetingFlags;
    bool          Targetable;
    bool          UseTargetGlow;
    bool          ValidTargetWithoutLOS;
};
int main()
{
    CTCTargeted t;
    if (offsetof(CTCTargeted, TargetingFlags) != 0xc || sizeof(CTCTargeted) != 0x14) { std::printf("007ea270_TEST FAIL layout\n"); return 1; }
    t.ParentThing = 0; t.ToBeRemoved = false; t.TargetingFlags = 0x11; t.Targetable = true;
    t.SetTargetingFlags(0x80000005ul);
    if (t.TargetingFlags != 0x80000005ul || !t.Targetable || t.ParentThing != 0) { std::printf("007ea270_TEST FAIL\n"); return 1; }
    t.SetTargetingFlags(0);
    if (t.TargetingFlags != 0) { std::printf("007ea270_TEST FAIL zero\n"); return 1; }
    std::printf("007ea270_TEST PASS\n");
    return 0;
}
