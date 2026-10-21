// behaviour test for 0x004EC9B5
#include <stdio.h>
#include <string.h>
#include <new>
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE };
class CDefString;
template <class T> class CArray {                        // retail 12 bytes: start, finish, end of storage
public:
    T* Start;
    T* Finish;
    T* EndOfStorage;
    CArray() : Start(0), Finish(0), EndOfStorage(0) {}
};
class CObjectAugmentationParticleSet {
public:
    float              PeriodSecs;                       // +0x00
    long               ParticleEffect;                   // +0x04
    long               ParticleEffectToBlendTo;          // +0x08
    long               AttackParticleEffect;             // +0x0c
    long               AttackParticleEffectToBlendTo;    // +0x10
    CArray<CDefString> StartPoint;                       // +0x14
    CArray<CDefString> EndPoint;                         // +0x20
    CObjectAugmentationParticleSet() : PeriodSecs(0.0f) {}
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
};
template struct pair<EObjectAugmentationType, CObjectAugmentationParticleSet>;
}

typedef std::pair<EObjectAugmentationType, CObjectAugmentationParticleSet> P;
int main()
{
    unsigned char buf[sizeof(P)]; memset(buf, 0x77, sizeof(buf));
    P* p = new (buf) P;
    int ok = sizeof(P) == 0x30 && p->first == 0 && p->second.PeriodSecs == 0.0f && buf[8] == 0x77 && buf[0x14] == 0x77
          && p->second.StartPoint.Start == 0 && p->second.StartPoint.EndOfStorage == 0 && p->second.EndPoint.Finish == 0
          && p->second.EndPoint.EndOfStorage == 0;
    printf(ok ? "004ec9b5_TEST PASS\n" : "004ec9b5_TEST FAIL\n");
    return ok ? 0 : 1;
}