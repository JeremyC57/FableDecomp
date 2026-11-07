// Behaviour test for CAIStateGroup_DeedReactionRoot::Update (0x00960120), the reaction choice: a flirting deed rejected by an
// unmarried, uncourtable woman uses deed 0x49's tag; otherwise the deed's own tag and animation, falling back for attitudes 13..17
// (to 12, 9, 0, 8, 3) when both are empty; greet deeds (0x36, 0x4d) use the greet dialogue group instead. A model.
#include <cstdio>
long Fallback(long attitude)
{
    switch (attitude) {
    case 13: return 12;
    case 14: return 9;
    case 15: return 0;
    case 16: return 8;
    case 17: return 3;
    }
    return 0;
}
long Tag(long deed, long attitude, bool rejected, const long* tags, const long* rejectTags, bool emptyAnim, long greet)
{
    if (deed == 0x36 || deed == 0x4d) return greet;
    if (rejected) return rejectTags[attitude];
    long tag = tags[attitude];
    if (emptyAnim && tag == 0 && attitude >= 13) tag = tags[Fallback(attitude)];
    return tag;
}
int main()
{
    long tags[18] = { 100, 0, 0, 103, 0, 0, 0, 0, 108, 109, 0, 0, 112, 0, 0, 0, 0, 0 };
    long reject[18] = { 0 };
    reject[13] = 700;
    bool ok = Tag(5, 13, false, tags, reject, true, 0) == 112 && Tag(5, 14, false, tags, reject, true, 0) == 109;
    ok = ok && Tag(5, 15, false, tags, reject, true, 0) == 100 && Tag(5, 17, false, tags, reject, true, 0) == 103;
    ok = ok && Tag(5, 13, false, tags, reject, false, 0) == 0 && Tag(5, 13, true, tags, reject, true, 0) == 700;
    ok = ok && Tag(0x36, 3, false, tags, reject, true, 42) == 42 && Tag(5, 8, false, tags, reject, true, 0) == 108;
    std::printf(ok ? "00960120_TEST PASS\n" : "00960120_TEST FAIL\n");
    return ok ? 0 : 1;
}
