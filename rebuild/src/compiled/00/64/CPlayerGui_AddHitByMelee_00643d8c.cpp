// CPlayerGui::AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos) -- spawn the GUI def's "smack" particle effect at the hit position
// Leaf: ?AddHitByMelee@CPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643D8C, 89 bytes, `ret 0Ch`. Size-optimised NPlayerGui TU (frame pointer,
// `leave`, fldz/fld1 + fstp constant stores, no function alignment).
// Identity: Ego_r.pdb `?AddHitByMelee@CPlayerGui@@QAEXAAVCThing@@0ABVC3DVector@@@Z`; reached only through the forwarder at
// 0x00437990 (`mov ecx,[ecx+150h] ; jmp`, CGameUI's AddHitByMelee onto its CPlayerGui), called from 0x00667F48 with the game
// UI 0x013B8790. Catalog label "InitParticleEmitter" is descriptive.
// Callees: PeekGuiDef 0x00438570 (__fastcall, as in the landed DrawStatBox 0x00645CE9), CRightHandedSet(const C3DVector& up,
// const C3DVector& forward) 0x00487050, NParticleEngine::CParticleEmitterManager::Get() 0x00AEAAF0, CreateEmitter(long,
// const C3DVector&, bool, const CRightHandedSet&) 0x00AEB6D0 (result dropped).
// Layout: CPlayerGuiDef SmackEffect at +0x48C (Ego_r.pdb +0x4BC; twelve 16-byte PDB vectors before it are 12 bytes in
// retail). `this`, attacker and victim are unused.
// Shape: the def is fetched first (named local); CreateEmitter's arguments evaluate right to left, so the orientation
// temporaries come first: forward (0,1,0), then up (0,0,1), then the set; Get() runs after the pushes.
#pragma optimize("s", on)
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};

class CPlayerGuiDef { public:
    unsigned char _pad[0x48c];
    long          SmackEffect;                                        // +0x48C (retail)
};
extern const CPlayerGuiDef* __fastcall PeekGuiDef();                  // 0x00438570

namespace NParticleEngine {
class CParticleEmitter;
class CParticleEmitterManager {
public:
    static CParticleEmitterManager* Get();                            // 0x00AEAAF0
    CParticleEmitter* CreateEmitter(long effect, const C3DVector& pos, bool start,
                                    const CRightHandedSet& orientation);   // 0x00AEB6D0
};
}

class CThing;

class CPlayerGui {
public:
    void AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos);
};

void CPlayerGui::AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos)
{
    const CPlayerGuiDef* def = PeekGuiDef();
    NParticleEngine::CParticleEmitterManager::Get()->CreateEmitter(def->SmackEffect, pos, false,
        CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
}