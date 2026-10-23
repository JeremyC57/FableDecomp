// CTCTurncoat::CTCTurncoat(CThing* parent) -- constructor: CTCBase, cleared turncoat state and effect pointers, empty stage-effect vector, the turncoat def, then the resistance-recovery delay (frames) and rate (per frame) from the def
// Leaf: ??0CTCTurncoat
// Retail: Fable.exe @ 0x007C78C0, 153 bytes (`ret 4`; vtable 0x0126E6F4 = CTCTurncoat per rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (x2, then the
//          CIntelligentPointer<CThing> vtable 0x01238C6C), ::SetPItem 0x00A01B90 (TurncoatedEffect built from a null
//          pointer); CDefClassBase::GetSubDef<CTurncoatDef>(CDefPointer*) 0x007C8000 on the parent's def class (+0x70;
//          template label propagated); _ftol 0x00BFEA70. Global: g_framesPerSecond (long) 0x01375550.
// Layout: engine/CTCTurncoat.h (Ego_r.pdb, packed) up to +0x3c; the retail stage-effect vector is 12 bytes, so
//         CreatureTurncoatResistance sits at +0x48 (PDB +0x4c). CTurncoatDef: TimeDelayBeforeResistanceRecoverySecs +0x38,
//         ResistanceRecoveryPerSec +0x3c.
// Shape: init-list members in declaration order (one ebp zero, ebx = 1 shared by TurncoatOverride and the body's
//        WillTurncoatTimer = 1 after GetSubDef); `secs * fps` truncates through _ftol, `perSec / fps` is `fdivr`.
class CThing;

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();
    long m_count;
};
struct CTurncoatDef : CDefPointee {
    unsigned char _pad_0x08[0x30];
    float TimeDelayBeforeResistanceRecoverySecs;        // +0x38
    float ResistanceRecoveryPerSec;                     // +0x3c
};
struct CDefPointer {                                    // CDefPointer<const CTurncoatDef>
    CTurncoatDef* Object;
    CDefPointer() : Object(0) {}
    CTurncoatDef* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef_007c8000(CDefPointer* out);                   // 0x007C8000
};
struct CThingDefs {
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;                       // +0x70
};

struct CBaseIntelligentPointer {                        // 8 bytes: vptr + item
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

template <class T> struct SRetailAllocProxy {
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

extern long g_framesPerSecond;                          // 0x01375550

struct CTCBase {
    CTCBase(CThing* parent);                            // 0x00686800
    virtual void v00();
    CThingDefs* ParentThing;                            // +0x04
    unsigned char _pad_0x08[0x4];
};

struct CTCTurncoat : CTCBase {
    long                WillTurncoatTimer;              // +0x0c
    float               CurrentTurncoatedness;          // +0x10
    CDefPointer         PDef;                           // +0x14
    CIntelligentPointer TurncoatedEffect;               // +0x18
    long                TurncoatTimeLast;               // +0x20
    CIntelligentPointer TurncoatOriginator;             // +0x24
    bool                TurncoatOverride;               // +0x2c
    long                TimeDelayBeforeResistanceRecoveryFrames;   // +0x30
    long                CurrentRecoveryTimer;           // +0x34
    float               ResistanceRecoveryPerFrame;     // +0x38
    CRetailVector<CIntelligentPointer> TurncoatStageEffects;   // +0x3c
    float               CreatureTurncoatResistance;     // +0x48
    CTCTurncoat(CThing* parent);
    virtual void v00();
};

CTCTurncoat::CTCTurncoat(CThing* parent)
    : CTCBase(parent), CurrentTurncoatedness(0), TurncoatedEffect(0), TurncoatTimeLast(0), TurncoatOverride(true),
      CurrentRecoveryTimer(0), CreatureTurncoatResistance(0)
{
    ParentThing->m_pDefinition->GetSubDef_007c8000(&PDef);
    WillTurncoatTimer = 1;
    TimeDelayBeforeResistanceRecoveryFrames = (long)(g_framesPerSecond * PDef->TimeDelayBeforeResistanceRecoverySecs);
    ResistanceRecoveryPerFrame = PDef->ResistanceRecoveryPerSec / g_framesPerSecond;
}