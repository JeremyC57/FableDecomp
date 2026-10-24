// Behaviour test for 0x004420D0: the melee-effects illustrator releases its ability def, destroys each particle effect then frees that storage, then destroys each primitive handle and frees that storage.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_log[32]; static int g_n = 0;
static void Free(void* block) { g_log[g_n++] = 99; std::free(block); }
struct CDefPointeeBase { virtual ~CDefPointeeBase() {} virtual void DeleteThis() { g_log[g_n++] = 50; } unsigned long RefCount; };
template<class T> struct CDefPointer {
    CDefPointeeBase* Object;
    ~CDefPointer() { CDefPointeeBase* object = Object; if (object) { if (--object->RefCount == 0) object->DeleteThis(); Object = 0; } }
};
struct CEnginePrimitiveHandle { virtual ~CEnginePrimitiveHandle() { g_log[g_n++] = id; } int id; };
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() { g_log[g_n++] = id; } int id; };
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
template<class T> inline void Destroy(T* first, T* last) { for (; first != last; ++first) first->~T(); }
template<class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~CVector() { Destroy(_M_start, _M_finish); if (_M_start) Free(_M_start); }
    void Make(int n, int firstId)
    {
        _M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        for (int i = 0; i < n; ++i) { new (&_M_start[i]) T(); _M_start[i].id = firstId + i; }
        _M_finish = _M_start + n; _M_end_of_storage = _M_finish;
    }
};
class CParticleEmitter; class CMeleeCombatAbilityDef;
struct CDrawBase {
    virtual void ClearPrimitivesIfNecessary() { } virtual void PrepareForLoad() { }
    void* PlayerGui; void* PlayerManager; void* World; void* Component; void* Bank; bool Drawing;
    CVector<CEnginePrimitiveHandle> PrimitiveHandles;
};
struct CDrawMeleeActionButtonEffects : CDrawBase {
    CVector<CIntelligentPointer<CParticleEmitter> > Effects;
    long FlashTimer; long ComboLoadingIconTimer; long FlourishCounter; long FlourishEffect;
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef;
};
int main()
{
    CDefPointeeBase def; def.RefCount = 1;
    {
        CDrawMeleeActionButtonEffects d;
        d.PrimitiveHandles.Make(1, 10);
        d.Effects.Make(2, 20);
        d.PMeleeAbilityDef.Object = &def;
    }
    // def deleted (50), effects 20, 21, their storage (99), handle 10, its storage (99)
    bool ok1 = g_n == 6 && g_log[0] == 50 && g_log[1] == 20 && g_log[2] == 21 && g_log[3] == 99 && g_log[4] == 10 && g_log[5] == 99
            && def.RefCount == 0;
    g_n = 0;
    {
        CDrawMeleeActionButtonEffects d;
        d.PrimitiveHandles.Make(0, 0);
        d.Effects.Make(0, 0);
        d.PMeleeAbilityDef.Object = 0;
    }
    bool ok2 = g_n == 0;
    if (!(ok1 && ok2)) { std::printf("004420d0_TEST FAIL\n"); return 1; }
    std::printf("004420d0_TEST PASS\n");
    return 0;
}