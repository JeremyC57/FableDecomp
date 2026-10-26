#include <cstdio>
// Behaviour test for 0x004ED459: the reconstructed body (pragmas stripped) driven through recording stubs;
// checks the transfer order, the helper chosen per member (kind) and the member address.
static const void* g_p[32]; static int g_k[32]; static int g_n = 0;
static void Rec(int k, const void* p) { if (g_n < 32) { g_k[g_n] = k; g_p[g_n] = p; } ++g_n; }
class CAnimationEntry;
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap { public:   // 0x10 bytes
    void* Data[3];                                     // +0x00 CArray< SPair<K, V> >
    C Compare;                                         // +0x0C
    bool Dirty;                                        // +0x0D
    void SortAndTrim();                                // 0x005DEDC0
};
class CAnimationSet { public:                          // retail 0x20 bytes
    CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > Anims;   // +0x00
    long DefaultFlags;                                 // +0x10
    long DefaultTransitionTime;                        // +0x14
    long DefaultDelay;                                 // +0x18
    long DefaultGroup;                                 // +0x1C CDefString
};
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CAnimatingObjectDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x24];       // CDefClassBase body
    CAnimationSet Animation;           // +0x28
};
template <> void CPersistContext::Transfer<CAnimationSet>(CAnimationSet& value);

void CAnimatingObjectDef::Transfer(CPersistContext& context)
{
    CAnimationSet* animation = &Animation;
    context.Transfer(*animation);
    animation->Anims.SortAndTrim();
}
template <> void CPersistContext::Transfer<CAnimationSet>(CAnimationSet& v) { Rec(13, &v); }
template <class K, class V, class C> void CVectorMap<K, V, C>::SortAndTrim() { Rec(14, this); }
static CAnimatingObjectDef d;
int main()
{
    CPersistContext c;
    d.Transfer(c);
    bool ok = g_n == 2;
    ok = ok && g_k[0] == 13 && g_p[0] == (const void*)&d.Animation;
    ok = ok && g_k[1] == 14 && g_p[1] == (const void*)&d.Animation.Anims;
    if (ok) { std::printf("004ed459_TEST PASS\n"); return 0; }
    std::printf("FAIL %d\n", g_n);
    return 1;
}
