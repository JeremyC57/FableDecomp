// NCreatureMode::CModeOutOfActionBase::OnDeactivate() -- the creature comes back into action: resume its animation if it was in state 1, clear targeting flag 4, wake its physics, kill the out-of-action emitter if it is still alive, then the stand-mode deactivation
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8F30, 161 bytes (`ret`).
// Callees: CThingAnimatable::SetSuspendAnimation(bool, long) 0x00833A10; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// 0x0040F020 (inlined HasTC/GetTC, type 9 = bit 9 of +0x20); the type-9 component's flag setter 0x007EA270 (beside
// CTCTargeted::IsTargetable 0x007EA280; manifest "CObservable::AddExclusiveObserver" is propagated); the physics object's
// (+0x60) virtual slot 44 (+0xb0, wake); CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times); CThing::Kill(bool)
// 0x004C9B80; CModeStandBase::OnDeactivate 0x00861D00 (qualified base call).
// Layout (PDB CModeOutOfActionBase 0x78, byte-packed tail): CModeStandBase 0x68, RecoveryCount +0x68, Recoverable +0x6c,
// PEmitter +0x6d (CIntelligentPointer<CThing> at an odd offset: the class is compiled under pack(1)).
// Shape: HasTC + a returned GetTC whose end/hit choice is one ternary (the component lands straight in ecx); ParentThing is re-read for every use; the flag clear is the component's inline ClearFlag (`if (flags & f) SetFlags(flags ^ f)`, `this` straight in ecx); IS_THING_ALIVE as in the
// landed 0x008C92C0 (GetPItem re-called for the null test, the dead bit and the call).
class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void Wake();   // slot 44 (+0xb0)
};

struct CThingAnimatable {                  // empty here: only its member is called (thing base at offset 0)
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

class CThing : public CThingAnimatable { public:
    unsigned char   _pad00[0x10];
    unsigned char   m_state;               // +0x10 creature state byte (compared with 1)
    unsigned char   _pad11[0x0f];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    unsigned char   _pad4c[0x14];
    CPhysicsObject* PPhysics;              // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;             // +0x91 bit 0: dead
    void Kill(bool immediately);           // 0x004C9B80
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
};

struct CTCTargetedFlags {                  // the type-9 component: a flags word at +0xc with an out-of-line setter
    unsigned char _pad00[0x0c];
    unsigned long m_flags;                 // +0x0c
    void SetFlags(unsigned long flags);    // 0x007EA270
    void ClearFlag(unsigned long flag)
    {
        if (m_flags & flag)
            SetFlags(m_flags ^ flag);
    }
};

#pragma pack(push, 1)
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x60];
    void OnDeactivate();                   // 0x00861D00
};

namespace NCreatureMode {

#pragma pack(push, 1)
struct CModeOutOfActionBase : CModeStandBase {
    long                    RecoveryCount; // +0x68
    bool                    Recoverable;   // +0x6c
    CIntelligentPointer<CThing> PEmitter;      // +0x6d
    void OnDeactivate();
};
#pragma pack(pop)

void CModeOutOfActionBase::OnDeactivate()
{
    if (ParentThing->m_state == 1)
        ParentThing->SetSuspendAnimation(false, 0);
    if (ParentThing->HasTC(9)) {
        ParentThing->GetTC<CTCTargetedFlags>(9)->ClearFlag(4);
    }
    ParentThing->PPhysics->Wake();
    if (IS_THING_ALIVE(PEmitter))
        PEmitter.GetPItem()->Kill(true);
    CModeStandBase::OnDeactivate();
}

}