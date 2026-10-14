// CSoundReceiverLH::CSoundReceiverLH(void* owner) -- base CBase(), own vtable, embedded receiver state (unit volume), an up vector (0,1,0), the owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAD020, length 50 bytes.
// Callees: CBase::CBase @ 0x0099A2D0. This class's vtable is 0x0129E76C (rtti-vtables.tsv:
// CSoundReceiverLH). Members +0x04..+0x0c are left uninitialised. The vtable store precedes the
// state stores, so they belong to embedded members with inline constructors; the retail store
// order (+0x10, +0x14, +0x18=1.0, +0x1c=0, +0x24=0, +0x20=1.0) is two objects: a three-member
// state {sound, channel, volume 1.0f} at +0x10 and a C3DVector(0, 1, 0) at +0x1c -- one flat
// object groups the two 1.0f stores together. The 1.0f values travel in an integer register
// (0x3F800000 in ecx) as VC7.1 emits float-constant stores; the owner is assigned in the body.
struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2D0
};

struct C3DVector {
    float X;                                // +0x00
    float Y;                                // +0x04
    float Z;                                // +0x08
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct SReceiverState {                     // inline constructor: null handles, unit volume
    void* m_pSound;                         // +0x00
    void* m_pChannel;                       // +0x04
    float m_volume;                         // +0x08
    SReceiverState() : m_pSound(0), m_pChannel(0), m_volume(1.0f) { }
};

struct CSoundReceiverLH : CBase {           // vtable 0x0129E76C
    unsigned char  _pad[0x0c];              // +0x04..+0x10 (not initialised here)
    SReceiverState m_state;                 // +0x10
    C3DVector      m_up;                    // +0x1c (0, 1, 0)
    void*          m_pOwner;                // +0x28
    virtual void v00();
    CSoundReceiverLH(void* owner);
};

CSoundReceiverLH::CSoundReceiverLH(void* owner)
    : CBase()
    , m_state()
    , m_up(0.0f, 1.0f, 0.0f)
{
    m_pOwner = owner;
}