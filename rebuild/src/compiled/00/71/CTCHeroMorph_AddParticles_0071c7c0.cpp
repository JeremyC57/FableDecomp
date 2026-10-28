// CTCHeroMorph::AddParticles(EHeroMorphType type, float value) -- for every particle morph of the type whose [Min, Max] range (with epsilon) covers the value, creates the emitter (interpolated between two particle types when asked), attaches it to the hero by attach mode and records it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C7C0, 548 bytes, `ret 8`.
// Callees: CDefClassBase::GetSubDef<CHeroMorphDef> @ 0x0071F930 (folded label), CParticleMorphs::GetAllMorphsOfType @ 0x0081B160,
//          CThing::GetPos @ 0x004C73D0, CTCDParticleEmitter::Create(long, long, float, const C3DVector&, bool) @ 0x006E11F0 /
//          Create(long, const C3DVector&, bool) @ 0x006E0880, CTCDParticleEmitter::AttachToThing @ 0x006E0BE0,
//          CCharString(const char*, long) @ 0x0099EBF0, CDefString::operator CCharString @ 0x00415DD0, ~CCharString @ 0x0099EAE0,
//          CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10, the emitter list push_back
//          @ 0x004BA4E0, free @ 0x00BFEA14. Epsilon 1e-4f @ 0x0129BA3C; CIntelligentPointer vtable 0x01238C6C.
// Layout: retail CTCHeroMorph ParticleEmitters +0x14; CHeroMorphDef ParticleMorphs +0x48; the 0x24-byte morph entries are
//         pair<EHeroMorphType, CParticleMorphs::CEntry> (Interpolate +4, AttachMode +8, HelperName +0xC, HelperIndex +0x10,
//         particle types +0x14/+0x18, Min/Max +0x1C/+0x20); CThing caches its particle-emitter TC at +0x5C.
// Shape: the GetSubDef stand-in (same TU, auto_inline off) lets VC keep the def in ebx as retail's link-time codegen did;
//        the helper-name conversion binds to a const reference (a named copy adds a copy-ctor call and a frame word); the
//        local vector frees its storage directly (free(), CRT thunk 0x00BFEA14).
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
    inline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
class CThing;
struct CIntelligentPointerList {
    void* Node;
    void push_back(const CIntelligentPointer<CThing>& item);   // 0x004BA4E0
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
    unsigned char _pad44[0x5c - 0x44];
    CTCDParticleEmitter* PParticleEmitter;              // +0x5C
    unsigned char _pad60[0x70 - 0x60];
    const CThingDef* PDef;                              // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                              // +0x91
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPos() const;               // 0x004C73D0
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
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

class CTCHeroMorph : public CTCBase {
public:
    unsigned char _pad0c[0x14 - 0x0c];
    CIntelligentPointerList ParticleEmitters;           // +0x14
    void AddParticles(long type, float value);
};

void CTCHeroMorph::AddParticles(long type, float value)
{
    CDefPointer<const CHeroMorphDef> def;
    Thing->PDef->GetSubDef(&def);
    vector<CParticleMorphEntry> entries;
    if (def->ParticleMorphs.GetAllMorphsOfType(type, entries)) {
        for (CParticleMorphEntry* entry = entries._M_start; entry != entries._M_finish; ++entry) {
            if (entry->Min - g_fEpsilon <= value && g_fEpsilon + entry->Max >= value) {
                CThing* particle;
                if (entry->Interpolate && entry->ParticleTypeEnd > 0) {
                    float t = 1.0f;
                    if (fabs(entry->Max - entry->Min) > g_fEpsilon)
                        t = (value - entry->Min) / (entry->Max - entry->Min);
                    particle = CTCDParticleEmitter::Create(entry->ParticleType, entry->ParticleTypeEnd, t, Thing->GetPos(), false);
                } else {
                    particle = CTCDParticleEmitter::Create(entry->ParticleType, Thing->GetPos(), false);
                }
                if (particle && !(particle->Flags91 & 1)) {
                    CTCDParticleEmitter* emitter = 0;
                    if (particle->HasTC(0x4b))
                        emitter = particle->PParticleEmitter;
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
                    ParticleEmitters.push_back(CIntelligentPointer<CThing>(particle));
                }
            }
        }
    }
}