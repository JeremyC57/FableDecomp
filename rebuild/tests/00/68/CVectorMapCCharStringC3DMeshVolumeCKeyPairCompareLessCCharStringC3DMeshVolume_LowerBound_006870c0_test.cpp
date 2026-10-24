// behaviour test for 006870c0: the lower bound over sorted 16-byte CCharString-keyed entries
#include <cstdio>
#include <cstring>
template <class C> class CBasicString {
public:
    const char* Text;
    bool operator<(const CBasicString& other) const { return std::strcmp(Text, other.Text) < 0; }
};
class CCharString {
public:
    CBasicString<char>* PData;
    bool Less(const CCharString& other) const
    {
        if (other.PData == PData) return false;
        if (PData == 0) return true;
        if (other.PData == 0) return false;
        return *PData < *other.PData;
    }
};
struct SValue { unsigned char _payload[12]; };
struct SEntry { CCharString first; SValue second; };
struct SMap { SEntry* First; SEntry* Last; SEntry* EndOfStorage; };
static SEntry* Bound(SMap* m, const CCharString& key)
{
    SEntry* first = m->First;
    int count = (int)(m->Last - m->First);
    if (count > 0)
    {
        CCharString k = key;
        do
        {
            int half = count / 2;
            SEntry* middle = first + half;
            if (middle->first.Less(k))
            {
                first = middle + 1;
                count -= half + 1;
            }
            else
                count = half;
        } while (count > 0);
    }
    return first;
}
int main()
{
    static const char* names[5] = { "apple", "bear", "bear", "cat", "dog" };
    CBasicString<char> data[5];
    SEntry entries[5];
    int i = 0;
    for (i = 0; i < 5; ++i) { data[i].Text = names[i]; entries[i].first.PData = &data[i]; }
    SMap m; m.First = entries; m.Last = entries + 5; m.EndOfStorage = entries + 5;
    CBasicString<char> kb; kb.Text = "bear"; CCharString key; key.PData = &kb;
    CBasicString<char> kz; kz.Text = "zebra"; CCharString keyz; keyz.PData = &kz;
    CBasicString<char> ka; ka.Text = "aardvark"; CCharString keya; keya.PData = &ka;
    CCharString keyn; keyn.PData = 0;
    int ok = 1;
    if (Bound(&m, key) != entries + 1) ok = 0;
    if (Bound(&m, keyz) != entries + 5) ok = 0;
    if (Bound(&m, keya) != entries + 0) ok = 0;
    if (Bound(&m, keyn) != entries + 0) ok = 0;          // a null key sorts first
    SMap e; e.First = entries; e.Last = entries; e.EndOfStorage = entries;
    if (Bound(&e, key) != entries) ok = 0;
    if (sizeof(SValue) != 12) ok = 0;   // the opaque payload after the 4-byte key (32-bit layout)
    if (!ok) { std::printf("006870c0_TEST FAIL\n"); return 1; }
    std::printf("006870c0_TEST PASS\n");
    return 0;
}