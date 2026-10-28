// 007cdd10 behaviour test: CTCBurntEffect::Reset (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
};
static int g_destroys;
static bool g_effectNameKnown;
static long g_effectType;
static const char* g_lookedUp;
struct CEffectDef {
    long RefCount;
    void Destroy() { ++g_destroys; }
    bool GetEffectFromString(const CCharString& name, long& type) const
    {
        g_lookedUp = name.Text;
        if (!g_effectNameKnown || strcmp(name.Text, "SINGE_EFFECT") != 0)
            return false;
        type = g_effectType;
        return true;
    }
};
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) {}
    ~CDefPointer() { if (Data) { if (--Data->RefCount == 0) Data->Destroy(); } }
    T* operator->() const { return Data; }
};
static CEffectDef g_effectDef;
static bool g_hasSubDef;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CEffectDef>* out)
    {
        if (!g_hasSubDef)
            return false;
        ++g_effectDef.RefCount;
        out->Data = &g_effectDef;
        return true;
    }
};
class CSpecialAbilitiesBurntEffectDef : public CDefClassBase { public:
    float FadeInTime;
    float FadeOutTimeStart;
    float FadeOutTime;
};
class CThing;
template <class T> class CIntelligentPointer { public:
    T* m_pItem;
    CIntelligentPointer(T* item) : m_pItem(item) {}
};
template <class T> struct CRetailVector {
    T* items[8];
    int count;
    void push_back(const T& value) { items[count++] = new T(value); }
};
struct CTCBaseX { int _x; };
static int g_attaches;
static CThing* g_attachThing[3]; static long g_attachType[3]; static const char* g_attachHelper[3]; static void* g_attachEmitter[3];
class CTCDParticleEmitter : public CTCBaseX { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset)
    {
        g_attachThing[g_attaches] = thing; g_attachType[g_attaches] = attachType; g_attachHelper[g_attaches] = helperName.Text;
        g_attachEmitter[g_attaches] = this; ++g_attaches;
        (void)helperIndex; (void)offset;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseX* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    C3DVector     Position;
    CThing() : Position(1.0f, 2.0f, 3.0f) {}
    const C3DVector& GetPosition() const { return Position; }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTCDriver(long interfaceType)
    {
        if (!(m_flags91 & 1) && HasTC(interfaceType))
            return GetTC<T>(interfaceType);
        return 0;
    }
};
static CThing* g_created[3];
static int g_createCalls; static long g_createTypes[3]; static float g_createX;
static CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{
    (void)flag;
    g_createTypes[g_createCalls] = type; g_createX = position->X;
    return g_created[g_createCalls++];
}
static long g_FramesPerSecond;
static const CCharString g_BurntHelperA("HELPER_A");
static const CCharString g_BurntHelperB("HELPER_B");

class CTCBase { public:
    CThing* ParentThing;
};
class CTCBurntEffect : public CTCBase { public:
    CSpecialAbilitiesBurntEffectDef* PDef;
    long CurTime;
    long FadeInTime;
    long FadeOutTimeStart;
    long FadeOutTime;
    CRetailVector< CIntelligentPointer<CThing> > Effects;
    CSpecialAbilitiesBurntEffectDef* GetDef() const { return PDef; }
    void Reset();
};

void CTCBurntEffect::Reset()
{
    CurTime = 0;
    FadeInTime = (long)(PDef->FadeInTime * g_FramesPerSecond);
    FadeOutTimeStart = (long)(PDef->FadeOutTimeStart * g_FramesPerSecond);
    FadeOutTime = (long)(PDef->FadeOutTime * g_FramesPerSecond);
    CDefPointer<CEffectDef> def;
    long type = 0;
    C3DVector position = ParentThing->GetPosition();
    if (GetDef()->GetSubDef(&def) && def->GetEffectFromString("SINGE_EFFECT", type)) {
        CThing* effectA = CreateEffectThing(type, &position, false);
        if (effectA) {
            CTCDParticleEmitter* emitter = effectA->GetTCDriver<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(ParentThing, 10, g_BurntHelperA, 0, 0.0f);
        }
        CThing* effectB = CreateEffectThing(type, &position, false);
        if (effectB) {
            CTCDParticleEmitter* emitter = effectB->GetTCDriver<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(ParentThing, 10, g_BurntHelperB, 0, 0.0f);
        }
        CThing* effectC = CreateEffectThing(type, &position, false);
        if (effectC) {
            CTCDParticleEmitter* emitter = effectC->GetTCDriver<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(ParentThing, 8, "", 0, 0.0f);
        }
        Effects.push_back(CIntelligentPointer<CThing>(effectA));
        Effects.push_back(CIntelligentPointer<CThing>(effectB));
        Effects.push_back(CIntelligentPointer<CThing>(effectC));
    }
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }

static void MakeEffect(CThing& t, CInterfaceMap::SEntry* entries, int n, bool hasEmitter, bool dying)
{
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    if (hasEmitter) t.m_presentBits[0x4b >> 5] |= 1UL << (0x4b & 31);
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + n;
    t.m_flags91 = dying ? 1 : 0;
}

int main()
{
    CTCDParticleEmitter emA, emC;
    CInterfaceMap::SEntry entA[2] = { { 0x10, 0 }, { 0x4b, &emA } };
    CInterfaceMap::SEntry entC[3] = { { 0x4b, &emC }, { 0x50, 0 }, { 0x60, 0 } };
    CThing parent, fxA, fxB, fxC;
    MakeEffect(fxA, entA, 2, true, false);
    MakeEffect(fxB, entA, 2, true, true);      // being deleted: no attach
    MakeEffect(fxC, entC, 3, true, false);
    CSpecialAbilitiesBurntEffectDef pdef; pdef.FadeInTime = 0.5f; pdef.FadeOutTimeStart = 2.0f; pdef.FadeOutTime = 1.25f;
    g_FramesPerSecond = 30;
    g_effectType = 77;

    // full path
    CTCBurntEffect burn; burn.ParentThing = &parent; burn.PDef = &pdef; burn.CurTime = 99; burn.Effects.count = 0;
    g_hasSubDef = true; g_effectNameKnown = true; g_effectDef.RefCount = 0;
    g_created[0] = &fxA; g_created[1] = &fxB; g_created[2] = &fxC;
    burn.Reset();
    Check("times", burn.CurTime == 0 && burn.FadeInTime == 15 && burn.FadeOutTimeStart == 60 && burn.FadeOutTime == 37);
    Check("lookup", g_lookedUp && strcmp(g_lookedUp, "SINGE_EFFECT") == 0);
    Check("creates", g_createCalls == 3 && g_createTypes[0] == 77 && g_createTypes[2] == 77 && g_createX == 1.0f);
    Check("attaches", g_attaches == 2 && g_attachEmitter[0] == &emA && g_attachType[0] == 10 && g_attachThing[0] == &parent
        && strcmp(g_attachHelper[0], "HELPER_A") == 0 && g_attachEmitter[1] == &emC && g_attachType[1] == 8
        && strcmp(g_attachHelper[1], "") == 0);
    Check("effects", burn.Effects.count == 3 && burn.Effects.items[0]->m_pItem == &fxA && burn.Effects.items[1]->m_pItem == &fxB
        && burn.Effects.items[2]->m_pItem == &fxC);
    Check("def released", g_effectDef.RefCount == 0 && g_destroys == 1);

    // a null effect thing is still pushed; no attach for it
    g_createCalls = 0; g_attaches = 0; g_destroys = 0; burn.Effects.count = 0;
    g_created[0] = 0; g_created[1] = &fxA; g_created[2] = 0;
    burn.Reset();
    Check("null effects", g_createCalls == 3 && g_attaches == 1 && g_attachEmitter[0] == &emA
        && strcmp(g_attachHelper[0], "HELPER_B") == 0 && burn.Effects.count == 3 && burn.Effects.items[0]->m_pItem == 0);

    // effect name unknown: nothing spawned, def still released
    g_createCalls = 0; g_attaches = 0; g_destroys = 0; burn.Effects.count = 0; g_effectNameKnown = false;
    burn.Reset();
    Check("unknown effect", g_createCalls == 0 && burn.Effects.count == 0 && g_destroys == 1 && g_effectDef.RefCount == 0);

    // no sub-def: nothing spawned, nothing released; times still set
    g_destroys = 0; g_hasSubDef = false; g_lookedUp = 0; burn.FadeInTime = -1;
    burn.Reset();
    Check("no subdef", g_createCalls == 0 && g_destroys == 0 && g_lookedUp == 0 && burn.FadeInTime == 15);

    if (g_fail) return 1;
    printf("007cdd10_TEST PASS\n");
    return 0;
}