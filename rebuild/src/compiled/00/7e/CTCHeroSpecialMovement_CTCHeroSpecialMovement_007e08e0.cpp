// CTCHeroSpecialMovement::CTCHeroSpecialMovement(void* owner) -- the base constructed with the owner, two cleared flag bytes at +0xc/+0xd, cleared words at +0x14 and +0x18, this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E08E0, length 37 bytes.
// Callees: the base constructor @ 0x00686800 (the owner; the manifest label "CEngineSubPrimitive2DClockSprite" is propagated). Vtable 0x0126FE3C = CTCHeroSpecialMovement
// (rtti-vtables.tsv). Shape: the zero register serves the two bytes and two words, the vtable store is scheduled between the +0x14 and +0x18 stores. The chunk
// label "CEnginePrimitiveManagerParticleGroup" is propagated.
struct CTCBase_00686800 {
    virtual void v00();
    unsigned char _pad[8];                         // +0x04 .. +0x0c
    CTCBase_00686800(void* owner);                 // 0x00686800
};

struct SZeroWord_007e08e0 { long m_value; SZeroWord_007e08e0() : m_value(0) { } };

class CTCHeroSpecialMovement : public CTCBase_00686800 { public: // vtable 0x0126FE3C
    bool  m_active;                                // +0x0c
    bool  m_pending;                               // +0x0d
    unsigned char _pad0e[6];
    long  m_target;                                // +0x14
    SZeroWord_007e08e0 m_timer;                    // +0x18 (a member object: its inline constructor runs after the vtable store)
    virtual void v00();
    CTCHeroSpecialMovement(void* owner);
};

CTCHeroSpecialMovement::CTCHeroSpecialMovement(void* owner)
    : CTCBase_00686800(owner)
    , m_active(false), m_pending(false), m_target(0), m_timer()
{
}