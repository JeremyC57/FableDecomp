// Behaviour test for 0x00686F60 CTCBase::GetPTCInfo: a registered name yields its info (the pair's second), an
// unregistered one -- between keys, past the end, or the null name against non-null keys -- yields null.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif

class CThing;
class CTCBase;

class CBasicString {
public:
    const char* Text;
    bool operator<(const CBasicString* other) const { return std::strcmp(Text, other->Text) < 0; }   // models 0x00429950
};
class CCharString {
public:
    CBasicString* PString;
    bool operator<(const CCharString& o) const
    {
        if (o.PString == PString) return false;
        if (!PString) return true;
        if (!o.PString) return false;
        return PString->operator<(o.PString);
    }
};

class CTCBase {
public:
    class CRegisteredTCInfo {
    public:
        void*        AllocFunc;
        unsigned int DriverType;
        CCharString  Name;
    };
    static const CRegisteredTCInfo* __fastcall GetPTCInfo(const CCharString& name);
};

template <class _T1, class _T2> class Pair { public: _T1 first; _T2 second; };

template <class K, class V> class CRegisteredTCMap {
public:
    Pair<K, V>* m_first;
    Pair<K, V>* m_last;
    Pair<K, V>* end() { return m_last; }
    Pair<K, V>* LowerBound(const K& key)                    // models 0x006870C0
    {
        Pair<K, V>* it = m_first;
        while (it != m_last && it->first < key) ++it;
        return it;
    }
};
CRegisteredTCMap<CCharString, CTCBase::CRegisteredTCInfo> g_RegisteredTCs;

const CTCBase::CRegisteredTCInfo* __fastcall CTCBase::GetPTCInfo(const CCharString& name)
{
    Pair<CCharString, CRegisteredTCInfo>* it = g_RegisteredTCs.LowerBound(name);
    if (it == g_RegisteredTCs.end() || name < it->first)
        return 0;
    return &it->second;
}

int main()
{
    CBasicString sa = { "Alpha" }, sm = { "Mid" }, sz = { "Zeta" }, qb = { "Beta" }, qm = { "Mid" }, qz = { "Zz" };
    Pair<CCharString, CTCBase::CRegisteredTCInfo> entries[3];
    entries[0].first.PString = &sa; entries[0].second.DriverType = 1;
    entries[1].first.PString = &sm; entries[1].second.DriverType = 2;
    entries[2].first.PString = &sz; entries[2].second.DriverType = 3;
    g_RegisteredTCs.m_first = entries; g_RegisteredTCs.m_last = entries + 3;
    CCharString mid; mid.PString = &qm;
    CCharString beta; beta.PString = &qb;
    CCharString past; past.PString = &qz;
    CCharString same; same.PString = &sz;
    CCharString none; none.PString = 0;
    const CTCBase::CRegisteredTCInfo* found = CTCBase::GetPTCInfo(mid);
    if (found != &entries[1].second || found->DriverType != 2 || CTCBase::GetPTCInfo(same) != &entries[2].second ||
        CTCBase::GetPTCInfo(beta) != 0 || CTCBase::GetPTCInfo(past) != 0 || CTCBase::GetPTCInfo(none) != 0) {
        std::printf("00686f60_TEST FAIL\n"); return 1;
    }
    std::printf("00686f60_TEST PASS\n");
    return 0;
}