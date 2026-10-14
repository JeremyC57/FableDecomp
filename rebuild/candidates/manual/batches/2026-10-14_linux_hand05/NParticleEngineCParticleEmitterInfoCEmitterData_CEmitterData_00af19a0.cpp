// NParticleEngine::CParticleEmitterInfo::CEmitterData::CEmitterData(unsigned long size) -- base CResource(), own vtable, four null members, the size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF19A0, length 41 bytes.
// Callees: CResource::CResource @ 0x009FBEC0. This class's vtable is 0x0129F354
// (rtti-vtables.tsv: NParticleEngine::CParticleEmitterInfo::CEmitterData). CResource spans
// +0..+0x1c. The vtable store precedes the four register-zeroed stores at +0x1c..+0x28 (an
// embedded member object with an inline constructor); the argument is assigned to +0x2c in the body.
struct CResource {
    virtual void v00();
    unsigned char _body[0x18];              // +0x04..+0x1c
    CResource();                            // 0x009FBEC0
};

struct SEmitterBuffers {                    // four raw pointers, nulled by the inline constructor
    void* m_pEmitters;                      // +0x00
    void* m_pParticles;                     // +0x04
    void* m_pTextures;                      // +0x08
    void* m_pOwner;                         // +0x0c
    SEmitterBuffers() : m_pEmitters(0), m_pParticles(0), m_pTextures(0), m_pOwner(0) { }
};

namespace NParticleEngine {
struct CParticleEmitterInfo {
    struct CEmitterData : CResource {       // vtable 0x0129F354
        SEmitterBuffers m_buffers;          // +0x1c
        unsigned long   m_size;             // +0x2c
        virtual void v00();
        CEmitterData(unsigned long size);
    };
};
}

NParticleEngine::CParticleEmitterInfo::CEmitterData::CEmitterData(unsigned long size)
    : CResource()
    , m_buffers()
{
    m_size = size;
}
