// CAnimatingObjectDef::Transfer(CPersistContext& context) -- persist the animation set, then re-sort its entry map
// Leaf: ?Transfer@CAnimatingObjectDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ED459, virtual (Ego_r.pdb ?Transfer@CAnimatingObjectDef@@UAEXAAVCPersistContext@@@Z), `ret 4`.
// Callees: CPersistContext::Transfer<CAnimationSet> 0x0047367A (landed under that name), then
//   CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess>::SortAndTrim 0x005DEDC0 on Animation.Anims.
// Layout: Ego_r.pdb CAnimatingObjectDef: Animation +0x28 (retail CAnimationSet 0x20 bytes); esi = &Animation.
// namepass DEF1: was a free `Transfer` over SubObject::Finalise / CPersistContext::Persist stand-ins.
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

void CAnimatingObjectDef::Transfer(CPersistContext& context)
{
    CAnimationSet* animation = &Animation;
    context.Transfer(*animation);
    animation->Anims.SortAndTrim();
}
