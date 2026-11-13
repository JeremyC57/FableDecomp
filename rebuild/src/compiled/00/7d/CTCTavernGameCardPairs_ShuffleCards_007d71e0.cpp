// CTCTavernGameCardPairs::ShuffleCards() -- size the round to the card order's position count, shuffle the pack, then pair it up: every
//   card in the first half that repeats the value of an earlier card is moved to the back (re-testing the same slot), every card of the
//   second half is matched by a card of the same value as its first-half partner (moved there with erase + insert); the dealt cards are
//   shuffled again and the whole pack is stacked face down at the pack position, card k (53 - k) card separations above the table.
// Leaf: ?ShuffleCards@CTCTavernGameCardPairs@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D71E0, 818 bytes (+ int3 padding), thiscall, plain `ret`, no SEH frame. Flags: none.
// Identity: Ego_r.pdb ?ShuffleCards@CTCTavernGameCardPairs@@UAEXXZ (ego 0x007D4630, between NewRound and the ctor as in retail);
//   slot 60 of the CTCTavernGameCardPairs vtable 0x0126F6FC (overrides CTCTavernGameCardBase::ShuffleCards 0x008F10B0).
// Callees: CTCTavernGameCardBase::GetCardDef 0x008F0B20; GetCardOrder (vtable +0xE4, slot 57); std::random_shuffle<CCard**> 0x007D7860
//   (__fastcall, first call; the second, on [begin, begin + NumCards), is expanded inline from the same visible STLport body);
//   memmove 0x00BFEAE6 (inline vector::erase); vector<CCard*>::_M_insert_overflow 0x007D5D60 (inline push_back's growth path);
//   vector<CCard*>::insert 0x007D78A0; rand 0x00BFEB16; CTCTavernGameCardBase::GetPackPosition 0x008F0B80;
//   CTCTavernGame::HideMesh 0x005E9B10 / PositionMesh 0x005EABC0 (static __fastcall); CCard::Move 0x008F0660; def slot 1 (release).
// Shape: _M_insert_overflow's real body (landed 0x007D5D60) sits in the TU out of line (noinline): seeing it, VC7.1 knows push_back
//   never captures `card`, so it keeps the card in esi across the erase's memmove (0x007D72CC / 0x007D72FC); declaration-only reloads
//   it. The iterator advances are counting loops (`if (i > 0) it += i` with the strength-reduced byte offsets in frame words); the
//   first-half scan exits straight into the move (goto past ++i); loop 2 reads its partner card into a local before the scan.
//   The def release is the count-through-reference form (FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF); CardOrders is typed through the
//   overlay opt-in FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS (std::vector<CCardPositionsDef>, 0x48-byte records).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG
#define FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
#include <stdlib.h>

namespace std {
template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 i1, ForwardIter2 i2, T*)
{
    T tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}
template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 i1, ForwardIter2 i2)
{
    __iter_swap(i1, i2, i1);
}
template <class _Distance> inline _Distance __random_number(_Distance __n) { return rand() % __n; }
// STLport 4.6 _algo.h random_shuffle (body visible in the retail TU; out-of-line instance 0x007D7860)
template <class _RandomAccessIter>
void __fastcall random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        iter_swap(__i, __first + __random_number((__i - __first) + 1));
}
}

namespace std {
// 0x007D5D60 vector<CCard*>::_M_insert_overflow (landed row; real STLport 4.6 body, visible to retail's TU but called out of line)
#pragma auto_inline(off)
template<class _Tp,class _Alloc> __declspec(noinline) void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    _Tp* newStart=this->_M_end_of_storage.allocate(len);
    _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
    newFinish=fill_n(newFinish,fillLength,x);
    if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
#pragma auto_inline(on)
}

class CTCTavernGameCardPairs : public CTCTavernGameCardBase {
public:
    virtual void ShuffleCards();                             // slot 60
};

void CTCTavernGameCardPairs::ShuffleCards()
{
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    NumCards = pdef->CardOrders[GetCardOrder()].Offset.size();
    std::random_shuffle(Pack.begin(), Pack.end());
    for (long i = 0; i < NumCards / 2; ) {
        std::vector<CCard*>::iterator it = Pack.begin();
        for (long n = 0; n < i; ++n) ++it;
        for (std::vector<CCard*>::iterator p = Pack.begin(); p != it; ++p) {
            if ((*it)->Value == (*p)->Value) {
                CCard* card = *it;
                Pack.erase(it);
                Pack.push_back(card);
                goto next;
            }
        }
        ++i;
    next:;
    }
    for (long j = NumCards / 2; j < NumCards; ++j) {
        std::vector<CCard*>::iterator it = Pack.begin();
        for (long n = 0; n < j; ++n) ++it;
        CCard* other = Pack[j - NumCards / 2];
        std::vector<CCard*>::iterator p = it;
        while ((*p)->Value != other->Value)
            ++p;
        if (p != it) {
            CCard* card = *p;
            Pack.erase(p);
            Pack.insert(it, card);
        }
    }
    {
        std::vector<CCard*>::iterator last = Pack.begin();
        for (long n = 0; n < NumCards; ++n) ++last;
        std::random_shuffle(Pack.begin(), last);
    }
    for (unsigned int k = 0; k < pdef->CardPack.size(); ++k) {
        C3DVector pos(0.0f, 0.0f, (float)(53 - (int)k) * pdef->CardSeparation + pdef->TableSeparation);
        pos += GetPackPosition();
        CCard* card = Pack[k];
        HideMesh(card->Thing);
        PositionMesh(card->Thing, GamePositionPos, GamePositionRHSet, pos, 0.5f, 0.5f);
        card->Move(pos, true);
        card->FinalRotation = 0.5f;
        card->FaceDown = true;
    }
}