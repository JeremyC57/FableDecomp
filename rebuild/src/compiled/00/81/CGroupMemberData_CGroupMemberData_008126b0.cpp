// CGroupMemberData::CGroupMemberData(const CGroupMemberData& other) -- copy constructor of a creature-group member record: the member's pointer, its active flag, joined and deactivated frames
// Leaf: ??0CGroupMemberData@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008126B0, 59 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<CIntelligentPointer<CThing>,long><CIntelligentPointer<CThing>,int>" is
//   propagated. A CIntelligentPointer<const CThingAICreature> (vtable 0x01264BAC, RTTI
//   .?AV?$CIntelligentPointer@$$CBVCThingAICreature@@@@) + byte + two dwords = Ego_r.pdb CGroupMemberData (20 bytes:
//   PMember +0, IsActive +8, JoinedFrame +0xC, DeactivatedFrame +0x10); FableWin.pdb ??0CGroupMemberData@@QAE@ABV0@@Z.
//   It sits in the CTCCreatureGroup TU (the map<unsigned __int64, CGroupMemberData> _Cons_val is 0x00812840). No callers.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Shape: the memberwise copy (pointer copy constructor inline, then the byte and the two dwords).
class CBaseObject;
class CThing;
class CThingAICreature;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                 // each instantiation has its own vtable
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CGroupMemberData {
public:
    CIntelligentPointer<const CThingAICreature> PMember;           // +0x00
    bool                                        IsActive;          // +0x08
    unsigned long                               JoinedFrame;       // +0x0C
    unsigned long                               DeactivatedFrame;  // +0x10
    CGroupMemberData(const CGroupMemberData& other);
};

CGroupMemberData::CGroupMemberData(const CGroupMemberData& other)
    : PMember(other.PMember), IsActive(other.IsActive), JoinedFrame(other.JoinedFrame), DeactivatedFrame(other.DeactivatedFrame)
{
}