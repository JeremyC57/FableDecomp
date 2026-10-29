// Behaviour test for 008f2580: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCardBase::AddPointerParticleEffect(long particleDef) -- spawns an effect thing at the pointer object, remembers it in the particle-effect list and attaches its emitter to the pointer
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
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
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingObject : public CThing { public: };

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
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
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
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
    CThing* effect = CreateEffectThing(particleDef, &Pointer->GetPosition(), false);
    if (effect && !effect->IsDead()) {
        ParticleEffects.push_back(CIntelligentPointer<CThing>(effect));
        CTCDParticleEmitter* emitter = GetParticleEmitter(effect);
        {
            CCharString noBone("", -1);
            emitter->AttachToThing(*Pointer, 0x28, noBone, 0, 0.0f);
        }
    }
}

// ---- stubs and driver ----
void CTCTavernGame::v00() {}
static C3DVector g_pos;
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing* g_spawn; static long g_spawnType; static const C3DVector* g_spawnPos; static bool g_spawnFlag = true;
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag) { g_spawnType = type; g_spawnPos = position; g_spawnFlag = flag; return g_spawn; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
static int g_ipDtors;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtors; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = item; }
template<class T> void CIntelligentPointer<T>::v00() {}
static CBaseObject* g_pushed[4]; static int g_pushes;
template<class T> void CRetailVector<T>::push_back(const T& value) { if (g_pushes < 4) g_pushed[g_pushes] = value.m_pItem; ++g_pushes; }
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() {}
static CTCDParticleEmitter g_emitter;
static int g_attaches; static CThing* g_attachThing; static long g_bone = -1, g_flags = -1; static float g_scale = -1; static bool g_emptyName;
void CTCDParticleEmitter::AttachToThing(CThing& thing_ref, long boneIndex, const CCharString& boneName, long flags, float scale)
{ CThing* thing = &thing_ref; if (this == &g_emitter) ++g_attaches; g_attachThing = thing; g_bone = boneIndex; g_flags = flags; g_scale = scale; g_emptyName = *(const char*)boneName.PStringData == 0; }

int main()
{
    static CTCTavernGameCardBase game;
    static CThingObject pointer;
    static CThing effect;
    game.Pointer = &pointer;
    effect.m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31);
    g_entries[0].type = 0x4b; g_entries[0].component = (CTCBase*)&g_emitter;
    effect.m_interfaces.first = g_entries; effect.m_interfaces.last = g_entries + 1;
    // no thing created: nothing else happens
    g_spawn = 0;
    game.AddPointerParticleEffect(17);
    if (g_spawnType != 17 || g_spawnPos != &g_pos || g_spawnFlag || g_pushes || g_attaches) { std::printf("008f2580_TEST FAIL (null)\n"); return 1; }
    // a dead thing: skipped
    g_spawn = &effect; effect.m_flags = 1;
    game.AddPointerParticleEffect(18);
    if (g_pushes || g_attaches) { std::printf("008f2580_TEST FAIL (dead)\n"); return 1; }
    // a live thing: remembered and attached to the pointer
    effect.m_flags = 0;
    game.AddPointerParticleEffect(19);
    if (g_pushes != 1 || g_pushed[0] != (CBaseObject*)&effect || g_ipDtors != 1 || g_attaches != 1 || g_attachThing != &pointer
        || g_bone != 0x28 || g_flags != 0 || g_scale != 0.0f || !g_emptyName) { std::printf("008f2580_TEST FAIL (alive)\n"); return 1; }
    std::printf("008f2580_TEST PASS\n");
    return 0;
}