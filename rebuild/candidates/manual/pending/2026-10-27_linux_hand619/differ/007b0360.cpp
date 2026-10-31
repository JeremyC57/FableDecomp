// CTCFishingSpot::FrameUpdate() -- while enabled, (re)creates the two ripple particles and the underwater fish particle and attaches them to the spot; otherwise removes them
// Grade: DIFFER(773v773) -- respelled: leaf ?FrameUpdate@CTCFishingSpot@@UAEXXZ (virtual), RemoveParticles protected (IAE), GetPos, CBaseIntelligentPointer/CIntelligentPointer<T> with T* GetPItem/operator->, the emitter via CThing::GetTC<CTCDParticleEmitter>(&emitter) over CVectorMap operator[] (the landed GetTC<T>(T**) form) with an if/return-true HasTC; emitter = 0 then particle->GetTC(&emitter) gives retail xor-before-GetPItem and ebx in blocks 1 and 3 (positional diff 18 -> 11 lines); residue: block 2 only colours the particle pointer ebx / Create result esi (retail esi / ebx) -- tried temporaries for the CCharStrings (identical), explicit blocks without the helper (identical), one emitter for all blocks (identical)
// Leaf: ?FrameUpdate@CTCFishingSpot@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B0360, 773 bytes, `ret`. Identity: Ego_r.pdb ?FrameUpdate@CTCFishingSpot@@UAEXXZ
//   (virtual), ?RemoveParticles@CTCFishingSpot@@IAEXXZ (protected) 0x007B01C0.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem const 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThing::GetPos const 0x004C73D0,
//   CTCDParticleEmitter::Create(const CCharString&, const C3DVector&, bool) 0x006E0970 (static __fastcall),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCDParticleEmitter::AttachToThing 0x006E0BE0,
//   SetAlwaysUpdate 0x006E0590. Data: CIntelligentPointer<CThing> vtable 0x01238C6C; literals 0x0126D5B8 / 0x0126D5A8 /
//   0x0126D594, "" 0x0122D70E.
// Layout: Ego_r.pdb CTCFishingSpot (CTCBase 12 bytes, PParticle1 +0x0C, PParticle2 +0x14, PParticleFish +0x1C, Enabled +0x24);
//   retail CThing UsingComponents +0x20, Components +0x44 (CVectorMap, STLport vector of pairs).
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                         // Ego_r: vptr + PData (8 bytes)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.CBaseIntelligentPointer::GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CTCBase;
enum ETCInterfaceType { TC_DParticleEmitter = 0x4B };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class T> class CArrayStorage { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; T* end() { return _M_finish; } };
template <class K, class V, class CompareType = CKeyPairCompareLess<K, V> > class CVectorMap { public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    CArrayStorage<value_type> Data;
    CompareType Compare;
    bool Dirty;
    iterator LowerBound(const K& key);                  // 0x0040F020 (K = ETCInterfaceType)
    iterator end() { return Data.end(); }
    V& operator[](const K& key) { K keyCopy = key; iterator p = LowerBound(keyCopy); if (p == end() || key < p->first) p = end(); return p->second; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;  // +0x44
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        *out = (T*)Components[T::StatGetInterfaceType()];
        return true;
    }
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
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TC_DParticleEmitter; }
    static CThing* __fastcall Create(const CCharString& name, const C3DVector& pos, bool flag);   // 0x006E0970
    void AttachToThing(CThing& thing, long bone, const CCharString& helper, long mode, float time);   // 0x006E0BE0
    void SetAlwaysUpdate(bool always);                  // 0x006E0590
};
class CTCFishingSpot : public CTCBase {
public:
    CIntelligentPointer<CThing> PParticle1;             // +0x0C
    CIntelligentPointer<CThing> PParticle2;             // +0x14
    CIntelligentPointer<CThing> PParticleFish;          // +0x1C
    bool Enabled;                                       // +0x24
    virtual void FrameUpdate();
protected:
    void RemoveParticles();                             // 0x007B01C0
};

void CTCFishingSpot::FrameUpdate()
{
    if (Enabled) {
        if (!PParticle1.GetPItem()) {
            PParticle1 = CTCDParticleEmitter::Create("FISHING_SPOT_01", Thing->GetPos(), false);
            if (PParticle1.GetPItem()) {
                CTCDParticleEmitter* emitter = 0;
                PParticle1->GetTC(&emitter);
                emitter->AttachToThing(*Thing, 0x40, "", 0, 0.0f);
                emitter->SetAlwaysUpdate(true);
            }
        }
        if (!PParticle2.GetPItem()) {
            PParticle2 = CTCDParticleEmitter::Create("FISHING_SPOT_02", Thing->GetPos(), false);
            if (PParticle2.GetPItem()) {
                CTCDParticleEmitter* emitter = 0;
                PParticle2->GetTC(&emitter);
                emitter->AttachToThing(*Thing, 0x40, "", 0, 0.0f);
                emitter->SetAlwaysUpdate(true);
            }
        }
        if (!PParticleFish.GetPItem()) {
            PParticleFish = CTCDParticleEmitter::Create("UNDERWATER_FISH_01", Thing->GetPos(), false);
            if (PParticleFish.GetPItem()) {
                CTCDParticleEmitter* emitter = 0;
                PParticleFish->GetTC(&emitter);
                emitter->AttachToThing(*Thing, 0x80, "", 0, -1.0f);
                emitter->SetAlwaysUpdate(true);
            }
        }
    } else {
        RemoveParticles();
    }
}
