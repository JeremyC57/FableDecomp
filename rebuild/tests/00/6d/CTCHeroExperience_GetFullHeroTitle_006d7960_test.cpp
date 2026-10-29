// 006d7960 behaviour test: CTCHeroExperience::GetFullHeroTitle (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stdio.h>
#include <string.h>
#include <wchar.h>

// wide-string model: stores wchar_t units (the body's L"..." literals are cast to const unsigned short*)
class CWideString {
public:
    wchar_t Data[128];
    CWideString() { Data[0] = 0; }
    CWideString(const wchar_t* t) { Data[0] = 0; Append(t); }
    CWideString(const CWideString& other) { memcpy(Data, other.Data, sizeof(Data)); }
    void Append(const wchar_t* t) { size_t n = wcslen(Data); while (*t && n < 127) Data[n++] = *t++; Data[n] = 0; }
    bool operator!=(const unsigned short* text) const { return wcscmp(Data, (const wchar_t*)text) != 0; }
    const CWideString& operator+=(const unsigned short* text) { Append((const wchar_t*)text); return *this; }
    const CWideString& operator+=(const CWideString& other) { Append(other.Data); return *this; }
};

template<class T> class CDefPointer;
class CTitleDef;
static int g_defDestroys, g_textDestroys;
class CDefClassBase {
public:
    void Destroy() const { ++g_defDestroys; }
    mutable long RefCount;
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    bool GetSubDef(CDefPointer<const CTitleDef>* out) const;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
template<class T> class CCountedPointer {
public:
    T* Object;
    CCountedPointer() : Object(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }   // test model: refcounted copy (VC7.1 /Od does not elide the return copy)
    __forceinline ~CCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
class CTitleDef : public CDefClassBase {
public:
    long TextID;
    long MinAge;
    long MinStat[3];
    bool Enabled;
    bool HasTitle;   // test hook: GetSubDef finds a title sub-def
};
bool CDefClassBase::GetSubDef(CDefPointer<const CTitleDef>* out) const
{
    const CTitleDef* t = (const CTitleDef*)this;
    if (!t->HasTitle)
        return false;
    ++RefCount;
    out->Object = t;
    return true;
}
static CTitleDef g_titles[4];
class CDefinitionManager {
public:
    CCountedPointer<CDefClassBase> GetPDefFromGlobalIndex(long index)
    {
        CCountedPointer<CDefClassBase> p;
        p.Object = &g_titles[index];
        ++g_titles[index].RefCount;
        return p;
    }
};
static CDefinitionManager g_defs;
class CTextEntry {
public:
    void Destroy() const { ++g_textDestroys; }
    mutable long RefCount;
    const wchar_t* Text;
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    CWideString GetText() const { return CWideString(Text); }
};
static CTextEntry g_texts[4];
static bool g_textExists[4];
class CDataBank {
public:
    CCountedPointer<CTextEntry> GetPTextEntry(long id, bool flag)
    {
        (void)flag;
        CCountedPointer<CTextEntry> p;
        p.Object = g_textExists[id] ? &g_texts[id] : 0;
        if (p.Object) ++p.Object->RefCount;
        return p;
    }
};
static CDataBank g_bank;
struct CGameTextManager { CDataBank* PDataBank; };
static CGameTextManager g_textManager = { &g_bank };
static CGameTextManager* g_GameText = &g_textManager;
inline CDataBank* GetGameTextBank() { return g_GameText->PDataBank; }
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; const T* begin() const { return _M_start; } const T* end() const { return _M_finish; } long size() const { return end() - begin(); } const T& operator[](long n) const { return *(begin() + n); } };
class CHeroExperienceDef {
public:
    vector<long> Titles;
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
    {
        SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
};
class CThing {
public:
    unsigned long PresentBits[9];
    CInterfaceMap Interfaces;
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
class CTCBase {
public:
    CThing* Thing;
    CDefinitionManager* GetDefinitions() const { return &g_defs; }
};
static const wchar_t* g_heroTitle;
class CTCHero : public CTCBase {
public:
    CWideString GetTitleGuiText() const { return CWideString(g_heroTitle); }
};
class CTCAge : public CTCBase {
public:
    long Age;
};
static long g_stats[3];
class CTCHeroExperience : public CTCBase {
public:
    const CHeroExperienceDef* PDef;
    long GetTotalStatLevel(long category) const { return g_stats[category]; }
    CWideString GetFullHeroTitle() const;
};

CWideString CTCHeroExperience::GetFullHeroTitle() const
{
    CTCHero* hero;
    if (Thing->HasTC(0x29))
        hero = (CTCHero*)Thing->GetTC(0x29);
    CWideString title = hero->GetTitleGuiText();
    CTCAge* age;
    if (Thing->HasTC(4))
        age = (CTCAge*)Thing->GetTC(4);
    for (long i = PDef->Titles.size() - 1; i >= 0; --i) {
        CCountedPointer<CDefClassBase> def = GetDefinitions()->GetPDefFromGlobalIndex(PDef->Titles[i]);
        CDefPointer<const CTitleDef> titleDef;
        def->GetSubDef(&titleDef);
        if (titleDef.Object && titleDef->Enabled && age->Age >= titleDef->MinAge && GetTotalStatLevel(0) >= titleDef->MinStat[0]
            && GetTotalStatLevel(1) >= titleDef->MinStat[1] && GetTotalStatLevel(2) >= titleDef->MinStat[2]) {
            CCountedPointer<CTextEntry> text = GetGameTextBank()->GetPTextEntry(titleDef->TextID, true);
            if (text.Object) {
                if (title != (const unsigned short*)L"")
                    title += (const unsigned short*)L" - ";
                title += text->GetText();
                break;
            }
        }
    }
    return title;
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }
static bool Is(const CWideString& s, const wchar_t* t) { return wcscmp(s.Data, t) == 0; }
static bool RefsBalanced() { for (int i = 0; i < 4; ++i) if (g_titles[i].RefCount != 1 || g_texts[i].RefCount != 1) return false; return true; }

int main()
{
    CTCHero hero; CTCAge age; age.Age = 30;
    CInterfaceMap::SEntry entries[3] = { { 4, &age }, { 0x10, 0 }, { 0x29, &hero } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.PresentBits[i] = 0;
    thing.PresentBits[0] |= 1UL << 4;
    thing.PresentBits[1] |= 1UL << (0x29 & 31);
    thing.Interfaces.first = entries; thing.Interfaces.last = entries + 3;
    long titleIndices[3] = { 0, 1, 2 };
    CHeroExperienceDef def; def.Titles._M_start = titleIndices; def.Titles._M_finish = titleIndices + 3;
    CTCHeroExperience exp; exp.Thing = &thing; exp.PDef = &def;
    static const wchar_t* texts[4] = { L"Novice", L"Adept", L"Master", L"Unused" };
    for (int i = 0; i < 4; ++i) {
        g_titles[i].RefCount = 1; g_titles[i].TextID = i; g_titles[i].MinAge = 10 * i; g_titles[i].Enabled = true; g_titles[i].HasTitle = true;
        g_titles[i].MinStat[0] = g_titles[i].MinStat[1] = g_titles[i].MinStat[2] = i;
        g_texts[i].RefCount = 1; g_texts[i].Text = texts[i]; g_textExists[i] = true;
    }
    g_stats[0] = g_stats[1] = g_stats[2] = 5;
    g_heroTitle = L"Hero";

    // the last qualifying entry (walked from the back) wins, appended with " - "
    Check("master", Is(exp.GetFullHeroTitle(), L"Hero - Master") && RefsBalanced());
    // too young for Master (MinAge 20): falls back to Adept
    age.Age = 15;
    Check("adept by age", Is(exp.GetFullHeroTitle(), L"Hero - Adept") && RefsBalanced());
    age.Age = 30;
    // stat 2 too low for Master and Adept
    g_stats[2] = 0;
    Check("novice by stat", Is(exp.GetFullHeroTitle(), L"Hero - Novice") && RefsBalanced());
    g_stats[2] = 5;
    // disabled / missing text / missing sub-def are skipped
    g_titles[2].Enabled = false; g_textExists[1] = false; g_titles[0].HasTitle = false;
    Check("none", Is(exp.GetFullHeroTitle(), L"Hero") && RefsBalanced());
    g_titles[0].HasTitle = true;
    Check("novice", Is(exp.GetFullHeroTitle(), L"Hero - Novice") && RefsBalanced());
    // empty hero title: no separator
    g_heroTitle = L"";
    Check("no separator", Is(exp.GetFullHeroTitle(), L"Novice") && RefsBalanced());
    // empty list
    def.Titles._M_finish = titleIndices;
    g_heroTitle = L"Hero";
    Check("empty", Is(exp.GetFullHeroTitle(), L"Hero"));
    Check("no destroys", g_defDestroys == 0 && g_textDestroys == 0);
    if (g_fail) return 1;
    printf("006d7960_TEST PASS\n");
    return 0;
}