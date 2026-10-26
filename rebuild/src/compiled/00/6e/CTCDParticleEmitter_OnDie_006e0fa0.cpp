// CTCDParticleEmitter::OnDie  @ 006e0fa0

struct Sub14 {
    int Check();  // 0xa01b50, returns int used as bool
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
    if (this->m14.Check())
        this->ParentThing->Notify();
}