// IS_THING_ALIVE(const T& thing) [T = CIntelligentPointer<NParticleEngine::CParticleEmitter>] -- true when the pointer holds an emitter whose Dead bit is clear
// Leaf: IS_THING_ALIVE
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079DC20, 37 bytes (+ int3 padding to 48) (`__fastcall`, the reference in ecx, plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 twice (null test, then the flag read).
// Model: the IS_THING_ALIVE template of the landed thing-pointer instances (0x004AB8F0: GetPItem twice, nested ifs, shared
// false tail); the emitter's flags are a local model: Ego_r.pdb CParticleEmitter keeps a bitfield word (ContinuousEmitter,
// Dead, IsScreenDisplacement, ...) at PDB +0x4c, retail +0x40; retail tests bit 5 (`mov al ; shr al,5 ; test al,1`),
// modelled as Dead at bit 5 (the PDB dump does not keep the bit order). Shape: an `unsigned short` bit field read
// through an inline `bool IsDead()` gives the shr/test pair (the direct field read folds to `test byte ptr [eax+40h],20h`).
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad_0x00[0x40];
    unsigned short LowFlags : 5;                      // +0x40 bits 0..4
    unsigned short Dead : 1;                           // +0x40 bit 5
    unsigned short HighFlags : 10;                    // (Priority, Emitter2D, Initialised, Sleeping, ... in the PDB)
    bool IsDead() const { return Dead; }
};
}

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }

    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing.GetPItem()->IsDead())
            return true;
    }
    return false;
}

template bool __fastcall IS_THING_ALIVE<CIntelligentPointer<NParticleEngine::CParticleEmitter> >(const CIntelligentPointer<NParticleEngine::CParticleEmitter>&);