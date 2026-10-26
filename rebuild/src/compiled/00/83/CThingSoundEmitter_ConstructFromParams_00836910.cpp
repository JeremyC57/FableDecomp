// CThingSoundEmitter::ConstructFromParams(const C3DVector&, const CThing*, bool) -- base thing construction (def 0, player 4, the map under the position), then the attached thing (intelligent pointer + flag), the keep-playing flag and the position
// Leaf: ?ConstructFromParams@CThingSoundEmitter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836910, 219 bytes, `ret 0Ch`, bool in al.
// Identity: ?ConstructFromParams@CThingSoundEmitter@@IAE_NABVC3DVector@@PBVCThing@@_N@Z (Ego_r.pdb / FableWin.pdb); its
//   only caller is CThingSoundEmitter::Create 0x008369F0. The manifest label "Sound_SoundEmitter_Constructor" is descriptive.
// Callees: CThing::ConstructFromParams(long, long, long, bool) 0x004CA010 (landed); CWorldMap::GetMapNumberAt(const
//   C3DVector&) const = slot 16 (+0x40) of the thing manager's world map (g_pThingManager 0x013B8A1C, +0x28 = PDB
//   WorldMap +0x34 less the 0xC smaller retail CClassFactory base); CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01237F1C).
// Layout: retail CThingSoundEmitter (landed constructor 0x008367B0, `#pragma pack(1)`): sound object +0x98,
//   AttachedToThing +0x9c, PAttachedThing +0x9d, ContinuePlayingAfterAttachedThingDeath +0xa5, Pos +0xa6.
// Shape: `if (base) { ...; return true; } return false;` puts the `xor al,al` exit last; the `false` argument is pushed before the map query (arguments right to left); each branch assigns the
//   intelligent pointer from a temporary built from the raw pointer (the landed 0x00830360 model) -- VC hoists the
//   shared temporary construction above the null test; the flag store sinks below the temporary's destructor.
class C3DVector { public: float x, y, z; };
class CThing;

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
};

class CThingManager { public:
    unsigned char _pad00[0x28];
    CWorldMap*    WorldMap;                              // +0x28 (PDB +0x34)
};
extern CThingManager* g_pThingManager;                   // 0x013B8A1C

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                       // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    ~CBaseIntelligentPointer();                          // 0x00A01C10
    CThing* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};

struct CThingPointer : CBaseIntelligentPointer {         // CIntelligentPointer<CThing>, vtable 0x01237F1C
    virtual void v00();
    CThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CThingPointer& operator=(const CThingPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing {
public:
    virtual void v000();
    unsigned char _pad04[0x94];
protected:
    bool ConstructFromParams(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA010
};

#pragma pack(push, 1)
class CThingSoundEmitter : public CThing {
public:
    void*          m_pSoundPlayer;                       // +0x98 (retail-only member, not in Ego_r.pdb)
    bool           AttachedToThing;                      // +0x9c
    CThingPointer  PAttachedThing;                       // +0x9d
    bool           ContinuePlayingAfterAttachedThingDeath;   // +0xa5
    C3DVector      Pos;                                  // +0xa6
protected:
    bool ConstructFromParams(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying);
};
#pragma pack(pop)

bool CThingSoundEmitter::ConstructFromParams(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying)
{
    if (CThing::ConstructFromParams(0, 4, g_pThingManager->WorldMap->GetMapNumberAt(pos), false)) {
        if (attachedThing) {
            PAttachedThing = (CThing*)attachedThing;
            AttachedToThing = true;
        } else {
            PAttachedThing = 0;
            AttachedToThing = false;
        }
        ContinuePlayingAfterAttachedThingDeath = continuePlaying;
        Pos = pos;
        return true;
    }
    return false;
}