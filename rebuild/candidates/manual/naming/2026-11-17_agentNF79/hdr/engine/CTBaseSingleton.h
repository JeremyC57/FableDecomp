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
#ifdef FABLE_RETAIL_CTBASESINGLETON_RELEASE_BODY
    // NF79 per-TU opt-in (row 0x00404490, the out-of-line CTBaseSingleton<CFileInstaller>::Release COMDAT itself): Ego_r.pdb 0x00403CB0 /
    // FableWin.pdb 0x01871060 ?Release@?$CTBaseSingleton@VCFileInstaller@@@@SIXXZ (public static __fastcall), `PClass.Reset(0);` with the
    // default CCountedPointer Reset / ReleaseData inline (retail: Info then Data nulled, delete 0x00BFE9BC).
    static void FABLE_FASTCALL Release() { PClass.Reset(0); }
#endif
#ifdef FABLE_RETAIL_CTBASESINGLETON_RELEASE
    // Opt-in (chunk 2051, row 0x0042EC7C CNewFrontendGameComponent::Run): Ego_r.pdb ?Release@?$CTBaseSingleton@VCFileInstaller@@@@SIXXZ (out of line in retail at 0x00404490: PClass.Reset(0)).
    static void FABLE_FASTCALL Release();
#endif
#ifdef FABLE_RETAIL_CTBASESINGLETON_ISALLOCATED
    // NF76 opt-in (row 0x00599E3F NUISystem::CFrontEndManager::Update): FableWin.pdb ?IsAllocated@?$CTBaseSingleton@VCFileInstaller@@@@SI_NXZ,
    // inline in retail (the PClass object word tested in place: `cmp dword ptr [013CA818h],ebx ; je`, 0x00599E6E). Body only.
    static bool FABLE_FASTCALL IsAllocated() { return PClass.Data != 0; }
#endif
#ifdef FABLE_RETAIL_CTBASESINGLETON_GET_DECL
    // NF76 per-TU opt-in (row 0x00599E3F): Get() declared only; that TU calls the CTBaseSingleton<CFileInstaller> COMDAT
    // (Ego_r.pdb ?Get@?$CTBaseSingleton@VCFileInstaller@@@@SIAAVCFileInstaller@@XZ, retail 0x00404440) where the body below would expand.
    static T& FABLE_FASTCALL Get();
#else
    static T& FABLE_FASTCALL Get()
    {
        if (PClass.IsNull())
            PClass.Reset(new T);
        return *PClass;
    }
#endif
};
#endif
