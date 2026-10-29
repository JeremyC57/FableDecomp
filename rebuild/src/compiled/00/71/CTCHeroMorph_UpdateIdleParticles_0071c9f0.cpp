// CTCHeroMorph::UpdateIdleParticles() -- prunes dead idle emitters; when the hero has stood still long enough spawns the idle particle morphs, otherwise kills them
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C9F0, 832 bytes, `ret`.
// Callees: CPlayerManager/GetMainPlayer folded getters @ 0x00686D50/0x00449960, CPlayer::PeekPControlledCreature @ 0x00487DD0,
//          GFGetCurrentFrame, CDefClassBase::GetSubDef<CHeroMorphDef> @ 0x0071F930 (folded label), CParticleMorphs::GetAllMorphsOfType
//          @ 0x0081B160, GetMorphStrength @ 0x0071BA60, IS_THING_ALIVE @ 0x0040DCE0, CThing::GetTCDriver @ 0x004ACB50,
//          CTCDParticleEmitter::Create / AttachToThing, CIntelligentPointer(T*) @ 0x004A96F0, KillIdleParticles @ 0x0071C1F0,
//          free @ 0x00BFEA14. Idle delay global 0x013BAD24; epsilon 1e-4f @ 0x0129BA3C.
// Layout: CTCHeroMorph IdleParticleEmitters +0x18 (STLport list of CIntelligentPointer<CThing>, Ego_r), FrameHeroBecameIdle +0x38,
//         IdleParticlesAdded +0x3C; CHeroMorphDef IdleParticleMorphs +0x58; CMovement velocity +0x10C.
// Shape: same GetSubDef stand-in and helper-name reference binding as AddParticles (0x0071C7C0); the emitter is held as a const
//        CThing* and IS_THING_ALIVE takes an explicit (CThing*) cast so the temporary is stored before the call; the list item
//        is a named local pushed by reference.
#include <math.h>
#include <stdlib.h>
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString {
public:
    long Index;
    operator CCharString() const;                       // 0x00415DD0
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* PItem;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item);                       // 0x004A96F0 (CThing instance: vtable 0x01238C6C)
};
class CThing;
struct CIntelligentPointerList {
    struct _Node {
        _Node* _M_next;
        _Node* _M_prev;
        CIntelligentPointer<CThing> _M_data;
    };
    _Node* _M_node;
    void push_back(const CIntelligentPointer<CThing>& item);   // 0x004BA4E0
    __forceinline _Node* erase(_Node* node)
    {
        _Node* next = node->_M_next;
        _Node* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        node->_M_data.~CIntelligentPointer<CThing>();
        free(node);
        return next;
    }
};
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class C3DVector { public: float X, Y, Z; };
struct CParticleMorphEntry {
    long Type;
    bool Interpolate;                                   // +0x04
    long AttachMode;                                    // +0x08
    CDefString HelperName;                              // +0x0C
    long HelperIndex;                                   // +0x10
    long ParticleType;                                  // +0x14
    long ParticleTypeEnd;                               // +0x18
    float Min;                                          // +0x1C
    float Max;                                          // +0x20
};
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
};
class CParticleMorphs { public:
    bool GetAllMorphsOfType(long type, vector<CParticleMorphEntry>& entries) const;   // 0x0081B160
};
class CHeroMorphDef : public CDefClassBase {
public:
    unsigned char _pad08[0x48 - 0x08];
    CParticleMorphs ParticleMorphs;                     // +0x48
    unsigned char _pad49[0x58 - 0x49];
    CParticleMorphs IdleParticleMorphs;                 // +0x58
};
class CThingDef : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CHeroMorphDef>* out) const;   // 0x0071F930
};
class CTCBase;
class CTCDParticleEmitter;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    unsigned char _pad44[0x70 - 0x44];
    const CThingDef* PDef;                              // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                              // +0x91
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPos() const;               // 0x004C73D0
    bool GetTCDriver(CTCDParticleEmitter** out);        // 0x004ACB50
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
class CThingPlayerCreature;
class CPlayer {
public:
    long GetCurrentMode() const;                        // 0x00633BE0
    const CThingPlayerCreature* PeekPControlledCreature() const;                    // 0x00487DD0
};
struct CMovement {
    unsigned char _pad00[0x10c];
    C3DVector Velocity;                                 // +0x10C
};
class CThingPlayerCreature : public CMovement { };
class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const;                           // 0x00449960
};
class CWorld { public: static long __fastcall GetFrame(); };                    // 0x0049D870
extern long g_HeroIdleParticleDelay;                    // 0x013BAD24
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};
class CTCDParticleEmitter : public CTCBase {
public:
    static CThing* __fastcall Create(long type, const C3DVector& pos, bool flag);   // 0x006E0880
    static CThing* __fastcall Create(long type, long typeEnd, float t, const C3DVector& pos, bool flag);   // 0x006E11F0
    void AttachToThing(CThing& thing, long bone, const CCharString& helper, long index, float time);   // 0x006E0BE0
};
extern const float g_fEpsilon;                          // 0x0129BA3C

// Same-TU stand-in for the out-of-line GetSubDef (retail's link-time codegen saw that the out pointer does not escape).
#pragma auto_inline(off)
bool CThingDef::GetSubDef(CDefPointer<const CHeroMorphDef>* out) const
{
    out->Object = (const CHeroMorphDef*)this;
    return true;
}
#pragma auto_inline(on)

enum EHeroMorphType { };
class CTCHeroMorph : public CTCBase {
public:
    unsigned char _pad0c[0x18 - 0x0c];
    CIntelligentPointerList IdleParticleEmitters;       // +0x18
    unsigned char _pad1c[0x38 - 0x1c];
    long FrameHeroBecameIdle;                           // +0x38
    bool IdleParticlesAdded;                            // +0x3C
    float GetMorphStrength(EHeroMorphType type) const;  // 0x0071BA60 (Ego_r.pdb)
    void KillIdleParticles();                           // 0x0071C1F0
    void UpdateIdleParticles();
};

void CTCHeroMorph::UpdateIdleParticles()
{
    for (CIntelligentPointerList::_Node* node = IdleParticleEmitters._M_node->_M_next; node != IdleParticleEmitters._M_node; ) {
        if (!node->_M_data.GetPItem())
            node = IdleParticleEmitters.erase(node);
        else
            node = node->_M_next;
    }
    long mode = PeekPlayerManager().GetMainPlayer()->GetCurrentMode();
    const CMovement* movement = PeekPlayerManager().GetMainPlayer()->PeekPControlledCreature();
    if (mode == 1 && !(movement->Velocity.X * movement->Velocity.X + movement->Velocity.Y * movement->Velocity.Y
                       + movement->Velocity.Z * movement->Velocity.Z > g_fEpsilon * g_fEpsilon)) {
        if (!FrameHeroBecameIdle) {
            FrameHeroBecameIdle = CWorld::GetFrame();
            IdleParticlesAdded = false;
            return;
        }
        if (CWorld::GetFrame() - FrameHeroBecameIdle >= g_HeroIdleParticleDelay && !IdleParticlesAdded) {
            CDefPointer<const CHeroMorphDef> def;
            Thing->PDef->GetSubDef(&def);
            vector<CParticleMorphEntry> entries;
            if (def->IdleParticleMorphs.GetAllMorphsOfType(7, entries)) {
                float value = GetMorphStrength((EHeroMorphType)7);
                for (CParticleMorphEntry* entry = entries._M_start; entry != entries._M_finish; ++entry) {
                    if (entry->Min - g_fEpsilon <= value && g_fEpsilon + entry->Max >= value) {
                        const CThing* particle;
                        if (entry->Interpolate && entry->ParticleTypeEnd > 0) {
                            float t = 1.0f;
                            if (fabs(entry->Max - entry->Min) > g_fEpsilon)
                                t = (value - entry->Min) / (entry->Max - entry->Min);
                            particle = CTCDParticleEmitter::Create(entry->ParticleType, entry->ParticleTypeEnd, t, Thing->GetPos(), false);
                        } else {
                            particle = CTCDParticleEmitter::Create(entry->ParticleType, Thing->GetPos(), false);
                        }
                        if (IS_THING_ALIVE((CThing*)particle)) {
                            CTCDParticleEmitter* emitter = 0;
                            ((CThing*)particle)->GetTCDriver(&emitter);
                            if (entry->AttachMode == 1) {
                                CCharString helper("");
                                emitter->AttachToThing(*Thing, 8, helper, 0, 0.0f);
                            } else if (entry->AttachMode == 2) {
                                const CCharString& helper = entry->HelperName;
                                emitter->AttachToThing(*Thing, 0xa, helper, entry->HelperIndex, 0.0f);
                            } else if (entry->AttachMode == 3) {
                                const CCharString& helper = entry->HelperName;
                                emitter->AttachToThing(*Thing, 0xc, helper, entry->HelperIndex, 0.0f);
                            }
                            CIntelligentPointer<CThing> item((CThing*)particle);
                            IdleParticleEmitters.push_back(item);
                        }
                    }
                }
            }
            IdleParticlesAdded = true;
        }
    } else if (IdleParticlesAdded) {
        FrameHeroBecameIdle = 0;
        IdleParticlesAdded = false;
        KillIdleParticles();
    }
}