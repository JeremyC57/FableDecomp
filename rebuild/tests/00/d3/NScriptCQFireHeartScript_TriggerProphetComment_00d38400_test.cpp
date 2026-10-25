// Behaviour test for CQ_FireHeartScript::TriggerProphetComment (0x00D38400): build the comment key
// "TEXT_QST_B01_PROPHET" + (freed + killed + 1) + "_" + comment and flag that prophet as the next to speak.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    char text[96];
    CCharString() { text[0] = 0; }
    CCharString(const char* s) { std::strcpy(text, s); }
};
CCharString GFIntToCharString(long value)
{
    CCharString r;
    std::sprintf(r.text, "%ld", value);
    return r;
}
CCharString operator+(const char* left, const CCharString& right) { CCharString r(left); std::strcat(r.text, right.text); return r; }
CCharString operator+(const CCharString& left, const char* right) { CCharString r(left.text); std::strcat(r.text, right); return r; }
CCharString operator+(const CCharString& left, const CCharString& right) { CCharString r(left.text); std::strcat(r.text, right.text); return r; }

struct CQ_FireHeartScript {
    long FreedProphets;
    long KilledProphets;
    bool ShouldProphetSpeak[8];
    CCharString ProphetComment;
    void TriggerProphetComment(const CCharString& comment)
    {
        ProphetComment = "TEXT_QST_B01_PROPHET" + GFIntToCharString(FreedProphets + KilledProphets + 1) + "_" + comment;
        ShouldProphetSpeak[FreedProphets + KilledProphets] = true;
    }
};

int main()
{
    CQ_FireHeartScript s;
    s.FreedProphets = 1;
    s.KilledProphets = 1;
    for (int i = 0; i < 8; ++i) s.ShouldProphetSpeak[i] = false;
    s.TriggerProphetComment(CCharString("abc"));
    bool ok = std::strcmp(s.ProphetComment.text, "TEXT_QST_B01_PROPHET3_abc") == 0 && s.ShouldProphetSpeak[2]
        && !s.ShouldProphetSpeak[0] && !s.ShouldProphetSpeak[1] && !s.ShouldProphetSpeak[3];
    s.FreedProphets = 0; s.KilledProphets = 0;
    s.TriggerProphetComment(CCharString("FREED"));
    ok = ok && std::strcmp(s.ProphetComment.text, "TEXT_QST_B01_PROPHET1_FREED") == 0 && s.ShouldProphetSpeak[0];
    if (!ok) {
        std::printf("00d38400_TEST FAIL\n");
        return 1;
    }
    std::printf("00d38400_TEST PASS\n");
    return 0;
}