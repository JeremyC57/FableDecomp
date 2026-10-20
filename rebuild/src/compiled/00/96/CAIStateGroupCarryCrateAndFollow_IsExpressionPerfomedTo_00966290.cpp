// CAIStateGroup_CarryCrateAndFollow::IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointer<CThing>& performer) const -- look for an "expression performed" message naming this expression; on a hit hand back its sender and consume the message
// Retail: Fable.exe (TLC, VC7.1) @ 0x00966290, 256 bytes (`ret 8`).
// Ego_r.pdb: ?IsExpressionPerfomedTo@CAIStateGroup_CarryCrateAndFollow@@ABE_NABVCCharString@@AAV?$CIntelligentPointer@VCThing@@@@@Z.
// Callees: CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x42), the message's virtual slot 1 (+4, type), the
// payload's virtual slot 10 (+0x28, CDefString by hidden pointer), CDefString::operator==(const CCharString&) 0x004B7980
// (its argument pushed before the virtual call), the payload's destroy (slot 1), the converting CIntelligentPointer<CThing>
// temporary (vtable 0x01238C6C: GetPItem 0x00A01B50, ctor 0x00A01B10, SetPItem 0x00A01B90 x2, dtor 0x00A01C10),
// the receiver's RemoveMessage(CMessage*) 0x008112D0.
// Shape: the landed AI state-group model; a list walk re-reading the sentinel each pass; a counted copy of the message
// payload (inline add-ref / release) for the expression compare; on a match `performer = CIntelligentPointer<CThing>(sender)`
// then the message is removed.
// Leaf: IsExpressionPerfomedTo@CAIStateGroup_CarryCrateAndFollow
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


struct CCharString;
struct CDefString {
    long m_index;
    bool operator==(const CCharString& other) const;   // 0x004B7980
};

struct CExpressionPayload {                     // intrusively counted (count +4, destroy = slot 1)
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+4)
    long m_count;                               // +0x04
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetExpression() const;   // slot 10 (+0x28)
};
template <class T> struct CIVCountedPointer {
    T* Data;
    CIVCountedPointer(const CIVCountedPointer& o) : Data(o.Data) { if (Data) ++Data->m_count; }
    ~CIVCountedPointer() { if (--Data->m_count == 0) Data->Destroy(); }
    T* operator->() const { return Data; }
};

struct CMessage {
    virtual void v00();
    virtual long GetType() const;               // slot 1 (+4); 3 = expression performed
    long _04;
    CBaseIntelligentPointer Sender;             // +0x08
    CIVCountedPointer<CExpressionPayload> Payload;   // +0x10
};

struct CMessageList {                           // STLport list<CMessage*>: the sentinel node
    struct SNode { SNode* _M_next; SNode* _M_prev; CMessage* _M_data; };
    SNode* _M_node;
};
struct CTCMessageReceiver {                     // the interface-0x42 component
    unsigned char _pad[0x0c];
    CMessageList Messages;                      // +0x0c
    void RemoveMessage(CMessage* message);      // 0x008112D0 (manifest "remove_if<..CIsType>" is propagated)
};

struct CIntelligentPointerThingC : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThingC(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThingC& operator=(const CIntelligentPointerThingC& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    bool IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointerThingC& performer) const;
};

bool CAIStateGroup_CarryCrateAndFollow::IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointerThingC& performer) const
{
    CTCMessageReceiver* receiver;
    if (PBrain->PCreature->GetInterface(&receiver, 0x42)) {
        for (CMessageList::SNode* node = receiver->Messages._M_node->_M_next; node != receiver->Messages._M_node; node = node->_M_next) {
            CMessage* message = node->_M_data;
            if (message->GetType() == 3) {
                bool performed;
                {
                    CIVCountedPointer<CExpressionPayload> payload = message->Payload;
                    performed = payload->GetExpression() == expression;
                }
                if (performed) {
                    performer = CIntelligentPointerThingC(message->Sender.GetPItem());
                    receiver->RemoveMessage(message);
                    return true;
                }
            }
        }
    }
    return false;
}