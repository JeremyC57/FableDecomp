// CCreatureAction_SendMessage::CCreatureAction_SendMessage(const CCreatureAction_SendMessage&) -- copy constructor: base CCreatureActionBase(other), own vtable, the counted message def (add-ref), Position, PReceiver and Extra
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982750, length 134 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (the inline
// CIntelligentPointer copy constructor of the landed 0x0084C070 shape, this instantiation's vtable 0x0123E7D8). Own vtable 0x0129947C.
// Shape: the memberwise copy in declaration order: the def pointer's inline copy constructor (null test scheduled around the store,
// then the inline AddRef `inc dword ptr [eax+4]` on the def's reference count), the three Position words through eax/ecx/edx with no base
// register (separate scalar members: a C3DVector member is copied through `lea` base pointers),
// the intelligent pointer (its address in ebx), the Extra word. `ret 4`.
// No engine header for this class; modelled locally (retail CCreatureActionBase is 0xa8 bytes, see PerformAction 0x008491E0).
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

class CThingCreatureBase;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {         // CIntelligentPointer<CThing>, vtable 0x0123E7D8
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CMessageEventDef { public:                   // a counted definition: reference count at +4
    void* __vftable;
    long m_refCount;                        // +0x04
    void AddRef() { ++m_refCount; }
};
struct CDefPointer {                        // counted def pointer: add-ref on copy
    CMessageEventDef* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];          // retail sizeof 0xa8
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_SendMessage : public CCreatureActionBase { public:    // vtable 0x0129947C; retail sizeof 0xc4
    CDefPointer MessageDef;                 // +0xa8
    float PositionX;                        // +0xac C3DVector Position, copied word by word (a struct member copies through `lea`)
    float PositionY;                        // +0xb0
    float PositionZ;                        // +0xb4
    CIntelligentPointer<CThingCreatureBase> PReceiver;          // +0xb8 CIntelligentPointer<CThing>
    long Extra;                             // +0xc0
    virtual void v00();
    CCreatureAction_SendMessage(const CCreatureAction_SendMessage& other);
};

CCreatureAction_SendMessage::CCreatureAction_SendMessage(const CCreatureAction_SendMessage& other)
    : CCreatureActionBase(other)
    , MessageDef(other.MessageDef)
    , PositionX(other.PositionX)
    , PositionY(other.PositionY)
    , PositionZ(other.PositionZ)
    , PReceiver(other.PReceiver)
    , Extra(other.Extra)
{
}