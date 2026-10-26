// Behaviour test for 0x0061A3B0 CTCInventoryQuests::GetCategoryNameFromAssociation
#include <cstdio>
#include <cstring>
class CWideString { public: char buf[32]; };
class CTCQuestCard { public: const char* name; CWideString GetQuestName() const { CWideString w; std::strcpy(w.buf, name); return w; } };
class CThing { public:
    unsigned int m_presentBits[9]; CTCQuestCard* card; unsigned char Dead;
    bool GetInterface(CTCQuestCard** out, long t)
    {
        if (m_presentBits[t >> 5] & (1u << (t & 31))) { *out = card; return true; }
        return false;
    }
};
static CWideString GetCategoryNameFromAssociation(CThing* thing)
{
    CTCQuestCard* questCard;
    if (thing && !thing->Dead && thing->GetInterface(&questCard, 0x6C))
        return questCard->GetQuestName();
    CWideString empty; empty.buf[0] = 0; return empty;
}
int main()
{
    CTCQuestCard card = { "Arena" }; CThing t = { { 0 }, &card, 0 };
    bool ok = GetCategoryNameFromAssociation(&t).buf[0] == 0;
    t.m_presentBits[3] = 0x1000;
    ok = ok && std::strcmp(GetCategoryNameFromAssociation(&t).buf, "Arena") == 0;
    t.Dead = 1; ok = ok && GetCategoryNameFromAssociation(&t).buf[0] == 0 && GetCategoryNameFromAssociation(0).buf[0] == 0;
    if (!ok) { std::printf("0061a3b0_TEST FAIL\n"); return 1; }
    std::printf("0061a3b0_TEST PASS\n");
    return 0;
}