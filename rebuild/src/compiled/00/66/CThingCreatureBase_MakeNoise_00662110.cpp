// CThingCreatureBase::MakeNoise(const CCharString& noiseDefName) -- spawn a CThingNoise of the named def at the creature's position and hand it the creature
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662110, 74 bytes (+ int3 padding).
// Callees: CThing::PeekDefinitionManager @ 0x004C7990 (reads the main game component global's +0x20; propagated
//          label "CViewBase::GetDisplayEngine"), CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//          CThing::GetPos @ 0x004C73D0 (`[this+60h] + 0Ch`), CThingNoise::Create(long def, const C3DVector& pos,
//          float radius) @ 0x008385B0 (static __fastcall: def in ecx, position in edx, radius on the stack),
//          CThingNoise::PassToNoiseComponent(CThing*) @ 0x008386E0 (landed; propagated "OnActionUse" label).
// Layout: the thing's dead flag is bit 0 of the byte at +0x91 (as in the landed IS_THING_ALIVE family).
// Shape: arguments right to left: the -1.0f radius is pushed before GetPos; the def index lives in edi.
class CCharString;
class C3DVector;

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;    // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    const C3DVector& GetPos() const;                 // 0x004C73D0
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
};

class CThingNoise : public CThing { public:
    static CThingNoise* __fastcall Create(long defIndex, const C3DVector& pos, float radius);   // 0x008385B0
    void PassToNoiseComponent(CThing* thing);       // 0x008386E0
};

class CThingCreatureBase : public CThing { public:
    void MakeNoise(const CCharString& noiseDefName);
};

void CThingCreatureBase::MakeNoise(const CCharString& noiseDefName)
{
    long defIndex = PeekDefinitionManager().GetDefGlobalIndexFromName(noiseDefName);
    if (defIndex) {
        CThingNoise* noise = CThingNoise::Create(defIndex, GetPos(), -1.0f);
        if (noise && !noise->m_dead)
            noise->PassToNoiseComponent(this);
    }
}