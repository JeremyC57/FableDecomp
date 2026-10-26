// CThingSoundEmitter::OnCreate() -- runs the CThing creation step, then asks the game state for a sound object at the emitter's position; true only when both succeed
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836740, 52 bytes, plain `ret`.
// Identity: ?OnCreate@CThingSoundEmitter@@MAE_NXZ (FableWin.pdb, the TU's ConstructFromParams/Create/OnCreate block); the
//   result lands in the retail-only sound-object member +0x98 that the constructor 0x008367B0 zeroes and every other
//   CThingSoundEmitter method (PlaySample 0x00836810 ..) dispatches through. The manifest label
//   "CThing_OnCreateWithInterface" is descriptive and wrong.
// Callees: CThing::OnCreate() 0x004C9560 (qualified base call); slot 4 (+0x10) of the game state 0x013B8394, given the
//   position member Pos (+0xa6) by reference.
// Layout: retail CThingSoundEmitter (the landed constructor 0x008367B0 model, `#pragma pack(1)`): sound object +0x98,
//   AttachedToThing +0x9c, PAttachedThing +0x9d, ContinuePlayingAfterAttachedThingDeath +0xa5, Pos +0xa6.
// Shape: `if (base && global) { store; if (stored) return true; } return false;` -- one `xor al,al` exit at the end; the new sound object is
//   stored before its null test (`test eax,eax ; mov [esi+98h],eax ; je`).
class C3DVector { public: float x, y, z; };
struct CSoundPlayer;

struct CGameState {                                      // 0x013B8394
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual CSoundPlayer* CreateSoundPlayer(const C3DVector& pos);   // slot 4 (+0x10)
};
extern CGameState* g_pGameState;                         // 0x013B8394

class CThing { public:
    virtual void v000();
    unsigned char _pad04[0x94];
    bool OnCreate();                                     // 0x004C9560
};

#pragma pack(push, 1)
class CThingSoundEmitter : public CThing { public:
    CSoundPlayer*  m_pSoundPlayer;                       // +0x98 (retail-only member, not in Ego_r.pdb)
    bool           AttachedToThing;                      // +0x9c
    unsigned char  PAttachedThing[8];                    // +0x9d CIntelligentPointer<CThing>
    bool           ContinuePlayingAfterAttachedThingDeath;   // +0xa5
    C3DVector      Pos;                                  // +0xa6
    bool OnCreate();
};
#pragma pack(pop)

bool CThingSoundEmitter::OnCreate()
{
    if (CThing::OnCreate() && g_pGameState) {
        m_pSoundPlayer = g_pGameState->CreateSoundPlayer(Pos);
        if (m_pSoundPlayer)
            return true;
    }
    return false;
}