// Behaviour test for CTCHitLocations::GetPHitLocationFromName (0x00815820): the first hit location
// whose def name matches is returned, otherwise null.
#include <cstdio>
#include <cstring>

struct CCharString {
    const char* m_pText;
};

struct CDefString {
    long TablePos;
    CDefString(long pos) : TablePos(pos) {}
    CDefString(const CDefString& other) : TablePos(other.TablePos) {}
    bool operator==(const CCharString& other) const;
};

static const char* g_table[] = { "Head", "Torso", "Legs" };

bool CDefString::operator==(const CCharString& other) const
{
    return std::strcmp(g_table[TablePos], other.m_pText) == 0;
}

struct CHitLocationDef {
    long m_name;
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual void v07() {}
    virtual void v08() {}
    virtual void v09() {}
    virtual CDefString GetInstantiationName() const { return CDefString(m_name); }
};

template <class T>
struct CDefPointer {
    T* m_pObject;
    T* operator->() const { return m_pObject; }
};

namespace NHitLocations {
struct CHitLocation {
    void*                        POwnerThing;
    CDefPointer<CHitLocationDef> PDef;
    void*                        PArmourDef;
    long                         HitResponseAction;
    long                         BlockResponseAction;
    bool                         Enabled;
};
}

struct SHitLocationEntry { NHitLocations::CHitLocation first; long second; };
struct CHitLocationVector { SHitLocationEntry* m_first; SHitLocationEntry* m_last; SHitLocationEntry* m_end; };

struct CTCHitLocations {
    virtual void v00() {}
    unsigned char      _base[8];
    CHitLocationVector HitLocations;
    NHitLocations::CHitLocation* GetPHitLocationFromName(const CCharString& name);
};

NHitLocations::CHitLocation* CTCHitLocations::GetPHitLocationFromName(const CCharString& name)
{
    for (SHitLocationEntry* it = HitLocations.m_first; it != HitLocations.m_last; ++it) {
        CDefString defName = it->first.PDef->GetInstantiationName();
        if (defName == name)
            return &it->first;
    }
    return 0;
}

int main()
{
    CHitLocationDef defs[3];
    SHitLocationEntry entries[3];
    for (int i = 0; i < 3; ++i) {
        defs[i].m_name = i;
        entries[i].first.PDef.m_pObject = &defs[i];
        entries[i].second = i;
    }
    CTCHitLocations tc;
    tc.HitLocations.m_first = entries; tc.HitLocations.m_last = entries + 3; tc.HitLocations.m_end = entries + 3;
    CCharString torso = { "Torso" };
    CCharString legs = { "Legs" };
    CCharString tail = { "Tail" };
    bool ok = tc.GetPHitLocationFromName(torso) == &entries[1].first && tc.GetPHitLocationFromName(legs) == &entries[2].first
        && tc.GetPHitLocationFromName(tail) == 0;
    tc.HitLocations.m_last = entries;
    ok = ok && tc.GetPHitLocationFromName(torso) == 0;
    if (!ok) {
        std::printf("00815820_TEST FAIL\n");
        return 1;
    }
    std::printf("00815820_TEST PASS\n");
    return 0;
}
