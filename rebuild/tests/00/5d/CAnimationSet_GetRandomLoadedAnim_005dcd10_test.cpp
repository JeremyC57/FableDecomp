#include <stdio.h>
#include <stdlib.h>
#include "engine/GFRandom.h"
long FABLE_FASTCALL GFROR13(unsigned long value) {
    return (value >> 13) | (value << 19);
}
#define FABLE_RETAIL_ASSERT(condition) do { if (!(condition)) abort(); } while (0)

namespace std {
template<class T> class vector {
    T values[24];
    unsigned long count;
public:
    vector() : count(0) {}
    void reserve(unsigned long) {}
    void push_back(const T& v) { if (count == 24) abort(); values[count++] = v; }
    unsigned long size() const { return count; }
    bool empty() const { return count == 0; }
    const T& operator[](unsigned long pos) const { if (pos >= count) abort(); return values[pos]; }
};
}
struct CCharString { unsigned long key; };
struct CAnimationEntry { unsigned int slot; bool accepted; bool loaded; mutable int load_calls; };
struct CAnimFilterBase {
    virtual bool operator()(const CAnimationEntry& entry) const { return entry.accepted; }
};
class CAnimationSet {
public:
    struct Pair { unsigned long first; CAnimationEntry second; };
    struct TAnimMap {
        typedef const Pair* const_iterator;
        Pair items[10];
        const_iterator end() const { return items+10; }
    } Anims;
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const {
        for (int i=0; i<10; ++i) if (Anims.items[i].first == name.key) return Anims.items+i;
        return Anims.end();
    }
    const CAnimationEntry* GetRandomLoadedAnim(const CCharString&, unsigned long&, bool, const CAnimFilterBase&) const;
private:
    bool IsAnimLoaded(const CAnimationEntry& entry) const { return entry.loaded; }
    void BeginLoading(const CAnimationEntry& entry) const { ++entry.load_calls; }
};
const CAnimationEntry* CAnimationSet::GetRandomLoadedAnim(const CCharString& name, unsigned long& seed,
                                                          bool force_something, const CAnimFilterBase& filter) const
{
    std::vector<TAnimMap::const_iterator> found_anims;
    found_anims.reserve(5);
    std::vector<TAnimMap::const_iterator> found_loaded_anims;
    found_loaded_anims.reserve(5);
    TAnimMap::const_iterator start_it = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = start_it; it != Anims.end() && it->first == start_it->first; ++it) {
        if (filter(it->second)) {
            found_anims.push_back(it);
            if (IsAnimLoaded(it->second))
                found_loaded_anims.push_back(it);
        }
    }
    long no_anims = found_anims.size();
    FABLE_RETAIL_ASSERT(no_anims>0);
    long index = GFRandom(no_anims, &seed);
    if (IsAnimLoaded(found_anims[index]->second))
        return &found_anims[index]->second;
    BeginLoading(found_anims[index]->second);
    if (!found_loaded_anims.empty()) {
        long loaded_index = GFRandom(found_loaded_anims.size(), &seed);
        return &found_loaded_anims[loaded_index]->second;
    }
    if (force_something)
        return &found_anims[index]->second;
    return 0;
}

unsigned long next_seed(unsigned long seed) {
    unsigned long v=seed*9377UL+9439UL;
    return (v>>13)|(v<<19);
}

int main() {
    CAnimationSet set;
    CAnimFilterBase filter;
    const CCharString name = { 2 };
    const unsigned long seeds[] = {0, 1, 27, 0xFFFFFFFFUL, 0x80104000UL};
    for (unsigned long state=0; state<256; ++state) {
        for (int force=0; force<2; ++force) {
            for (unsigned int si=0; si<5; ++si) {
                int all[8], loaded[8], na=0, nl=0;
                for (int i=0; i<10; ++i) {
                    CAnimationSet::Pair& e=set.Anims.items[i];
                    e.first=i==0 ? 1 : (i==9 ? 3 : 2);
                    e.second.slot=i;
                    e.second.accepted=i!=4 && i!=7;
                    e.second.loaded=(i>=1 && i<=8 && (state & (1UL<<(i-1)))!=0);
                    e.second.load_calls=0;
                    if (i>=1 && i<=8 && e.second.accepted) {
                        all[na++]=i;
                        if(e.second.loaded) loaded[nl++]=i;
                    }
                }
                unsigned long seed=seeds[si], expected_seed=next_seed(seed);
                int first=all[expected_seed%na], chosen=first;
                bool load_requested=!set.Anims.items[first].second.loaded;
                if (load_requested && nl) {
                    expected_seed=next_seed(expected_seed);
                    chosen=loaded[expected_seed%nl];
                } else if (load_requested && !force) chosen=-1;
                const CAnimationEntry* got=set.GetRandomLoadedAnim(name,seed,force!=0,filter);
                if (got!=(chosen<0 ? 0 : &set.Anims.items[chosen].second) || seed!=expected_seed) return 1;
                for (int k=0; k<10; ++k)
                    if (set.Anims.items[k].second.load_calls!=(load_requested && first==k ? 1 : 0)) return 2;
            }
        }
    }
    puts("005dcd10_TEST PASS");
    return 0;
}