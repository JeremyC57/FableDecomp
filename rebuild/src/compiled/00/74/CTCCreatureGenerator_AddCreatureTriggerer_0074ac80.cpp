// CTCCreatureGenerator::AddCreatureTriggerer(CThingCreatureBase& thing) -- add the creature to the generator's triggerer list unless it is already there
// Leaf: ?AddCreatureTriggerer@CTCCreatureGenerator@@QAEXAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074AC80, 96 bytes, __fastcall `ret 4`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / SetPItem @ 0x00A01B90 / ~CBaseIntelligentPointer
//   @ 0x00A01C10 (the CIntelligentPointer<CThingCreatureBase> temporary, vtable 0x0123E7D8 whose RTTI is
//   `.?AV?$CIntelligentPointer@VCThingCreatureBase@@@@`), STLport __find(first, last, val, random_access_iterator_tag) @
//   0x0074CEE0 (__fastcall, out of line), vector::push_back @ 0x0074DA40.
// Layout: CreatureTriggerers (vector<CIntelligentPointer<CThingCreatureBase> >) at +0x1C in retail (begin +0x1C, end +0x20).
// Shape: `CIntelligentPointer<CThingCreatureBase> p(&thing); it = find(begin, end, p); if (it == CreatureTriggerers.end())
//   CreatureTriggerers.push_back(p);` -- the find result must be NAMED: with `find(...) == end()` VC7.1 evaluates the right-hand
//   end() first and compares against the pre-call value, retail re-reads +0x20 after the call. STLport's find passes an
//   empty random_access_iterator_tag temporary (user empty ctor, else a zero byte store) homed in the dead `thing` slot.
class CBaseObject;
class CThingCreatureBase;

class CBaseIntelligentPointer {                 // 8 bytes: vptr + the pointed-to item
public:
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class T>
RandomAccessIter __fastcall __find(RandomAccessIter first, RandomAccessIter last, const T& val,
                                   const random_access_iterator_tag&);                     // 0x0074CEE0

template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last, const T& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}

template <class T> class vector {
public:
    T* _M_start;                                 // +0x00
    T* _M_finish;                                // +0x04
    T* _M_end_of_storage;                        // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void push_back(const T& x);                  // 0x0074DA40
};
}

// Naming (NF446, 2026-11-25): self respelt from ?AddShopper@CTCShop@@QAEXPAVCThingCreatureBase@@@Z (catalog CTCShop::AddShopper; the old Label
//   line was wrong). Evidence: both PDB name sets (Ego_r 0x0075D330, FableWin 0x02519100 ?AddCreatureTriggerer@CTCCreatureGenerator@@QAEXAAVCThingCreatureBase@@@Z);
//   FableWin's /Od CGameScriptInterface::CreatureGeneratorAddTriggerer calls it, and retail 0x0089EC60 calls 0x0074AC80 there (the only other retail
//   caller, 0x0074C20A, sits in CTCCreatureGenerator::FrameUpdate); the address sits in the CTCCreatureGenerator TU (0x0074AB00 .. 0x0074ACE0
//   UpdateUndead) next to its twin RemoveCreatureTriggerer 0x0074A1E0; the body pushes into +0x1C = CTCCreatureGenerator::CreatureTriggerers
//   (engine/CTCCreatureGenerator.h asserts +0x1c). The real CTCShop::AddShopper is 0x00773DC0 (Ego_r 0x00782450 / FableWin 0x02619DC0).
// Layout: the generated engine/CTCCreatureGenerator.h under the alias of the landed 0x0074A1E0 (its opaque CreatureTriggerers viewed as the STLport
//   vector); the row's intelligent-pointer / STLport stand-ins are kept as at HEAD (header debt unchanged), so its callee spellings do not move.
#define CTCCreatureGenerator CTCCreatureGenerator_HeaderLayout
#include "engine/CTCCreatureGenerator.h"
#undef CTCCreatureGenerator
class CTCCreatureGenerator : public CTCCreatureGenerator_HeaderLayout {
public:
    void AddCreatureTriggerer(CThingCreatureBase& thing);
};

void CTCCreatureGenerator::AddCreatureTriggerer(CThingCreatureBase& thing)
{
    CIntelligentPointer<CThingCreatureBase> pThing(&thing);
    CIntelligentPointer<CThingCreatureBase>* it = std::find(reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->begin(),
        reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->end(), pThing);
    if (it == reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->end())
        reinterpret_cast<std::vector<CIntelligentPointer<CThingCreatureBase> >*>(&CreatureTriggerers)->push_back(pThing);
}
