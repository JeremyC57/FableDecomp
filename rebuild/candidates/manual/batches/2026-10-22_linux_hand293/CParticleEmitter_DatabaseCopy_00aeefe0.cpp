// CParticleEmitter::DatabaseCopy(const CParticleEmitter* source) -- refresh this emitter from its database template: copy the name, update (or clone in) every particle system the template has, erase the systems the template no longer has, then copy the template's render flags and distances
// Leaf: DatabaseCopy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEEFE0, length 528 bytes (`ret 4`).
// Callees: CCharString::operator= @ 0x0099EFB0, CParticleEmitter::GetPParticleSystemFromName @ 0x00AEE180 (non-const,
//   on this) and @ 0x00AEE1E0 (const, on the source), NParticleEngine::CParticleSystem::DatabaseClone @ 0x00AF5960,
//   CParticleEmitter::AddParticleSystem @ 0x00AEEF40 (landed as "CGuiVarTransferStruct::Add": its vector at +8 is
//   ParticleSystems), NParticleEngine::CParticleSystem::DatabaseCopy @ 0x00AF4EE0, the STLport counted-pointer
//   __copy @ 0x00AF0D00 (`__fastcall`, tag by reference + null distance pointer), operator delete @ 0x00BFE9BC.
// Layout: retail CParticleEmitter (as the landed 0x00AEDAF0 WriteBinary): ParticleSystems +0x08, the ushort flag word
//   +0x40, Emitter2DLayer +0x42, the uchar flag bytes +0x44/+0x45, Name +0x140, MaxSpawnDistance..FadeInEnd
//   +0x158..+0x168; engine/CParticleEmitter.h is the wider PDB view, so the class is modelled locally.
// Shape: the erase loop is STLport vector::erase(pos) inlined (copy down, --finish, destroy the counted pointer).
struct CCharStringData;
struct CCharString {
    CCharStringData* m_pData;
    CCharString& operator=(const CCharString& other);                  // 0x0099EFB0
};
void __cdecl operator delete(void* p);                                 // 0x00BFE9BC
struct SCountBlock {
    long  m_references;                                                // +0
    void (__fastcall* m_pfnDestroy)(void*);                            // +4
    void* m_pObject;                                                   // +8
};
class CParticleEmitter;
namespace NParticleEngine {
struct CParticleSystem {
    void*             __vftable;                                       // +0x00
    CParticleEmitter* PEmitter;                                        // +0x04
    unsigned char     _state08[0x64];                                  // +0x08
    CCharString       Name;                                            // +0x6c
    CParticleSystem* DatabaseClone();                                  // 0x00AF5960
    void DatabaseCopy(CParticleSystem* source);                        // 0x00AF4EE0
};
}
struct CParticleSystemPtr {                                            // CCountedPointer<CParticleSystem> (8 bytes)
    NParticleEngine::CParticleSystem* Data;
    SCountBlock*                      Info;
    CParticleSystemPtr& operator=(const CParticleSystemPtr& other)
    {
        SCountBlock* count = other.Info;
        NParticleEngine::CParticleSystem* object = other.Data;
        if (Info != count) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = object;
            Info = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
    ~CParticleSystemPtr()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct random_access_iterator_tag { random_access_iterator_tag() {} };
#pragma auto_inline(off)
// Same-TU body of the STLport random-access __copy back end (0x00AF0D00): VC7.1 then sees that the empty tag is never
// read, so the tag temporary and the spilled `position + 1` share the dead `source` argument slot as in retail.
__declspec(noinline) CParticleSystemPtr* __fastcall __copy(CParticleSystemPtr* first, CParticleSystemPtr* last,
                                                           CParticleSystemPtr* result, const random_access_iterator_tag& tag,
                                                           int* distance)   // 0x00AF0D00
{
    for (int n = (int)(last - first); n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}
#pragma auto_inline(on)
struct CParticleSystemPtrVector {                                      // retail vector<CCountedPointer<CParticleSystem> >
    CParticleSystemPtr* _M_start;
    CParticleSystemPtr* _M_finish;
    CParticleSystemPtr* _M_end_of_storage;
    CParticleSystemPtr* erase(CParticleSystemPtr* position)
    {
        if (position + 1 != _M_finish)
            __copy(position + 1, _M_finish, position, random_access_iterator_tag(), (int*)0);
        --_M_finish;
        _M_finish->~CParticleSystemPtr();
        return position;
    }
};
class CParticleEmitter {
public:
    void*                    __vftable;                                // +0x00
    void*                    m_pLink;                                  // +0x04
    CParticleSystemPtrVector ParticleSystems;                          // +0x08
    unsigned char            UpdateParameters[0x24];                   // +0x14
    unsigned char            PrimitiveGroupHandle[8];                  // +0x38
    unsigned short Priority : 3;                                       // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned short Emitter2DLayer : 7;                                 // +0x42
    unsigned char PreWaterEffect : 1;                                  // +0x44
    unsigned char WaterEffect : 1;
    unsigned char ZBufferWriteable : 1;
    unsigned char Visible : 1;
    unsigned char AlwaysUpdate : 1;
    unsigned char DieIfOffsceen : 1;
    unsigned char OffscreenUpdate : 1;
    unsigned char ClipEffectToWeatherMask : 1;
    unsigned char EnableDithering : 1;                                 // +0x45
    unsigned char CalcBoundingSphereOnceOnly : 1;
    unsigned char BoundingSphereValid : 1;
    unsigned char ClearedParticles : 1;
    unsigned char ClearedDecals : 1;
    unsigned char _rest46[0xfa];                                       // +0x46 OldPos, Velocity, interpolators, transforms
    CCharString Name;                                                  // +0x140
    unsigned char BSphere[0x10];                                       // +0x144
    float DistanceToCam;                                               // +0x154
    float MaxSpawnDistance;                                            // +0x158
    float MaxDrawDistance;                                             // +0x15c
    float FadeOutStart;                                                // +0x160
    float FadeInStart;                                                 // +0x164
    float FadeInEnd;                                                   // +0x168
    NParticleEngine::CParticleSystem* GetPParticleSystemFromName(const CCharString& name);                   // 0x00AEE180
    const NParticleEngine::CParticleSystem* GetPParticleSystemFromName(const CCharString& name) const;       // 0x00AEE1E0
    void AddParticleSystem(NParticleEngine::CParticleSystem* system);                                        // 0x00AEEF40
    void DatabaseCopy(const CParticleEmitter* source);
};

void CParticleEmitter::DatabaseCopy(const CParticleEmitter* source)
{
    Name = source->Name;
    for (CParticleSystemPtr* it = source->ParticleSystems._M_start; it != source->ParticleSystems._M_finish; ++it) {
        NParticleEngine::CParticleSystem* system = it->Data;
        NParticleEngine::CParticleSystem* mine = GetPParticleSystemFromName(system->Name);
        if (mine == 0) {
            NParticleEngine::CParticleSystem* clone = system->DatabaseClone();
            clone->PEmitter = this;
            AddParticleSystem(clone);
        } else {
            mine->DatabaseCopy(system);
        }
    }
    for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ) {
        if (source->GetPParticleSystemFromName(it->Data->Name) == 0)
            it = ParticleSystems.erase(it);
        else
            ++it;
    }
    ContinuousEmitter = source->ContinuousEmitter;
    MaxSpawnDistance = source->MaxSpawnDistance;
    MaxDrawDistance = source->MaxDrawDistance;
    FadeOutStart = source->FadeOutStart;
    FadeInEnd = source->FadeInEnd;
    FadeInStart = source->FadeInStart;
    Priority = source->Priority;
    IsScreenDisplacement = source->IsScreenDisplacement;
    ReadZBuffer = source->ReadZBuffer;
    Emitter2D = source->Emitter2D;
    Emitter2DLayer = source->Emitter2DLayer;
    PreWaterEffect = source->PreWaterEffect;
    WaterEffect = source->WaterEffect;
    ZBufferWriteable = source->ZBufferWriteable;
    DieIfOffsceen = source->DieIfOffsceen;
    OffscreenUpdate = source->OffscreenUpdate;
    ClipEffectToWeatherMask = source->ClipEffectToWeatherMask;
    EnableDithering = source->EnableDithering;
    CalcBoundingSphereOnceOnly = source->CalcBoundingSphereOnceOnly;
}
