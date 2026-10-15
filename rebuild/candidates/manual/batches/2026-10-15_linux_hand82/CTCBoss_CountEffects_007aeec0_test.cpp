// Behaviour test for 007aeec0 CTCBoss::CountLivingHelperCreatures: counts the entries of HelperCreatures that point at a thing that is not dead.
#include <cstdio>

struct CThing { unsigned char Dead : 1; };

struct CBaseIntelligentPointer
{
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer
{
    T* Get() const { return (T*)GetPItem(); }
};

template <class T>
struct CRetailVector
{
    T* _First; T* _Last; T* _End;
    unsigned int size() const { return (unsigned int)(_Last - _First); }
    const T& operator[](unsigned int i) const { return _First[i]; }
};

struct CTCBoss
{
    CRetailVector<CIntelligentPointer<CThing> > HelperCreatures;

    long CountLivingHelperCreatures() const
    {
        long count = 0;
        for (unsigned int i = 0; i < HelperCreatures.size(); ++i)
        {
            const CIntelligentPointer<CThing>& entry = HelperCreatures[i];
            if (entry.Get() && !entry.Get()->Dead)
                ++count;
        }
        return count;
    }
};

int main()
{
    CThing a, b, c; a.Dead = 0; b.Dead = 1; c.Dead = 0;
    CIntelligentPointer<CThing> list[4];
    list[0].m_pItem = &a; list[1].m_pItem = &b; list[2].m_pItem = 0; list[3].m_pItem = &c;
    CTCBoss boss; boss.HelperCreatures._First = list; boss.HelperCreatures._Last = list + 4; boss.HelperCreatures._End = list + 4;
    if (boss.CountLivingHelperCreatures() != 2) { std::printf("007aeec0_TEST FAIL\n"); return 1; }
    boss.HelperCreatures._Last = list;
    if (boss.CountLivingHelperCreatures() != 0) { std::printf("007aeec0_TEST FAIL\n"); return 1; }
    std::printf("007aeec0_TEST PASS\n");
    return 0;
}
