// NCreatureMode::CModeJackOfBladesScream::SetSwordObject(CThing*) -- PSword pointed at the sword, InitialSwordPos = the sword's position
// Leaf: SetSwordObject
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7370, 109 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer: ctor 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50, ~ 0x00A01C10;
// CThing::GetPosition @ 0x004C73D0.
// Layout: Ego_r.pdb CModeJackOfBladesScream (offsets equal retail): Filter +0x68, PSword +0x74 (CIntelligentPointer<CThing>,
// vtable 0x01238C6C), InitialSwordRHS +0x7c, SwordFalling +0x94, SwordFallTimer +0x98, PEmitter +0x9c,
// SwordFallPos +0xa4, InitialSwordPos +0xb0.
// Shape: `PSword = CIntelligentPointer<CThing>(sword)` (8-byte frame temporary, inline operator= that takes GetPItem() into a local BEFORE SetPItem: retail calls GetPItem before forming the member address; the direct `SetPItem(other.GetPItem())` puts the `lea edi` first),
// then the position copied word by word through the pointer's inline operator-> (the member address stays in edi).
struct C3DVector { float x, y, z; };

struct CThing {
    const C3DVector& GetPosition() const;          // 0x004C73D0 (CThing::GetPosition, GOTCHAS chunk 141)
};
struct CThingCreatureBase : CThing { };

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                          // +4
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    void SetPItem(CBaseObject* item);              // 0x00A01B90
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
    T* operator->() const { return (T*)GetPItem(); }
};

struct CModeBase {
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
    unsigned char _pad08[0x14];
};

struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                    // +0x1c .. +0x68
};

namespace NCreatureMode {

struct CModeJackOfBladesScream : CModeStandBase {
    unsigned char               _filter[0xc];      // +0x68 CAnimFilterOfGroup Filter
    CIntelligentPointer<CThing> PSword;            // +0x74 (vtable 0x01238C6C)
    unsigned char               InitialSwordRHS[0x18];   // +0x7c CRightHandedSet
    bool                        SwordFalling;      // +0x94
    long                        SwordFallTimer;    // +0x98
    CIntelligentPointer<CThing> PEmitter;          // +0x9c
    C3DVector                   SwordFallPos;      // +0xa4
    C3DVector                   InitialSwordPos;   // +0xb0
    void SetSwordObject(CThing* sword);
};

void CModeJackOfBladesScream::SetSwordObject(CThing* sword)
{
    PSword = CIntelligentPointer<CThing>(sword);
    InitialSwordPos = PSword->GetPosition();
}

}
