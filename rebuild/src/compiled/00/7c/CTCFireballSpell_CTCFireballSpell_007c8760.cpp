// CTCFireballSpell::~CTCFireballSpell() -- kill a live build-up effect, then destroy the attachment name, the three thing pointers, the spell def pointer and the will-button timer before ~CTCBase
// Leaf: ??1CTCFireballSpell
//
// Retail 0x007C8760, 144 bytes (tail `jmp` to the base dtor). Ego_r.pdb `??1CTCFireballSpell@@UAE@XZ`
// (implied by the vtable 0x0126E964 store). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (three times), CThing::Kill(bool) @ 0x004C9B80, ~CCharString @ 0x0099EAE0, ~CBaseIntelligentPointer @
// 0x00A01C10 (four times), free @ 0x00BFEA14 (the timer's vector<float> storage), CTCBase::~CTCBase @
// 0x00686830 (manifest "~CChunkedFileChunk", propagated); the spell def's virtual slot 1 on its last
// release.
// Layout: Ego_r.pdb CTCFireballSpell with retail 12-byte vectors: Timer (CWillButtonHelperTimer) +0xc
// (its PCreature +0x14, WillPowerUpAccumulatedTimings +0x24), PDef +0x40, InitLoopingSound +0x44,
// FireballBuildUpEffect +0x48, ImmediateCastMode +0x50, OneAnimCastMode +0x51, PTarget +0x52 and
// PItemRemoved +0x5a (CIntelligentPointer is declared under pack(1)), WeaponState +0x64, LeftHandAnim
// +0x68, FireballAttachmentName +0x6c, StartEffect +0x70, MaxLevelAllowableByStamina +0x74,
// ReleaseDelayFrameCount +0x78, SpellFinished +0x7c.
// Shape: the dtor body is the inline IsThingAlive + Kill(true) of the landed CTCBoss::ClearEffects
// (0x007AEF60); members die in reverse declaration order: the CIntelligentPointer members have no
// destructor of their own (the base dtor is called directly), the CDefPointer dtor is `if (Object)
// { Object->Release(); Object = 0; }`, the timer's implicit dtor frees its vector then its pointer.
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                                  // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                            // 0x004C9B80
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                                          // +4
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
};
#pragma pack(pop)

inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

class CCharString { public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

template <class T> struct CRetailVector {                   // STLport vector (12 bytes) over malloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector() { if (_M_start) free(_M_start); }
};

class CSpecialAbilitiesFireballSpellDef { public:
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    void Release() { if (--RefCount == 0) DestroySelf(); }
};

template<class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }
};

struct CWillButtonHelperTimer {                             // retail 0x34 bytes
    long                        ButtonHeldDownTimer;        // +0x00
    long                        WillAccumulatedLevel;       // +0x04
    CIntelligentPointer<CThing> PCreature;                  // +0x08
    long                        MyAbility;                  // +0x10
    long                        CurrentState;               // +0x14
    CRetailVector<float>        WillPowerUpAccumulatedTimings;   // +0x18
    float                       MaxNumberLevels;            // +0x24
    float                       CurMaxLevels;               // +0x28
    bool                        WillIncrementedFlag;        // +0x2c
    bool                        UpdateAccLevel;             // +0x2d
    bool                        IsAICasting;                // +0x2e
    bool                        IsAIHoldingDown;            // +0x2f
    long                        LevelToCast;                // +0x30
};

class CTCBase { public:
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCFireballSpell : public CTCBase { public:                         // vtable 0x0126E964
    CWillButtonHelperTimer                         Timer;                    // +0x0c
    CDefPointer<CSpecialAbilitiesFireballSpellDef> PDef;                     // +0x40
    unsigned long                                  InitLoopingSound;         // +0x44
    CIntelligentPointer<CThing>                    FireballBuildUpEffect;    // +0x48
    bool                                           ImmediateCastMode;        // +0x50
    bool                                           OneAnimCastMode;          // +0x51
    CIntelligentPointer<CThing>                    PTarget;                  // +0x52
    CIntelligentPointer<CThing>                    PItemRemoved;             // +0x5a
    long                                           WeaponState;              // +0x64
    bool                                           LeftHandAnim;             // +0x68
    CCharString                                    FireballAttachmentName;   // +0x6c
    bool                                           StartEffect;              // +0x70
    long                                           MaxLevelAllowableByStamina;   // +0x74
    long                                           ReleaseDelayFrameCount;   // +0x78
    bool                                           SpellFinished;            // +0x7c
    virtual ~CTCFireballSpell();
};

CTCFireballSpell::~CTCFireballSpell()
{
    if (IsThingAlive(FireballBuildUpEffect))
        FireballBuildUpEffect.GetPItem()->Kill(true);
}