// ForEachIf<CThing**, CThingFilter_IsThrownByExplosion, CApplyAdditionalExplosiveImpulseIfInsideRing>(first, last, pred, func) -- for every thing in [first, last) that is alive and whose creature-mode manager's current mode is type 0x42, apply the extra explosive impulse inside the ring
// Leaf: ??$ForEachIf
// Retail: Fable.exe @ 0x0080E530, 156 bytes, `__fastcall` (first in ecx, last in edx), `ret 18h` (the empty filter's 4-byte slot +
// the 0x14-byte functor by value). Ego_r.pdb ??$ForEachIf@V?$_Vector_iterator<CThing*>@@VCThingFilter_IsThrownByExplosion@?A0x41ace303@@
// VCApplyAdditionalExplosiveImpulseIfInsideRing@@@@YIX... (Dinkumware iterator there; retail STLport uses the plain pointer).
// Callees: CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated; named by
// the landed CCreatureAction_BeStunned::Initialise 0x008EDA10), the mode's virtual slot 1 (its type),
// CApplyAdditionalExplosiveImpulseIfInsideRing::operator()(CThing*) const 0x0080DB70.
// Layout: CThing interface bits +0x20 (type 0x31 = bit 17 of +0x24), interface CVectorMap first/last +0x44/+0x48, Dead +0x91 bit 0.
// Shape: the anonymous-namespace filter is a `__forceinline` operator() written as one `&&` guard on `return true`; HasTC is
// `if (bits & mask) return true; return false;` (a `!= 0` return gives `shr ; test cl,1`); GetTC is the `__forceinline` inline
// lower_bound walk (last, then first, the count, then `it = first`) with the `it == last || type < it->type` miss test.
class CTCBase;
class CMode {
public:
    virtual void v00();
    virtual long GetModeType();             // slot 1
};
class CTCCreatureModeManager {              // interface type 0x31
public:
    CMode* GetCurrentMode();                // 0x0073A6E0
};
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                          // +0x00
    SEntry* last;                           // +0x04
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];         // +0x20
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                 // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long type) const
    {
        CInterfaceMap::SEntry* last = m_interfaces.last;
        CInterfaceMap::SEntry* first = m_interfaces.first;
        int count = last - first;
        CInterfaceMap::SEntry* it = first;
        while (count > 0) {
            int half = count / 2;
            if (it[half].type < type) {
                it += half + 1;
                count -= half + 1;
            } else
                count = half;
        }
        if (it == last || type < it->type)
            it = last;
        return (T*)it->component;
    }
};

namespace {
struct CThingFilter_IsThrownByExplosion {
    __forceinline bool operator()(CThing* thing) const
    {
        if (thing && !thing->Dead && thing->HasTC(0x31)
            && thing->GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetModeType() == 0x42)
            return true;
        return false;
    }
};
}

struct CApplyAdditionalExplosiveImpulseIfInsideRing {   // 0x14 bytes by value
    float m_data[5];
    void operator()(CThing* thing) const;   // 0x0080DB70
};

template <class It, class Pred, class Func> void __fastcall ForEachIf(It first, It last, Pred pred, Func func)
{
    for (; first != last; ++first)
        if (pred(*first))
            func(*first);
}
template void __fastcall ForEachIf<CThing**, CThingFilter_IsThrownByExplosion, CApplyAdditionalExplosiveImpulseIfInsideRing>(
    CThing** first, CThing** last, CThingFilter_IsThrownByExplosion pred, CApplyAdditionalExplosiveImpulseIfInsideRing func);