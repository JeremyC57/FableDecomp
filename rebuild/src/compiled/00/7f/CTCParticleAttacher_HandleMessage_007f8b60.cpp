// CTCParticleAttacher::HandleMessage(CTCMessage*) -- on a type-5 message: forward it to every attached particle, clear the list and attach afresh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8B60, length 67 bytes.
// Callees: the message's virtual GetType() (slot 1), for_each<CIntelligentPointer<CThing>*, CKillParticle> @ 0x007F8760 (SGI,
// __fastcall under /Gr: the hidden functor result in ecx, first in edx, last and the functor on the stack; its body kills every
// live particle with CThing::Kill(false); manifest label "_Destroy_range" is propagated), vector::erase(first, last) @ 0x004AC0F0 (the inlined
// clear()), CTCParticleAttacher::AttachParticles() @ 0x007F8890.
// Retail CTCParticleAttacher (PDB): PDef +0x0c, ParticleList (std::vector<CIntelligentPointer<CThing> >) +0x10.
// Shape: the functor is an EMPTY class with a user constructor (hidden-pointer return): its temporary and the returned copy
// both live in the dead message slot, so retail "loads the message" (`mov ecx,[esp+8] ; push ecx`) only to copy the empty
// temporary; the message itself is used once and never enregistered. ParticleList's address in esi for the erase.
struct CTCMessage {
    virtual void v00();
    virtual long GetType() const;                   // slot 1 (+0x04)
};

struct CParticlePointer { void* __vftable; void* m_pItem; };   // CIntelligentPointer<CThing>, 8 bytes

struct CKillParticle {                              // empty functor: kills each live particle (Kill(false) @ 0x004C9B80)
    CKillParticle() {}
};

CKillParticle __fastcall for_each(CParticlePointer* first, CParticlePointer* last, CKillParticle f);   // 0x007F8760

struct CParticleVector {
    CParticlePointer* m_pFirst;
    CParticlePointer* m_pLast;
    CParticlePointer* m_pEnd;
    CParticlePointer* erase(CParticlePointer* first, CParticlePointer* last);   // 0x004AC0F0
    void clear() { erase(m_pFirst, m_pLast); }
};

struct CTCBase_007f8b60 {
    virtual void v00();
    unsigned char _pad04[8];
};

struct CTCParticleAttacher : CTCBase_007f8b60 {
    const void*     PDef;                           // +0x0c
    CParticleVector ParticleList;                   // +0x10
    void AttachParticles();                         // 0x007F8890
    virtual void HandleMessage(CTCMessage* msg);
};

void CTCParticleAttacher::HandleMessage(CTCMessage* msg)
{
    if (msg->GetType() == 5) {
        for_each(ParticleList.m_pFirst, ParticleList.m_pLast, CKillParticle());
        ParticleList.clear();
        AttachParticles();
    }
}