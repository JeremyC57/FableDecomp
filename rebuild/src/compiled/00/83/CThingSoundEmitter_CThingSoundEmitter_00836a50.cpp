// CThingSoundEmitter::~CThingSoundEmitter() -- release the emitter's sound object through the sound system, clear the attached-thing intelligent pointer, then the member and CThing base destructors
// Leaf: ??1CThingSoundEmitter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836A50, length 128 bytes (plain `ret`). Vtables: CThingSoundEmitter
// 0x01272C24 (stored at entry), CIntelligentPointer<CThing> 0x01237F1C (the temporary's).
// Callees: slot 5 (+0x14) of the sound system singleton 0x013B8394 (releases the sound object),
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::SetPItem 0x00A01B90, ::GetPItem 0x00A01B50,
// ::~CBaseIntelligentPointer 0x00A01C10, CThing::~CThing 0x004C8310.
// Owner note (audio layer): this is the CThingSoundEmitter thing class (true class, vtable 0x01272C24 verified by
// rtti-vtables.tsv); retail layout (Ego_r.pdb is packed here and lacks the +0x98 sound object, as recorded by the
// landed ctor 0x008367B0): sound object +0x98, AttachedToThing +0x9c, PAttachedThing (CIntelligentPointer<CThing>,
// 8 bytes) +0x9d (unaligned: `#pragma pack(1)`), ContinuePlayingAfterAttachedThingDeath +0xa5, Pos +0xa6.
// Shape: the clearing assignment `PAttachedThing = CIntelligentPointer<CThing>(0)` written out: a named cleared
// pointer (inline ctor stores the derived vtable after the base ctor; implicit dtor calls the base dtor), its
// GetPItem result in a named local, then the member's SetPItem -- only the named item local forms the member
// address (`lea edi,[esi+9Dh]`) after the GetPItem call as retail does; edi is reused for the member's dtor.
struct CSoundObject;

struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void ReleaseSoundObject(CSoundObject* sound);   // slot 5 (+0x14)
};
extern CSoundSystem* g_pSoundSystem;                     // 0x013B8394

class CThing { public:
    virtual ~CThing();                                   // 0x004C8310
    unsigned char _pad04[0x94];
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                       // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    ~CBaseIntelligentPointer();                          // 0x00A01C10
    CThing* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public: float x, y, z; };

class CThingSoundEmitter : public CThing { public:
    CSoundObject*            m_pSoundObject;             // +0x98
    bool                     AttachedToThing;            // +0x9c
    CIntelligentPointerThing PAttachedThing;             // +0x9d
    bool                     ContinuePlayingAfterAttachedThingDeath;   // +0xa5
    C3DVector                Pos;                        // +0xa6
    ~CThingSoundEmitter();
};
#pragma pack(pop)

CThingSoundEmitter::~CThingSoundEmitter()
{
    if (m_pSoundObject)
        g_pSoundSystem->ReleaseSoundObject(m_pSoundObject);
    CIntelligentPointerThing cleared(0);
    CThing* item = cleared.GetPItem();
    PAttachedThing.SetPItem(item);
}