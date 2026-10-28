// CTCCoopSpirit::AddAppearanceParticle(const CCharString& particleThingName) -- create the named particle thing at the spirit's position and attach its emitter to the spirit (bone 0x88, scale 1.1)
//
// Retail 0x00670560, 141 bytes (`ret 4`). Callees: CThing::GetPos() const @ 0x004C73D0, the
// global Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970 (`__fastcall`),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const
// char*, long) @ 0x0099EBF0 (the empty string 0x0122D70E), CTCDParticleEmitter::AttachToThing
// (CThing*, long, const CCharString&, long, float) @ 0x006E0BE0, ~CCharString @ 0x0099EAE0.
// Shape: the emitter (interface 0x4b: bitset word 2 bit 11, `mov ecx,[eax+28h] ; test ch,8`) comes
// from a `__forceinline` helper around the out-parameter GetInterface whose uninitialised out pointer,
// the lookup key and the empty CCharString (a named local in its own `{}` block) all share the dead
// argument slot (a clear bit reads it back, `mov esi,[esp+0Ch]`); with the out pointer and the string
// as plain locals of the body VC opens a `push ecx` frame word instead.
class CTCBase;
class C3DVector;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
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
    const C3DVector& GetPos() const;                   // 0x004C73D0
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

CThing* __fastcall Create(const CCharString& defName, const C3DVector& position, bool flag);   // 0x006E0970

class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

__forceinline CTCDParticleEmitter* GetParticleEmitter(CThing* thing)
{
    CTCDParticleEmitter* emitter;
    thing->GetInterface(&emitter, 0x4b);                    // TC_PARTICLE_EMITTER
    return emitter;
}

class CTCCoopSpirit { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void AddAppearanceParticle(const CCharString& particleThingName);
};

void CTCCoopSpirit::AddAppearanceParticle(const CCharString& particleThingName)
{
    CThing* particle = Create(particleThingName, ParentThing->GetPos(), false);
    if (particle)
    {
        CTCDParticleEmitter* emitter = GetParticleEmitter(particle);
        {
            CCharString noBone("", -1);
            emitter->AttachToThing(ParentThing, 0x88, noBone, 0, 1.1f);
        }
    }
}