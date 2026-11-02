// CTCCoopSpirit::~CTCCoopSpirit() -- co-op spirit thing component destructor: kill the particle emitter if it is still alive, clear Master, then destroy the members and ~CTCBase
// Leaf: ??1CTCCoopSpirit@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00670090, int3 padding before and after, thiscall, tail `jmp ~CTCBase`.
// Identity (proven, two sources): (1) it stores vtable 0x0125B264 (RTTI .?AVCTCCoopSpirit@@ : CTCBase) and its sole caller is the
//   scalar deleting destructor 0x00670C50, slot 0 of that vtable; (2) Ego_r.pdb 0x006777C0 / FableWin.pdb 0x020D5030
//   ??1CTCCoopSpirit@@UAE@XZ; members walked are Ego_r's PParticleEmitter (+0x2C), Master (+0x24) and PCoopSpiritDef (+0x28).
// Callees (reloc_bindings): ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50, ?Kill@CThing@@QAEX_N@Z 0x004C9B80,
//   ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10, ??1CTCBase@@UAE@XZ (tail). Ego_r places GetPItem 0x00B4C7E0 and ??1 0x00B4C8A0
//   next to each other (+0xC0), the same spacing as retail 0x00A01B50 -> 0x00A01C10.
// Shape: user body (null test through Get(), Kill(true), Master = 0), then the compiler destroys PParticleEmitter (out-of-line
//   ~CBaseIntelligentPointer) and PCoopSpiritDef (CDefPointer: trivial in release), then CTCBase.
// Layout: Ego_r.pdb CTCCoopSpirit, retail sizeof 0x34 (no STL member). CTCBase / CClass / CDefPointer are the shared headers' retail
//   opt-ins. CBaseIntelligentPointer has no shared header (chunk 706 local model).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include "engine/CClass.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CThing : public CClass {                          // RTTI CThing : CClass
public:
    void Kill(bool immediately);                        // 0x004C9B80 ?Kill@CThing@@QAEX_N@Z
};

#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes; 1-aligned in retail, chunk 706 model)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                      // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // FableWin member set: Get / Set / -> / *
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};
#pragma pack(pop)

class C3DVector { public: float X; float Y; float Z; };
class CPlayer;
class CCoopSpiritDef;

class CTCCoopSpirit : public CTCBase {                  // vtable 0x0125B264
public:
    CTCCoopSpirit(CThing& thing);
    virtual ~CTCCoopSpirit();
    long                              NoMovementTimer;      // +0x0c
    long                              Score;                // +0x10
    long                              OffscreenCounter;     // +0x14
    C3DVector                         MovementVector;       // +0x18
    CPlayer*                          Master;               // +0x24
    CDefPointer<const CCoopSpiritDef> PCoopSpiritDef;       // +0x28
    CIntelligentPointer<CThing>       PParticleEmitter;     // +0x2c
};
FABLE_STATIC_ASSERT(sizeof(CTCCoopSpirit) == 0x34);

CTCCoopSpirit::~CTCCoopSpirit()
{
    if (PParticleEmitter.Get())
        PParticleEmitter->Kill(true);
    Master = 0;
}