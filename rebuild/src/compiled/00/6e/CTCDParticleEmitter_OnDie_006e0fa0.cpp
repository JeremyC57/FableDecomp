// CTCDParticleEmitter::OnDie  @ 006e0fa0

class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct Sub14 {
};

struct Target4 {
    void Notify();  // 0x6e0f10
};

class CTCDParticleEmitter { public:
    int      m0;     // +0x00
    Target4* ParentThing;     // +0x04
    int      m8;     // +0x08
    int      mC;     // +0x0C
    int      ParticleTypeID;    // +0x10
    Sub14    m14;    // +0x14
    void OnDie();
};

void CTCDParticleEmitter::OnDie()
{
    if (((CBaseIntelligentPointer*)&(this->m14))->GetPItem())
        this->ParentThing->Notify();
}