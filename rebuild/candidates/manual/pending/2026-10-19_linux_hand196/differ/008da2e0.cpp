// NCreatureMode::CModeScorpionKingExhausted::OnActivate() -- the scorpion king tires: stand-mode activation, clear the type-9 component's active flag, recovery countdown = the battle def's exhaustion time in frames, re-apply the weapon filter, switch its physics to mode 8, clear thing flag bit 0 at +0xbc and, when no action is running, reset the creature's current action
// Leaf: OnActivate
// Grade: DIFFER(ours v retail) -- frame 0xc vs 8: retail reuses the out CDefPointer slot for the interface key and keeps the def in ebp straight from the slot (ours copies through eax and gives the time its own slot); tried a ternary HasTC/GetTC, a block-scoped out pointer and an inline TakeBattleDef helper
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA2E0, 195 bytes (`ret`).
// Callees: CModeStandBase::OnActivate 0x008621A0 and CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0 (both qualified);
// CDefClassBase::GetSubDef(CDefPointer<const CScorpionKingBattleDef>*) 0x008C5A50 (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>"
// is a byte-twin label); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined HasTC/GetTC, type 9 = bit 9 of +0x20);
// __ftol2 0x00BFEA70; the physics object's (+0x60) virtual slot 43 (+0xac); CThingCreatureBase::IsPerformingAnyAction 0x006622A0;
// PCreature's virtual slot 28 (+0x70); the def's virtual slot 1 (delete at zero references).
// Globals: the frames-per-second long 0x01375550.
// Layout (PDB CModeScorpionKingExhausted 0x80, matches retail): CModeStandBase 0x68, RecoveryCount +0x68, Filter +0x6c,
// PCreature +0x78, bRecovering +0x7c. Thing: def +0x70, physics +0x60, flags +0xbc. The battle def's float at +0x40 is the
// exhaustion time (its PDB name is not pinned down: the def's base sits below the PDB's).
// Shape: the thing's def class is read before the out CDefPointer is zeroed; the def is copied out of it into a holder (ebp)
// read once for the time and released at the end without a null test (`dec [ebp+4] ; jne`); the component is chosen by a
// ternary (null when absent) and written unconditionally.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CDefPointeeBase {
    virtual void Destroy();                // slot 0
    virtual void Delete();                 // slot 1 (+0x04)
    long m_refCount;                       // +0x04
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};

template <class T> struct CDefHolder {     // the def taken out of the out pointer, released when the function returns
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (--Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};

struct CScorpionKingBattleDef : CDefPointeeBase {
    unsigned char _pad08[0x40 - 0x08];
    float         ExhaustedTime;           // +0x40
};

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CScorpionKingBattleDef>* out);   // 0x008C5A50
};

struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042();
    virtual void SetPhysicsMode(long mode);   // slot 43 (+0xac)
};

struct CTCActiveFlag {                     // the type-9 component (as CModeOutOfActionBase::OnDeactivate 0x008D8F30)
    unsigned char _pad00[0x10];
    bool          m_active;                // +0x10
};

struct CThing {
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    unsigned char   _pad4c[0x14];
    CPhysicsObject* PPhysics;              // +0x60
    unsigned char   _pad64[0x0c];
    CDefClassBase*  PDef;                  // +0x70
    unsigned char   _pad74[0xbc - 0x74];
    unsigned char   m_flagsBC;             // +0xbc
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction();          // 0x006622A0
};

struct CCreatureController {               // what PCreature's slot 28 is called on
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void ResetCurrentAction(bool immediately);   // slot 28 (+0x70)
};

extern long g_framesPerSecond;             // 0x01375550

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
    void OnActivate();                     // 0x0073AAC0
};

struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
    void OnActivate();                     // 0x008621A0
};

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    void*         Group;                   // +4 (CCharString)
    unsigned long GroupCRC;                // +8
};

namespace NCreatureMode {

struct CModeScorpionKingExhausted : CModeStandBase {
    long                  RecoveryCount;   // +0x68
    CAnimFilterOfGroup    Filter;          // +0x6c
    CCreatureController*  PCreature;       // +0x78
    bool                  bRecovering;     // +0x7c
    void OnActivate();
};

void CModeScorpionKingExhausted::OnActivate()
{
    CModeStandBase::OnActivate();
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<CScorpionKingBattleDef> out;
    thingDef->GetSubDef(&out);
    CDefHolder<CScorpionKingBattleDef> def(out.Object);
    float exhaustedTime = def->ExhaustedTime;
    CTCActiveFlag* flag = 0;
    ParentThing->GetInterface(&flag, 9);
    flag->m_active = false;
    RecoveryCount = (long)(exhaustedTime * g_framesPerSecond);
    CModeBase::OnActivate();
    ParentThing->PPhysics->SetPhysicsMode(8);
    ParentThing->m_flagsBC &= ~1;
    if (!ParentThing->IsPerformingAnyAction())
        PCreature->ResetCurrentAction(false);
}

}
