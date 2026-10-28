// CTCAISummon::GetAllSummonedCreatures() -- the global list of every creature any summoner has summoned, first purged of entries whose creature has gone (null intelligent pointers)
// Leaf: ?GetAllSummonedCreatures@CTCAISummon@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FDEB0, 61 bytes (+ int3 padding). Ego_r.pdb / FableWin.pdb
//   `?GetAllSummonedCreatures@CTCAISummon@@SIAAV?$list@V?$CIntelligentPointer@VCThing@@@@...@std@@XZ` (public static
//   `__fastcall`, returns the list by reference); caller MakeSummonedCreaturesImmuneToTrap 0x0080F240. Catalog label
//   `GetAllSummonedCreatures` (_global).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::SetPItem 0x00A01B90, ::~CBaseIntelligentPointer
//   0x00A01C10; list<CIntelligentPointer<CThing> >::remove(const T&) 0x0065DE8A (manifest
//   "CEditControlCentre::SetScriptBrushThingsToMove" is propagated); CIntelligentPointer<CThing> vtable 0x01238C6C.
// Globals: CTCAISummon::AllSummonedCreatures 0x013BAF14 (Ego_r has its dynamic initializer / atexit destructor).
// Shape: `AllSummonedCreatures.remove(CIntelligentPointer<CThing>(0)); return AllSummonedCreatures;` -- the temporary's
//   inline constructor stores the derived vtable after the out-of-line base constructor and its compiler-generated
//   destructor calls only the base destructor (the landed CThingGameObject::OnCreate 0x00838A40 model).
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                              // +0x04
    CBaseIntelligentPointer();                                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                                 // 0x00A01C10
    CThing* GetPItem() const;                                   // 0x00A01B50
    void SetPItem(CThing* item);                                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {     // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {                         // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    void remove(const T& value);                                // 0x0065DE8A for T = CIntelligentPointer<CThing>
};

class CTCAISummon {
public:
    static CRetailList<CIntelligentPointerThing> AllSummonedCreatures;   // 0x013BAF14
    static CRetailList<CIntelligentPointerThing>& __fastcall GetAllSummonedCreatures();
};

CRetailList<CIntelligentPointerThing>& __fastcall CTCAISummon::GetAllSummonedCreatures()
{
    AllSummonedCreatures.remove(CIntelligentPointerThing(0));
    return AllSummonedCreatures;
}