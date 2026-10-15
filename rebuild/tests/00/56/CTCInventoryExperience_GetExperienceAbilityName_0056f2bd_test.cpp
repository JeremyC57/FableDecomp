#include <cstdio>
#include <cstring>
// Behaviour test for 0x0056f2bd: the def is add-ref'd/released around the ExperienceItemLabelAbility lookup; id 0 -> empty string.
struct CWideString {
    char m_text[32];
    CWideString(const char* t) { std::strcpy(m_text, t); }
    CWideString(const CWideString& other) { std::strcpy(m_text, other.m_text); }
    ~CWideString() {}
};
static CWideString g_EmptyWideString("");
static int g_destroyed = 0;
struct SLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
struct CInventoryDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long refCount;
    SLongVector ExperienceItemLabelStat;
    SLongVector ExperienceItemLabelAbility;
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};
struct CInventoryDefPointer {
    CInventoryDef* m_p;
    CInventoryDefPointer(CInventoryDef* p) : m_p(p) {}
    CInventoryDefPointer(const CInventoryDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CInventoryDefPointer() { m_p->Release(); }
    CInventoryDef* operator->() const { return m_p; }
};
static long g_peakRef = 0;
struct CTCInventoryExperience {
    CInventoryDefPointer PDef;
    CTCInventoryExperience(CInventoryDef* d) : PDef(d) {}
    CWideString GetInventoryText(long textId) { char b[32]; std::sprintf(b, "T%ld", textId); return CWideString(b); }
    CWideString GetExperienceAbilityName(long index);
};
CWideString CTCInventoryExperience::GetExperienceAbilityName(long index)
{
    long textId;
    {
        CInventoryDefPointer def(PDef);
        g_peakRef = def->refCount;
        textId = def->ExperienceItemLabelAbility._M_start[index];
    }
    if (textId)
        return GetInventoryText(textId);
    return g_EmptyWideString;
}
int main()
{
    long ids[3] = { 5, 0, 42 };
    long other[3] = { 9, 9, 9 };
    CInventoryDef def; def.refCount = 1;
    def.ExperienceItemLabelAbility._M_start = ids;
    if (def.ExperienceItemLabelStat._M_start != ids) def.ExperienceItemLabelStat._M_start = other; else def.ExperienceItemLabelAbility._M_start = other;
    CTCInventoryExperience e(&def);
    if (std::strcmp(e.GetExperienceAbilityName(2).m_text, "T42") != 0 || g_peakRef != 2 || def.refCount != 1) { std::printf("0056f2bd_TEST FAIL\n"); return 1; }
    if (std::strcmp(e.GetExperienceAbilityName(1).m_text, "") != 0 || def.refCount != 1 || g_destroyed != 0) { std::printf("0056f2bd_TEST FAIL\n"); return 1; }
    if (std::strcmp(e.GetExperienceAbilityName(0).m_text, "T5") != 0) { std::printf("0056f2bd_TEST FAIL\n"); return 1; }
    std::printf("0056f2bd_TEST PASS\n");
    return 0;
}