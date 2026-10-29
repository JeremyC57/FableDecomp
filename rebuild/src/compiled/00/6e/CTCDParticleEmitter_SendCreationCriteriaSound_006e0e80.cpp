// CTCDParticleEmitter::SendCreationCriteriaSound(CThing& thing) -- when the emitter thing has a sound component (interface 16), play the positional sound "CREATE_PARTICLE_" + the upper-cased name of its live particle emitter
// Leaf: ?SendCreationCriteriaSound@CTCDParticleEmitter@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E0E80, 141 bytes (+ int3 padding), private static `__fastcall` (ecx = thing), plain `ret`.
// Identity: Ego_r.pdb name set `?SendCreationCriteriaSound@CTCDParticleEmitter@@CIXAAVCThing@@@Z`; the catalog name is right.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface, interface 16 = sound),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CCharString::ToUpper() const @ 0x0099EE20 (by value), the `__fastcall`
//   `operator+(const char*, const CCharString&)` @ 0x0099F690 (hidden result in ecx, literal in edx), CCharString::
//   ~CCharString @ 0x0099EAE0 (x2), the sound component's PlaySound(const CCharString&, bool, long, long, long) @ 0x0066BD00.
// Layout: retail CThing interface bitset +0x20, interface map +0x44, driver +0x5C (read unchecked: the caller only passes
//   emitter things); CTCDParticleEmitter PEmitter +0x14; NParticleEngine::CParticleEmitter Name at +0x140 (PDB +0x14C,
//   0xC lower in retail like the +0x40 flag word). "CREATE_PARTICLE_" @ 0x0125FFB0.
// Shape: the driver is a named local read before the interface test (edi); the landed out-parameter GetInterface template
//   (key in the first frame word); the live emitter is fetched into a named local before the string expression (a chained
//   `GetPItem()->Name.ToUpper()` pushes the ToUpper result slot first and forms the member with `add`); the sound name is a named local in its own block, so it shares the key's frame word,
//   and the ToUpper temporary takes the second word.
class CTCBase;
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString ToUpper() const;                            // 0x0099EE20
};
CCharString __fastcall operator+(const char* text, const CCharString& tail);   // 0x0099F690

class CTCSound {                                            // the thing's interface-16 sound component
public:
    unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c);   // 0x0066BD00
};

namespace NParticleEngine {
class CParticleEmitter {
public:
    unsigned char _pad00[0x140];
    CCharString Name;                                       // +0x140
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCDParticleEmitter;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x5c - 0x4c];
    CTCDParticleEmitter* PTCDriver;                         // +0x5C
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCDParticleEmitter {
public:
    virtual void v00();
    unsigned char _base04[0x0c - 0x04];
    CCharString EmitterName;                                // +0x0C
    long ParticleTypeID;                                    // +0x10
    CIntelligentPointer<NParticleEngine::CParticleEmitter> PEmitter;                       // +0x14
private:
    static void __fastcall SendCreationCriteriaSound(CThing& thing);
};

void __fastcall CTCDParticleEmitter::SendCreationCriteriaSound(CThing& thing)
{
    CTCDParticleEmitter* emitter = thing.PTCDriver;
    CTCSound* sound;
    if (thing.GetInterface(&sound, 16)) {
        NParticleEngine::CParticleEmitter* particles = emitter->PEmitter.GetPItem();
        CCharString name = "CREATE_PARTICLE_" + particles->Name.ToUpper();
        sound->PlaySound(name, true, 0, 0, -1);
    }
}