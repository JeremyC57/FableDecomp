// CAIStateGroup_DeedReactionRoot::OnActivate() -- terminate unless the thing reacted to lives; reset the delivered flags, then either remember a virtual deed's source UID or apply the creature's pending deed log
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00960070, length 163 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE inlined, then the thing),
// CAIStateGroupBase::Terminate @ 0x008FCF40 (sets Terminated +0x10; manifest "CXMVPlaybackInfo" ctor label is a byte twin),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (type 0x4F = word 2 bit 15, `test ch,ch ; jns`),
// CTCHeroOpinionDeedLog::ApplyPendingDeedLog @ 0x00683B00.
// Shape: the deed-log component is deliberately unset when the interface is absent (retail calls through the dead key
// slot), so `T* tc; if (HasTC(k)) tc = GetTC(k); tc->Apply();` -- VC duplicates the call tail per arm here.
// `DeliveredReaction = DeliveredSpeech = false` shares the `xor al,al` with the DeedIsVirtual compare.
// Layout: Ego_r.pdb CAIStateGroup_DeedReactionRoot = retail (ThingReactingTo +0x14, DeedIsVirtual +0x24,
// LastThingOpinionSourceUID +0x28 (CThing::UniqueID +0x18, ulong64), DeliveredReaction +0x34, DeliveredSpeech +0x35).
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    T* Get() const { return (T*)GetPItem(); }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CTCBase;

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                  // +0x00
    SEntry* last;                                   // +0x04
    SEntry* LowerBound(const long* type);           // 0x004365B0
};

class CThing
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long type)    // the entry; `->component` read at the call site
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->type <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CAIBrain
{ public:
    unsigned char _pad0[0x18];
    CThing* Creature;                               // +0x18
};

struct CThingWithUID                                // CThing::UniqueID at +0x18
{
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                      // +0x18
};

class CTCHeroOpinionDeedLog
{ public:
    void ApplyPendingDeedLog();                     // 0x00683B00
};

class CAIStateGroupBase                            // a real base: a flat class with a vptr and an __int64 pads the vptr to 8
{ public:
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08 priority, name, flags
    void Terminate();                               // 0x008FCF40
};

struct CAIStateGroup_DeedReactionRoot : CAIStateGroupBase
{
    CIntelligentPointer<const CThing> ThingReactingTo;   // +0x14
    long CurrentDeedType;                           // +0x1c
    long CurrentDeedHandle;                         // +0x20
    bool DeedIsVirtual;                             // +0x24
    unsigned __int64 LastThingOpinionSourceUID;     // +0x28
    long LastOpinionSourceVirtualDeedID;            // +0x30
    bool DeliveredReaction;                         // +0x34
    bool DeliveredSpeech;                           // +0x35
    void OnActivate();
};

__forceinline bool IS_THING_ALIVE(const CIntelligentPointer<const CThing>& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

void CAIStateGroup_DeedReactionRoot::OnActivate()
{
    if (!IS_THING_ALIVE(ThingReactingTo))
    {
        Terminate();
        return;
    }
    DeliveredReaction = false;
    DeliveredSpeech = false;
    if (DeedIsVirtual)
    {
        LastThingOpinionSourceUID = ((CThingWithUID*)ThingReactingTo.GetPItem())->UniqueID;
        return;
    }
    CThing* creature = PBrain->Creature;
    CTCHeroOpinionDeedLog* log;
    if (creature->HasTC(0x4f))
        log = (CTCHeroOpinionDeedLog*)creature->FindEntry(0x4f)->component;
    log->ApplyPendingDeedLog();
}