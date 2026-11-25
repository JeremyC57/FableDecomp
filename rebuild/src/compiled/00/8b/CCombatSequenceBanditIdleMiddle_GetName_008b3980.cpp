// CCombatSequence_BanditIdleMiddle::GetName  (retail 0x008b3980)
// virtual const method returning CCharString by value; body is just
//   return CCharString("literal", -1);
// Literal (NF418, 2026-11-25): was extern data `s_BanditIdleMiddleName` (table ?s_BanditIdleMiddleName@@3QBDB -> 0x01278824); now spelled at the use site:
//   "COMBAT_SEQUENCE_BANDIT_IDLE_MIDDLE", retail's pooled literal at 0x01278824 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).

class CCharString
{
public:
    CCharString(const char* str, long len);
private:
    char* m_pData;
    unsigned long m_Length;
    unsigned long m_Capacity;
};

// retail ctor at 0x0099ebf0: CCharString(CCharString*, char*, long)

class CCombatSequence_BanditIdleMiddle
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditIdleMiddle::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_IDLE_MIDDLE", -1);
}