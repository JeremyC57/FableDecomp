#ifndef FABLE_ENGINE_CTBASESINGLETON_H
#define FABLE_ENGINE_CTBASESINGLETON_H
// New shared header (chunk 1013b, row 0x00507650 CWorldMap::LoadWorldFromBinaryFile): Lionhead's allocating singleton base.
// Ego_r.pdb / FableWin.pdb: ??0?$CTBaseSingleton@VCEngineManager@@@@IAE@XZ (protected ctor), ??1?$CTBaseSingleton@..@@UAE@XZ
// (public virtual dtor; size 4 in Ego_r: the vptr only), ?Get@?$CTBaseSingleton@VCEngineManager@@@@SIAAVCEngineManager@@XZ
// (public static __fastcall). The instance is a static CCountedPointer<T> (retail 0x013BA854 Data / 0x013BA858 Info for
// CEngineManager); its data-symbol name is in neither PDB name dump, so it is spelt like the sibling template's
// (engine/CTBaseSingletonNoDefaultAlloc.h, private static PClass).
// Get: retail's out-of-line COMDAT 0x00436FB0 (`mov eax,[013BA854h] ; test ; jne` .. `push 14h ; call new` + the inline
// T constructor .. `mov ecx,013BA854h ; call ?Reset@?$CCountedPointer@VCEngineManager@@@@QAEXPAVCEngineManager@@@Z`
// 0x004366B0 .. `mov eax,[013BA854h]`) is `if (PClass.IsNull()) PClass.Reset(new T); return *PClass;`; the CWorldMap loader
// 0x00507650 expands it with Reset inline (0x005078A0..0x00507951).
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
template<class T> class CTBaseSingleton {
private:
    static CCountedPointer<T> PClass;
protected:
    CTBaseSingleton() { }
public:
    virtual ~CTBaseSingleton();
    static T& FABLE_FASTCALL Get()
    {
        if (PClass.IsNull())
            PClass.Reset(new T);
        return *PClass;
    }
};
#endif
