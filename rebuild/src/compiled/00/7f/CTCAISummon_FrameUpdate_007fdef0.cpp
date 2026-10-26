// CTCAISummon::FrameUpdate() -- drop the dead (null) entries from the summoned-creature list, then run the base hook
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FDEF0, length 68 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::SetPItem
// @ 0x00A01B90, std::list<CIntelligentPointer<CThing> >::remove(const value_type&) @ 0x0065DE8A (manifest
// "CEditControlCentre::SetScriptBrushThingsToMove" is a propagated name; the body walks the list and erases
// each node whose pointer compares equal), CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10,
// CTCBase's frame hook @ 0x006868A0 (an empty `ret`, called non-virtually; manifest "$E2").
//
// Layout: CTCAISummon::MySummonedCreatures (list<CIntelligentPointer<CThing> >) is +0x10 (Ego_r.pdb).
// Shape: an 8-byte stack temporary CIntelligentPointer<CThing>(0): the inline converting constructor runs
// the base constructor, stores the derived vtable 0x01238C6C, then SetPItem(0); the derived class has no
// destructor of its own, so the temporary dies through the base destructor right after remove().
class CThing;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:      // vtable 0x01238C6C
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

template<class T>
struct CRetailList {                                        // retail std::list: one sentinel pointer
    void* m_head;
    void remove(const T& value);                            // 0x0065DE8A for T = CIntelligentPointer<CThing>
};

#pragma pack(push, 1)
struct CTCBase_Calls                                   // engine/CTCBase.h layout (the header holds the layout only)
{
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    unsigned char _pad_0x0a[0x2];                           // +0x0a
    void FrameUpdate();                                     // 0x006868A0
};

class CTCAISummon : public CTCBase_Calls
{ public:
    void*                                         PSummonDef;            // +0x0c CDefPointer<CSummonDef>
    CRetailList<CIntelligentPointer<CThing> >     MySummonedCreatures;   // +0x10
    long                                          LastSummonTick;        // +0x14
    void FrameUpdate();
};
#pragma pack(pop)

void CTCAISummon::FrameUpdate()
{
    MySummonedCreatures.remove(CIntelligentPointer<CThing>(0));
    CTCBase_Calls::FrameUpdate();
}