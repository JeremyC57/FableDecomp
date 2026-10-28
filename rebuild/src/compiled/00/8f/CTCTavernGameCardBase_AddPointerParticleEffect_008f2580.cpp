// CTCTavernGameCardBase::AddPointerParticleEffect(long particleDef) -- spawns an effect thing at the pointer object, remembers it in the particle-effect list and attaches its emitter to the pointer
// Leaf: AddPointerParticleEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2580, 218 bytes (`ret 4`). Ego_r.pdb
// `?AddPointerParticleEffect@CTCTavernGameCardBase@@QAEXJ@Z`.
// Callees: CThing::GetPos() const 0x004C73D0; CreateEffectThing(long, const C3DVector*, bool) 0x006E0880
//          (`__fastcall`, landed spelling; manifest "Create"); CBaseIntelligentPointer() 0x00A01B10 (then the
//          CIntelligentPointer<CThing> vtable 0x01238C6C), ::SetPItem 0x00A01B90, ::~CBaseIntelligentPointer 0x00A01C10;
//          vector<CIntelligentPointer<CThing> >::push_back(const T&) 0x004AD520; CVectorMap<ETCInterfaceType, CTCBase*>::
//          LowerBound 0x0040F020; CCharString(const char*, long) 0x0099EBF0 ("" @ 0x0122D70E); CTCDParticleEmitter::
//          AttachToThing(CThing*, long, const CCharString&, long, float) 0x006E0BE0; ~CCharString 0x0099EAE0.
// Layout (retail, as the landed constructor 0x008F1CD0): Pointer +0x140, ParticleEffects +0x148; the thing's dead flag
//         is bit 0 of +0x91, the interface bitset +0x20 (0x4B = word 2 bit 11, `test ah,8`), the map +0x44.
// Shape: the effect thing is skipped when null or dead; a CIntelligentPointer temporary is built in the frame for the
//        out-of-line push_back; the emitter comes from a `__forceinline` helper around the unchecked out-parameter
//        GetInterface, so its uninitialised out pointer, the lookup key and the empty CCharString (a named local in its
//        own block) all share the dead `particleDef` argument slot (landed twin 0x00670560).
class CTCBase;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingObject : public CThing { public: };

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

__forceinline CTCDParticleEmitter* GetParticleEmitter(CThing* thing)
{
    CTCDParticleEmitter* emitter;
    thing->GetInterface(&emitter, 0x4b);        // TC_PARTICLE_EMITTER
    return emitter;
}

struct CBaseIntelligentPointer {                // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    void push_back(const T& value);             // 0x004AD520
};

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _body04[0xf0 - 0x04];         // CTCBase + CTCTavernGame members (retail 0xF0)
};
struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    unsigned char _pad_0xf0[0x50];
    CThingObject* Pointer;                      // +0x140
    float         PointerScale;                 // +0x144
    CRetailVector<CIntelligentPointer<CThing> > ParticleEffects;   // +0x148
    void AddPointerParticleEffect(long particleDef);
};

void CTCTavernGameCardBase::AddPointerParticleEffect(long particleDef)
{
    CThing* effect = CreateEffectThing(particleDef, &Pointer->GetPos(), false);
    if (effect && !effect->IsDead()) {
        ParticleEffects.push_back(CIntelligentPointer<CThing>(effect));
        CTCDParticleEmitter* emitter = GetParticleEmitter(effect);
        {
            CCharString noBone("", -1);
            emitter->AttachToThing(Pointer, 0x28, noBone, 0, 0.0f);
        }
    }
}