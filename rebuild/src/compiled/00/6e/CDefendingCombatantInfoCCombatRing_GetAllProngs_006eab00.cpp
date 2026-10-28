// CDefendingCombatantInfo::CCombatRing::GetAllProngs(vector<CProng*>& prongs) -- append the address of every prong of this combat ring to the caller's vector
// Leaf: ?GetAllProngs@CCombatRing@CDefendingCombatantInfo@@QAEXAAV?$vector@PAVCProng@CDefendingCombatantInfo@@V?$allocator@PAVCProng@CDefendingCombatantInfo@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EAB00, 81 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb CDefendingCombatantInfo::CCombatRing (RotationAngle, DeltaRotation, Index, RingThickness,
//   Prongs vector<CProng> +0x10) with 0x1C-byte CProng elements (POccupier CIntelligentPointer at +0x14); the PDB's two
//   vector<CProng*>& collectors are GetAllFreeProngs then GetAllProngs in donor order, and retail has the free-prong filter
//   (IS_THING_ALIVE on +0x14) at 0x006EAA90 right before this unfiltered copy. The catalog label
//   "CMusicManager::ResetMusicEntriesOffsets" is propagated (that PDB function takes no argument).
// Callees: vector<CProng*>::_M_insert_overflow(pos, x, __true_type, 1, true) 0x006F25A0 (both collectors call it).
// Shape: STLport push_back inlined over a plain iterator loop (end re-read each pass); `&*it` is an rvalue, so VC7.1 homes
//   it in the dead `prongs` argument slot and the empty __true_type temporary shares that slot -- with the overflow
//   callee as a same-TU stand-in body under auto_inline(off) (the landed 0x0070D0A0 idiom).
extern "C" void* __cdecl malloc(unsigned int size);
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x006F25A0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};
// Same-TU stand-in body (the real one, 0x006F25A0, is out of line in retail): VC7.1 uses what it learns from it.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = (unsigned int)(this->_M_finish - this->_M_start);
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = this->_M_start; __p != __position; ++__p, ++__new_finish)
        *__new_finish = *__p;
    for (; __fill_len > 0; --__fill_len, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        for (_Tp* __q = __position; __q != this->_M_finish; ++__q, ++__new_finish)
            *__new_finish = *__q;
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)
}

class CThingCreatureBase;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class C3DVector { public: float X, Y, Z; };

class CDefendingCombatantInfo {
public:
    class CProng {
    public:
        bool                                    Blocked;              // +0x00
        unsigned char                           _prong[0xC];          // +0x01 C3DVector Prong (packed)
        long                                    ProngBlockageTimer;   // +0x10 (retail +0x10)
        CIntelligentPointer<CThingCreatureBase> POccupier;            // +0x14
    };
    class CCombatRing {
    public:
        float RotationAngle;                                // +0x00
        float DeltaRotation;                                // +0x04
        long  Index;                                        // +0x08
        float RingThickness;                                // +0x0C
        std::vector<CProng> Prongs;                         // +0x10
        void GetAllProngs(std::vector<CProng*>& prongs);
    };
};

void CDefendingCombatantInfo::CCombatRing::GetAllProngs(std::vector<CProng*>& prongs)
{
    for (std::vector<CProng>::iterator it = Prongs.begin(); it != Prongs.end(); ++it)
        prongs.push_back(&*it);
}