// Behaviour test for 007aef10 CTCBoss::CountEffects: counts the entries of Effects that point at a thing that is not dead.
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
    CRetailVector<CIntelligentPointer<CThing> > Effects;

    long CountEffects() const
    {
        long count = 0;
        for (unsigned int i = 0; i < Effects.size(); ++i)
        {
            const CIntelligentPointer<CThing>& entry = Effects[i];
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
    CTCBoss boss; boss.Effects._First = list; boss.Effects._Last = list + 4; boss.Effects._End = list + 4;
    if (boss.CountEffects() != 2) { std::printf("007aef10_TEST FAIL\n"); return 1; }
    boss.Effects._Last = list;
    if (boss.CountEffects() != 0) { std::printf("007aef10_TEST FAIL\n"); return 1; }
    std::printf("007aef10_TEST PASS\n");
    return 0;
}