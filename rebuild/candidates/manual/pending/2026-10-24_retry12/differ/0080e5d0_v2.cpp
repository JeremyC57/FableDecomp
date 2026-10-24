// Grade: DIFFER(158v158) -- one register copy: retail loads the map's first pointer into eax (`mov eax,[eax+44h] ; mov ecx,edi ; sub ecx,eax ; sar ecx,3 ; test ecx,ecx ; mov esi,eax`), ours loads it straight into esi (`mov esi,[eax+44h] ; ... sub ecx,esi`) and pads the loop alignment with a 6-byte `lea ebx,[ebx]` instead of `lea esp,[esp]` -- rest identical
// ForEachIf(TIterator first, TIterator last, TFilter filter, TFunction function) [TIterator = CThing**, TFilter = CThingFilter_IsThrownByExplosion, TFunction = CApplyAdditionalExplosiveImpulseIfInsideRing] -- apply the explosion-impulse functor to every thing the filter accepts
// Leaf: ForEachIf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080E5D0, length 158 bytes (__fastcall first, last; `ret 20h`: a 4-byte empty filter and the 28-byte functor by value).
// Callees: the physics TC's GetPhysicsObject 0x0073A6E0 and its result's slot 1 (== 0x42), the functor's operator() 0x0080DA50.
// Filter (inline): thing && !Dead (+0x91 bit 0) && HasTC(0x31) (`test [eax+24h],20000h`) && the 0x31 component (an inlined
//   CVectorMap lower_bound over the {kind, TC} entries at +0x44) has a physics object of type 0x42. Caller CTCDExplosion::UpdateExplosiveAdditionalImpulse.
// Tried: plain inline LowerBound/GetTC/operator() (not inlined: out-of-line calls), __forceinline on LowerBound, + GetTC, + the
//   filter (this file: the near miss). Next: the STLport free `__lower_bound(first, last, val, less, (long*)0)` with
//   `distance(first, last)` into its own variable (two values, first copied after the length test).
struct CTCBase;
struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};
template <class TIter, class T>
__forceinline TIter __lower_bound(TIter first, TIter last, const T& val)
{
    long len = last - first;
    long half;
    TIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if (middle->m_kind < val) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    __forceinline STCEntry* LowerBound(const long& kind) const
    {
        return __lower_bound(m_pFirst, m_pLast, kind);
    }
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[9];                             // +0x20
    CTCMap         m_tcs;                                   // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  StateBits;                               // +0x91
    bool IsDead() const { return (StateBits & 1) != 0; }
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) const
    {
        const CTCMap* map = &m_tcs;
        STCEntry* it = map->LowerBound(kind);
        return (it == map->m_pLast || kind < it->m_kind ? map->m_pLast : it)->m_pTC;
    }
};
struct CPhysicsObject {
    virtual void v00();
    virtual long GetType() const;                           // slot 1 (+4)
};
struct CTCPhysicsBase {
    CPhysicsObject* GetPhysicsObject() const;               // 0x0073A6E0
};
namespace {
struct CThingFilter_IsThrownByExplosion {
    __forceinline bool operator()(CThing* thing) const
    {
        return thing && !thing->IsDead() && thing->HasTC(0x31) &&
               ((CTCPhysicsBase*)thing->GetTC(0x31))->GetPhysicsObject()->GetType() == 0x42;
    }
};
struct CApplyAdditionalExplosiveImpulseIfInsideRing {
    float Data[7];
    void operator()(CThing* thing);                         // 0x0080DA50
};
}
template <class TIterator, class TFilter, class TFunction>
void __fastcall ForEachIf(TIterator first, TIterator last, TFilter filter, TFunction function)
{
    for (; first != last; ++first)
        if (filter(*first))
            function(*first);
}
template void __fastcall ForEachIf<CThing**, CThingFilter_IsThrownByExplosion, CApplyAdditionalExplosiveImpulseIfInsideRing>(CThing**, CThing**, CThingFilter_IsThrownByExplosion, CApplyAdditionalExplosiveImpulseIfInsideRing);
