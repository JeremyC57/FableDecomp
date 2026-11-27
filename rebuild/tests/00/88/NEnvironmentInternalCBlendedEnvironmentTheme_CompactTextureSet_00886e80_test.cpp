#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FABLE_RETAIL_ASSERT(x) do { if (!(x)) exit(3); } while(0)
template<class T> struct CArray {
    T data[13];
    long count;
    long size() const { return count; }
    T& operator[](unsigned int n) { if(n >= 13) exit(2); return data[n]; }
    void resize(long n) { if(n > count || n < 0) exit(4); count=n; }
};
template<class T> void SWAP(T& a,T& b) { T c=a; a=b; b=c; }
struct CIDrawEnvironmentTextureSet { CArray<long> Textures; CArray<float> Blends; };
namespace NEnvironmentInternal { class CBlendedEnvironmentTheme { public: void CompactTextureSet(CIDrawEnvironmentTextureSet&,long) const; }; }
void NEnvironmentInternal::CBlendedEnvironmentTheme::CompactTextureSet(CIDrawEnvironmentTextureSet& tset, long max_count) const
{
    long src_count = tset.Textures.size();
    FABLE_RETAIL_ASSERT(max_count >= 0);
    FABLE_RETAIL_ASSERT(src_count >= max_count);
    for (long n = 0; n < src_count; ++n) {
        for (long n2 = n + 1; n2 < src_count; ++n2) {
            if (tset.Textures[n2] == tset.Textures[n]) {
                tset.Blends[n] += tset.Blends[n2];
                tset.Blends[n2] = 0.0f;
            }
        }
    }
    bool sorting = true;
    while (sorting) {
        sorting = false;
        for (long n = 0; n < src_count - 1; ++n) {
            if (tset.Blends[n] < tset.Blends[n + 1]) {
                sorting = true;
                SWAP(tset.Blends[n], tset.Blends[n + 1]);
                SWAP(tset.Textures[n], tset.Textures[n + 1]);
            }
        }
    }
    for (long n = 0; n < max_count; ++n)
        tset.Blends[n] -= tset.Blends[max_count];
    float total_blends = 0.0f;
    for (long n = 0; n < max_count; ++n)
        total_blends += tset.Blends[n];
    if (total_blends < 0.00390625f) {
        for (long n = 0; n < max_count; ++n)
            tset.Blends[n] = (float)(1.0f / max_count);
    } else {
        for (long n = 0; n < max_count; ++n)
            tset.Blends[n] /= total_blends;
    }
    tset.Textures.resize(max_count);
    tset.Blends.resize(max_count);
}
int main()
{
    NEnvironmentInternal::CBlendedEnvironmentTheme theme;
    unsigned int seed=1349;
    for (int repeat=0;repeat<72;++repeat) for (int len=0;len<=12;++len) for (int keep=0;keep<=len;++keep) {
        CIDrawEnvironmentTextureSet set;
        long ids[13],order[13];
        float weights[13],expected[13];
        set.Textures.count=set.Blends.count=len;
        for (int i=0;i<=len;++i) {
            seed=1664525U*seed+1013904223U;
            ids[i]=set.Textures.data[i]=(seed>>16)%(1+repeat%9);
            weights[i]=set.Blends.data[i]=((int)((seed>>12)%9)-3)*(repeat%3==0?1.0f/1024.0f:0.25f);
            order[i]=i;
        }
        for(int i=0;i<len;++i) for(int j=0;j<i;++j) if(ids[i]==ids[j]) {
            weights[j]+=weights[i]; weights[i]=0; break;
        }
        for(int i=1;i<len;++i) {
            int j=i;
            while(j>0 && weights[order[j-1]]<weights[i]) { order[j]=order[j-1]; --j; }
            order[j]=i;
        }
        float sum=0;
        for(int i=0;i<keep;++i) { expected[i]=weights[order[i]]-weights[order[keep]]; sum+=expected[i]; }
        theme.CompactTextureSet(set,keep);
        if(set.Textures.count!=keep || set.Blends.count!=keep)return 5;
        for(int i=0;i<keep;++i) {
            float w=sum<1.0f/256.0f?1.0f/keep:expected[i]/sum;
            if(set.Textures.data[i]!=ids[order[i]] || fabs(w-set.Blends.data[i])>1e-6)return 6;
        }
    }
    puts("00886e80_TEST PASS"); return 0;
}