// Behaviour test for CParticleEmitter::DatabaseCopy (0x00AEEFE0): systems present in both are refreshed, missing ones
// are cloned in, extra ones are erased (counted pointers released), and the flag bits / distances are copied.
#include <cstdio>
#include <cstdlib>
struct CCharString {
    int m_id;
    CCharString& operator=(const CCharString& other) { m_id = other.m_id; return *this; }
};
struct SCountBlock {
    long  m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
static int g_destroyed = 0;
static int g_copied = 0;
class CParticleEmitter;
struct CParticleSystem {
    CParticleEmitter* PEmitter;
    CCharString       Name;
    int               Data;
    CParticleSystem* DatabaseClone() { CParticleSystem* c = new CParticleSystem(*this); c->PEmitter = 0; return c; }
    void DatabaseCopy(CParticleSystem* source) { Data = source->Data; ++g_copied; }
};
static void DestroySystem(void* p) { delete (CParticleSystem*)p; ++g_destroyed; }
struct CParticleSystemPtr {
    CParticleSystem* Data;
    SCountBlock*     Info;
    CParticleSystemPtr& operator=(const CParticleSystemPtr& other)
    {
        SCountBlock* count = other.Info;
        CParticleSystem* object = other.Data;
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
    void Release()
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
static CParticleSystemPtr* copy_down(CParticleSystemPtr* first, CParticleSystemPtr* last, CParticleSystemPtr* result)
{
    for (int n = (int)(last - first); n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}
struct CParticleSystemPtrVector {
    CParticleSystemPtr* _M_start;
    CParticleSystemPtr* _M_finish;
    CParticleSystemPtr  storage[8];
    CParticleSystemPtr* erase(CParticleSystemPtr* position)
    {
        if (position + 1 != _M_finish)
            copy_down(position + 1, _M_finish, position);
        --_M_finish;
        _M_finish->Release();
        return position;
    }
};
class CParticleEmitter {
public:
    CParticleSystemPtrVector ParticleSystems;
    unsigned short Priority : 3;
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned short Emitter2DLayer : 7;
    unsigned char PreWaterEffect : 1;
    unsigned char WaterEffect : 1;
    unsigned char ZBufferWriteable : 1;
    unsigned char Visible : 1;
    unsigned char AlwaysUpdate : 1;
    unsigned char DieIfOffsceen : 1;
    unsigned char OffscreenUpdate : 1;
    unsigned char ClipEffectToWeatherMask : 1;
    unsigned char EnableDithering : 1;
    unsigned char CalcBoundingSphereOnceOnly : 1;
    unsigned char BoundingSphereValid : 1;
    CCharString Name;
    float MaxSpawnDistance, MaxDrawDistance, FadeOutStart, FadeInStart, FadeInEnd;
    CParticleEmitter() { ParticleSystems._M_start = ParticleSystems._M_finish = ParticleSystems.storage; }
    CParticleSystem* Find(const CCharString& name) const
    {
        for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it)
            if (it->Data->Name.m_id == name.m_id)
                return it->Data;
        return 0;
    }
    CParticleSystem* GetPParticleSystemFromName(const CCharString& name) { return Find(name); }
    const CParticleSystem* GetPParticleSystemFromName(const CCharString& name) const { return Find(name); }
    void AddParticleSystem(CParticleSystem* system)
    {
        SCountBlock* b = new SCountBlock;
        b->m_references = 1; b->m_pfnDestroy = DestroySystem; b->m_pObject = system;
        ParticleSystems._M_finish->Data = system;
        ParticleSystems._M_finish->Info = b;
        ++ParticleSystems._M_finish;
    }
    void DatabaseCopy(const CParticleEmitter* source);
};
void CParticleEmitter::DatabaseCopy(const CParticleEmitter* source)
{
    Name = source->Name;
    for (CParticleSystemPtr* it = source->ParticleSystems._M_start; it != source->ParticleSystems._M_finish; ++it) {
        CParticleSystem* system = it->Data;
        CParticleSystem* mine = GetPParticleSystemFromName(system->Name);
        if (mine == 0) {
            CParticleSystem* clone = system->DatabaseClone();
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
static CParticleSystem* Make(int name, int data)
{
    CParticleSystem* s = new CParticleSystem;
    s->PEmitter = 0; s->Name.m_id = name; s->Data = data;
    return s;
}
int main()
{
    CParticleEmitter* src = new CParticleEmitter;
    CParticleEmitter* dst = new CParticleEmitter;
    src->AddParticleSystem(Make(1, 10));
    src->AddParticleSystem(Make(2, 20));
    dst->AddParticleSystem(Make(3, 30));
    dst->AddParticleSystem(Make(1, 0));
    dst->AddParticleSystem(Make(4, 40));
    src->Name.m_id = 77; dst->Name.m_id = 0;
    src->Priority = 5; dst->Priority = 1;
    src->Visible = 0; dst->Visible = 1;
    src->ClipEffectToWeatherMask = 1; dst->ClipEffectToWeatherMask = 0;
    src->Emitter2DLayer = 99; dst->Emitter2DLayer = 3;
    src->CalcBoundingSphereOnceOnly = 1; dst->CalcBoundingSphereOnceOnly = 0;
    src->BoundingSphereValid = 0; dst->BoundingSphereValid = 1;
    src->FadeInStart = 2.5f; src->FadeInEnd = 3.5f; src->MaxDrawDistance = 100.0f;
    dst->DatabaseCopy(src);
    int bad = 0;
    if (dst->ParticleSystems._M_finish - dst->ParticleSystems._M_start != 2) bad = 1;
    else {
        CParticleSystem* a = dst->ParticleSystems._M_start[0].Data;
        CParticleSystem* b = dst->ParticleSystems._M_start[1].Data;
        if (a->Name.m_id != 1 || a->Data != 10) bad = 2;
        if (b->Name.m_id != 2 || b->Data != 20 || b->PEmitter != dst) bad = 3;
    }
    if (g_destroyed != 2 || g_copied != 1) bad = 4;
    if (dst->Name.m_id != 77 || dst->Priority != 5 || dst->Emitter2DLayer != 99) bad = 5;
    if (dst->Visible != 1 || dst->ClipEffectToWeatherMask != 1 || dst->CalcBoundingSphereOnceOnly != 1 || dst->BoundingSphereValid != 1) bad = 6;
    if (dst->FadeInStart != 2.5f || dst->FadeInEnd != 3.5f || dst->MaxDrawDistance != 100.0f) bad = 7;
    if (bad) { std::printf("00aeefe0_TEST FAIL\n"); return 1; }
    std::printf("00aeefe0_TEST PASS\n");
    return 0;
}
