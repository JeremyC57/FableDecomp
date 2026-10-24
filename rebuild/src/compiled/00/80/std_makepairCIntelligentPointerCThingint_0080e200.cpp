// std::make_pair<CIntelligentPointer<CThing>,int>(const CIntelligentPointer<CThing>& x, const int& y) -- STLport make_pair: returns pair(x, y) by value
// Leaf: ??$make_pair@
// Retail: Fable.exe @ 0x0080E200, 48 bytes, plain `ret`: __cdecl (this TU is not /Gr) with the hidden pair result
// pointer as the first stack argument (-> esi, returned in eax); x and y by reference after it.
// Caller 0x0081083B builds the pair, copies it (0x0080DE90) and appends it to a list (0x00810D40 push_back).
// Shape: the returned temporary is constructed straight into the result slot (VC7.1 elides the copy of an unnamed
// temporary): pair(const T1&, const T2&) inlined -- CIntelligentPointer<CThing>'s copy constructor
// (CBaseIntelligentPointer() @ 0x00A01B10, vtable 0x01238C6C, SetPItem(x.GetPItem()) @ 0x00A01B50/0x00A01B90), then
// the int at +8. The second element type (int vs long) follows the catalog label; the bytes are identical for both.
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
    CBaseObject* PData;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                  // vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                            // +0x00
    T2 second;                                           // +0x08
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& x, const T2& y) { return pair<T1, T2>(x, y); }
}
template std::pair<CIntelligentPointer<CThing>, int> std::make_pair(const CIntelligentPointer<CThing>&, const int&);