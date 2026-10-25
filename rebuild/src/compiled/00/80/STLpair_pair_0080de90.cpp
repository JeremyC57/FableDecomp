// _STL::pair::pair(const pair<CIntelligentPointer<CThing>, int>& p) -- converting constructor of pair<CIntelligentPointer<CThing>, long> from the int pair make_pair returns: copy the pointer, widen the int
// Leaf: ??$?0V?$CIntelligentPointer@VCThing@@@@H@?$pair@V?$CIntelligentPointer@VCThing@@@@J@_STL@@QAE@ABU?$pair@V?$CIntelligentPointer@VCThing@@@@H@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080DE90, 47 bytes, __fastcall `ret 4`, returns `this`.
// Identity: the catalog name pair<CIntelligentPointer<CThing>,long><CIntelligentPointer<CThing>,int> (STLport's member
//   template `template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p)`). The only caller, 0x00810848, converts the
//   result of make_pair<CIntelligentPointer<CThing>, int> (0x0080E200, hidden result pointer) into a frame
//   pair<CIntelligentPointer<CThing>, long> that it hands to a list insert (0x00810D40); FableWin.pdb has the
//   list<pair<CIntelligentPointer<CThing>, long>> node allocator.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90;
//   CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: first(__p.first) is CIntelligentPointer<CThing>'s inline copy constructor (base constructor, own vtable store
//   between the source load and GetPItem, SetPItem), then the int moved into the long.
class CBaseObject;
class CThing;
class CThingAICreature;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                 // each instantiation has its own vtable
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template pair<CIntelligentPointer<CThing>, long>::pair(const pair<CIntelligentPointer<CThing>, int>&);
}