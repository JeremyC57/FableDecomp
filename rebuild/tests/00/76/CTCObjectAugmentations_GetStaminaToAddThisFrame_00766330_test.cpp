#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
struct CDefVirtual { virtual void v00() { } virtual void Destroy() { } };
struct CDefBase : CDefVirtual { long m_refCount; };
template <class T> class CDefPointer { public: T* Object; CDefPointer() : Object(0) { } };
template <class T> inline void ReleaseDef(T* def) { if (--def->m_refCount == 0) def->Destroy(); }
class CAugmentationDef : public CDefBase { public:
    unsigned char _pad08[0x3c - 0x08]; float FrequencyToAddHealthInSeconds; long HealthToAddPerIncrease; float FrequencyToAddStaminaInSeconds; long StaminaToAddPerIncrease;
};
class CDefClassBase : public CDefBase { public: CAugmentationDef* m_sub; template <class T> bool GetSubDef(CDefPointer<T>& out) const { out.Object = m_sub; ++m_sub->m_refCount; return true; } };
class CThingBaseDef : public CDefClassBase { public: };
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; unsigned long size() const { return (unsigned long)(_M_finish - _M_start); } T& operator[](unsigned long n) const { return *(_M_start + n); } };
static long g_frame = 0;
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
long g_framesPerSecond = 30;
const float g_epsilon_0129ba3c = 0.0001f;
class CTCObjectAugmentations { public:
    virtual void v00() { }
    unsigned char _pad04[0x14 - 0x04];
    vector< CDefPointer<CThingBaseDef> > Augmentations;
    long GetHealthToAddThisFrame() const
    {
        long total = 0;
        for (unsigned long i = 0; i < Augmentations.size(); ++i) {
            CThingBaseDef* def = Augmentations[i].Object;
            if (def) {
                CDefPointer<CAugmentationDef> slot; def->GetSubDef(slot); CAugmentationDef* aug = slot.Object;
                if (aug->FrequencyToAddHealthInSeconds > g_epsilon_0129ba3c) {
                    long period = CeilToLong(g_framesPerSecond * aug->FrequencyToAddHealthInSeconds);
                    if (CWorld::GetWorldFrame() % period == 0) total += aug->HealthToAddPerIncrease;
                }
                ReleaseDef(aug);
            }
        }
        return total;
    }
    long GetStaminaToAddThisFrame() const
    {
        long total = 0;
        for (unsigned long i = 0; i < Augmentations.size(); ++i) {
            CThingBaseDef* def = Augmentations[i].Object;
            if (def) {
                CDefPointer<CAugmentationDef> slot; def->GetSubDef(slot); CAugmentationDef* aug = slot.Object;
                if (aug->FrequencyToAddStaminaInSeconds > g_epsilon_0129ba3c) {
                    long period = CeilToLong(g_framesPerSecond * aug->FrequencyToAddStaminaInSeconds);
                    if (CWorld::GetWorldFrame() % period == 0) total += aug->StaminaToAddPerIncrease;
                }
                ReleaseDef(aug);
            }
        }
        return total;
    }
};
static CAugmentationDef g_aug1, g_aug2; static CThingBaseDef g_def1, g_def2; static CDefPointer<CThingBaseDef> g_slots[3];
static CTCObjectAugmentations* Make()
{
    static CTCObjectAugmentations o;
    g_aug1.m_refCount = 5; g_aug1.FrequencyToAddHealthInSeconds = 0.21f; g_aug1.HealthToAddPerIncrease = 3; g_aug1.FrequencyToAddStaminaInSeconds = 0.0f; g_aug1.StaminaToAddPerIncrease = 9;
    g_aug2.m_refCount = 5; g_aug2.FrequencyToAddHealthInSeconds = 0.5f;  g_aug2.HealthToAddPerIncrease = 4; g_aug2.FrequencyToAddStaminaInSeconds = 1.0f; g_aug2.StaminaToAddPerIncrease = 7;
    g_def1.m_sub = &g_aug1; g_def2.m_sub = &g_aug2;
    g_slots[0].Object = &g_def1; g_slots[1].Object = 0; g_slots[2].Object = &g_def2;
    o.Augmentations._M_start = g_slots; o.Augmentations._M_finish = g_slots + 3; o.Augmentations._M_end_of_storage = g_slots + 3;
    return &o;
}
int main()
{
    CTCObjectAugmentations* o = Make();
    g_frame = 0;  bool ok = o->GetStaminaToAddThisFrame() == 7;     // aug1 frequency 0 -> skipped; aug2 period 30
    g_frame = 15; ok = ok && o->GetStaminaToAddThisFrame() == 0;
    g_frame = 60; ok = ok && o->GetStaminaToAddThisFrame() == 7;
    ok = ok && g_aug1.m_refCount == 5 && g_aug2.m_refCount == 5;
    std::printf(ok ? "00766330_TEST PASS\n" : "00766330_TEST FAIL\n");
    return ok ? 0 : 1;
}