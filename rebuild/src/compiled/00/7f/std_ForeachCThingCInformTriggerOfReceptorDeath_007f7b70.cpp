// std::_For_each<CThing**,CInformTriggerOfReceptorDeath>(CThing** first, CThing** last, CInformTriggerOfReceptorDeath func) -- tell every trigger thing of a dying receptor to drop its receptor pointer; returns the functor
// Leaf: ??$_For_each@PAPAVCThing@@VCInformTriggerOfReceptorDeath@@@std@@YI?AVCInformTriggerOfReceptorDeath@@PAPAVCThing@@0V1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7B70, 156 bytes (`ret 8` + int3 padding), /Gr `__fastcall` returning the functor
//   through the hidden pointer (hidden result in ecx, first in edx, last and the by-value functor on the stack). FableWin.pdb
//   `??$_For_each@PAPAVCThing@@VCInformTriggerOfReceptorDeath@@@std@@YI?AVCInformTriggerOfReceptorDeath@@PAPAVCThing@@0V1@@Z`;
//   one caller, CTCActivationReceptorBase::OnKill 0x007F7850 (`for_each(Triggers.begin(), Triggers.end(),
//   CInformTriggerOfReceptorDeath(this))`).
// Callees: CTCActivationTrigger::SetPTCReceptor(CTCActivationReceptorBase*) @ 0x0077B070 (stores +0x0c).
// Shape: the functor's operator() is the out-parameter GetTC<CTCActivationTrigger>(&trigger) (interface 0x66, presence word 3
//   bit 6) with the map's lower_bound INLINED in this TU (signed halving `cdq ; sub ; sar`, `count += -1 - half`), its result
//   ignored, then `trigger->SetPTCReceptor(0)`; the search cursor IS the picked entry (a separate LowerBound result
//   variable copies it through eax); the never-written trigger local shares the unused functor argument's slot
//   (esi is loaded from it before the loop). operator() needs __forceinline once the search is written out.
class CTCBase;
class CTCActivationReceptorBase;

struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            STCEntry* last = m_tcs.m_pLast;                     // the map's lower_bound, inlined into one cursor
            STCEntry* it = m_tcs.m_pFirst;
            long count = last - it;
            while (count > 0) {
                long half = count / 2;
                STCEntry* middle = it + half;
                if (middle->m_kind < T::INTERFACE_TYPE) {
                    it = middle + 1;
                    count = count - half - 1;
                } else
                    count = half;
            }
            if (it == last || T::INTERFACE_TYPE < it->m_kind)
                it = last;
            *out = (T*)it->m_pTC;
            return true;
        }
        return false;
    }
};

class CTCActivationTrigger {
public:
    enum { INTERFACE_TYPE = 0x66 };
    void SetPTCReceptor(CTCActivationReceptorBase* receptor);   // 0x0077B070
};

class CInformTriggerOfReceptorDeath {
public:
    CTCActivationReceptorBase* PReceptor;
    CInformTriggerOfReceptorDeath(CTCActivationReceptorBase* receptor) : PReceptor(receptor) { }
    __forceinline void operator()(CThing* thing)
    {
        CTCActivationTrigger* trigger;
        thing->GetTC(&trigger);
        trigger->SetPTCReceptor(0);
    }
};

namespace std {
template <class _InIt, class _Fn1> _Fn1 __fastcall _For_each(_InIt first, _InIt last, _Fn1 func)
{
    for (; first != last; ++first)
        func(*first);
    return func;
}
}

template CInformTriggerOfReceptorDeath __fastcall std::_For_each<CThing**, CInformTriggerOfReceptorDeath>(CThing**, CThing**, CInformTriggerOfReceptorDeath);