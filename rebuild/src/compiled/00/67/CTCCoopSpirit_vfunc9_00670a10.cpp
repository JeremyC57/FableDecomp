// CTCCoopSpirit::vfunc_9() -- per-frame update: re-create the appearance particle when its emitter is gone, update movement, jostling and gravity, kill the spirit thing when its +0xb4 value has run out, then update the score
//
// Retail 0x00670A10, 143 bytes (plain `ret`). Vtable 0x0125B264 (CTCCoopSpirit) slot 9.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CDefStringTable::GetString(long)
// @ 0x009D49B0 on the global table @ 0x013CA828 (CCharString by value), CTCCoopSpirit::
// SetAppearanceParticle(const CCharString&) @ 0x006705F0, ~CCharString @ 0x0099EAE0, UpdateMovement
// @ 0x00670820, UpdateLocalJostling @ 0x0066FB80, the gravity update @ 0x00670470 (manifest
// "CThingCreatureBase_UpdateGravity"), CThing::Kill(bool) @ 0x004C9B80, UpdateScore @ 0x00670710.
// Layout: Ego_r.pdb CTCCoopSpirit: PCoopSpiritDef +0x28 (the def's appearance-particle CDefString at
// +0x30), PParticleEmitter +0x2c; the parent thing's float at +0xb4.
// Shape: `if (!IsThingAlive(PParticleEmitter))` over the intelligent pointer (GetPItem for the null test
// and again for the dead bit); the name is the by-value temporary of an inline CDefString::GetString()
// destroyed after the call; `value <= 0.0f` gives `test ah,41h ; jp`.
class CCharString { public:
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};

struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828

class CDefString { public:
    long TablePos;
    CCharString GetString() const { return g_DefStringTable.GetString(TablePos); }
};

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                     // +0x91 (bit 0 = dead)
    unsigned char _pad92[0xb4 - 0x92];
    float         m_lifeRemaining;                  // +0xb4
    void Kill(bool removeNow);                      // 0x004C9B80
};

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

inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_stateFlags & 1);
}

class CCoopSpiritDef { public:
    unsigned char _pad00[0x30];
    CDefString    AppearanceParticle;               // +0x30
};

class C3DVector { public: float X, Y, Z; };

class CTCCoopSpirit { public:
    virtual void v00();
    CThing*         ParentThing;                    // +0x04 (CTCBase)
    unsigned char   _pad08[4];
    long            NoMovementTimer;                // +0x0c
    long            Score;                          // +0x10
    long            OffscreenCounter;               // +0x14
    C3DVector       MovementVector;                 // +0x18
    void*           Master;                         // +0x24
    CCoopSpiritDef* PCoopSpiritDef;                 // +0x28 CDefPointer<const CCoopSpiritDef>
    CIntelligentPointer<CThing> PParticleEmitter;       // +0x2c CIntelligentPointer<CThing>

    void SetAppearanceParticle(const CCharString& name);   // 0x006705F0
    void UpdateMovement();                          // 0x00670820
    void UpdateLocalJostling();                     // 0x0066FB80
    void UpdateGravity();                           // 0x00670470
    void UpdateScore();                             // 0x00670710
    void vfunc_9();
};

void CTCCoopSpirit::vfunc_9()
{
    if (!IsThingAlive(PParticleEmitter))
        SetAppearanceParticle(PCoopSpiritDef->AppearanceParticle.GetString());
    UpdateMovement();
    UpdateLocalJostling();
    UpdateGravity();
    if (ParentThing && ParentThing->m_lifeRemaining <= 0.0f)
        ParentThing->Kill(true);
    UpdateScore();
}