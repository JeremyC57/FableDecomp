// CTCQuestCompletionUI::IsOverridingScreenFade  @ 0x005e3e70  (__fastcall, this=ecx)
struct CTCQuestCompletionUI {
    char   pad00[0x0c];
    unsigned char Active;
    char   pad0d[0x10-0x0d];
    int    m_10;
    int    m_14;
    char   pad18[0x1c-0x18];
    unsigned char m_1c;
    char   pad1d[0xc0-0x1d];
    unsigned char m_c0;
};

bool __fastcall IsOverridingScreenFade(CTCQuestCompletionUI* completionUI)
{
    if (completionUI->Active != 0 && completionUI->m_1c == 0 && completionUI->m_c0 == 0
        && completionUI->m_10 != completionUI->m_14)
        return true;
    return false;
}