#ifndef FABLE_ENGINE_CSPAWNEDFUNC2_H
#define FABLE_ENGINE_CSPAWNEDFUNC2_H
#include "engine/CSpawnedFuncBase.h"
// Both PDBs prove the template, signatures, protected fields and their names.
// Ego_r size 0x44; native 00D0EBA0 reads Func+34, PClass+38, Par1+3c,
// Par2+40. Native 00D0EDB0 destroys Par2 before the base. Implicit destructor.
template<class T, class P1, class P2> class CSpawnedFunc2 : public CSpawnedFuncBase {
public:
    typedef void (T::*TSpawnedFunction)(P1,P2);
    FABLE_FORCEINLINE CSpawnedFunc2(const CCharString& name, unsigned long id, T* pclass,
                 TSpawnedFunction func, P1 par1, P2 par2)
        : CSpawnedFuncBase(name,id), Func(func), PClass(pclass), Par1(par1), Par2(par2) {}
    virtual void SuspendableProcess() { (PClass->*Func)(Par1,Par2); StopRunning=true; }
protected:
    TSpawnedFunction Func;
    T* PClass;
    P1 Par1;
    P2 Par2;
};
#endif
