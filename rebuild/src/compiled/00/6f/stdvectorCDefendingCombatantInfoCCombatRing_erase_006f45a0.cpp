// std::vector<CDefendingCombatantInfo::CCombatRing>::erase(CCombatRing* position) -- shift the later rings down one slot, destroy the old last ring (its prong vector), return position
// Leaf: ?erase@?$vector@VCCombatRing@CDefendingCombatantInfo@@V?$allocator@VCCombatRing@CDefendingCombatantInfo@@@std@@@std@@QAEPAVCCombatRing@CDefendingCombatantInfo@@PAV34@@Z
// Retail: Fable.exe @ 0x006F45A0, 94 bytes, thiscall `ret 4`.
// Identity (probable): the STLport 4.6 `vector::erase(iterator)` (`if (position + 1 != end()) __copy_ptrs(position + 1,
//   finish, position, __false_type()); --finish; _Destroy(finish); return position;`) over the 28-byte retail
//   CDefendingCombatantInfo::CCombatRing (Ego_r: four words, then vector<CProng> Prongs at +0x10; the Ego_r 0x20 is
//   with a 16-byte vector). The inline __copy_ptrs calls the random-access __copy 0x006F4470 (landed as
//   Copy_SVectorRecord1C, a descriptive stand-in: its element operator= runs vector<CProng>::operator= 0x006F3C20)
//   with the iterator-tag reference homed in the position argument's slot and a null distance pointer; the inline
//   ~CCombatRing destroys each 28-byte CProng (only CIntelligentPointer<CThingCreatureBase> POccupier +0x14 is
//   non-trivial: ~CBaseIntelligentPointer 0x00A01C10) and frees the storage (free 0x00BFEA14). FableWin.pdb has
//   `?erase@?$vector@VCCombatRing@CDefendingCombatantInfo@@...`. Its caller 0x006EC7B0 erases end() - 1 when the
//   vector is not empty (beside the landed CCombatWheel::AddRing 0x006EC7D0).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};

class CThingCreatureBase;
#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };                // retail C3DVector is 1-aligned
#pragma pack(pop)

namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    ~_Vector_base() { if (_M_start) _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x006F4470

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator end() { return this->_M_finish; }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, __false_type());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};
}

class CDefendingCombatantInfo {
public:
    class CProng { public:                                 // 28 bytes
        bool                                    Blocked;              // +0x00
        C3DVector                               Prong;                // +0x01
        long                                    ProngBlockageTimer;   // +0x10
        CIntelligentPointer<CThingCreatureBase> POccupier;            // +0x14
    };
    class CCombatRing { public:                            // Ego_r.pdb (retail 0x1c: 12-byte STLport vector)
        float RotationAngle;                               // +0x00
        float DeltaRotation;                               // +0x04
        long Index;                                        // +0x08
        float RingThickness;                               // +0x0c
        std::vector<CProng> Prongs;                        // +0x10
    };
};

template std::vector<CDefendingCombatantInfo::CCombatRing>::iterator
std::vector<CDefendingCombatantInfo::CCombatRing>::erase(std::vector<CDefendingCombatantInfo::CCombatRing>::iterator);