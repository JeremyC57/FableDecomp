#include <cstdio>
// Behaviour model of CScriptConversation::OnCleanup @ 0x006E5690 (copy of the source's logic, stand-in callees).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> bool GetInterface(T** ppOut, long t)
    {
        if (HasTC(t)) {
            long type = t; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || t < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
    template<class T> T* GetTC(long t)
    {
        long type = t; CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; }
    return false;
}
struct CRetailPointerVector {
    CBaseIntelligentPointer* m_pFirst; CBaseIntelligentPointer* m_pLast;
    CBaseIntelligentPointer* begin() const { return m_pFirst; }
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    CBaseIntelligentPointer& operator[](unsigned long i) const { return *(begin() + i); }
};
struct CTCTargetingBase { int cleared; CThing* target; void SetPScriptOverrideInterestingThing(CThing* t) { target = t; ++cleared; } };
struct CTCTalk { int stopped; void StopCurrentSpeech() { ++stopped; } };
struct CScriptConversation {
    CRetailPointerVector PeopleInConversation;
    void OnCleanup(bool stopSpeech)
    {
        for (unsigned long i = 0; i < PeopleInConversation.size(); ++i) {
            if (IS_THING_ALIVE(PeopleInConversation[i])) {
                {
                    CTCTargetingBase* targeting;
                    if (PeopleInConversation[i].GetPItem()->GetInterface(&targeting, 8))
                        targeting->SetPScriptOverrideInterestingThing(0);
                }
                if (stopSpeech) {
                    CThing* thing = PeopleInConversation[i].GetPItem();
                    if (thing->HasTC(0xf))
                        thing->GetTC<CTCTalk>(0xf)->StopCurrentSpeech();
                }
            }
        }
    }
};
int main()
{
    CTCTargetingBase tg = { 0, (CThing*)&tg }; CTCTalk talk = { 0 };
    CInterfaceMap::SEntry both[2] = { { 8, (CTCBase*)&tg }, { 0xf, (CTCBase*)&talk } };
    CThing a = { { (1UL << 8) | (1UL << 15) }, { both, both + 2 }, 0 };
    CThing dead = a; dead.m_flags91 = 1;
    CThing none = { { 0 }, { both, both }, 0 };
    CBaseIntelligentPointer people[4] = { { &a }, { &dead }, { 0 }, { &none } };
    CScriptConversation c; c.PeopleInConversation.m_pFirst = people; c.PeopleInConversation.m_pLast = people + 4;
    c.OnCleanup(false);
    int ok = tg.cleared == 1 && tg.target == 0 && talk.stopped == 0;
    c.OnCleanup(true);
    ok = ok && tg.cleared == 2 && talk.stopped == 1;
    if (!ok) { std::printf("006e5690_TEST FAIL\n"); return 1; }
    std::printf("006e5690_TEST PASS\n");
    return 0;
}