#ifndef FABLE_ENGINE_CTBASESINGLETONNODEFAULTALLOC_H
#define FABLE_ENGINE_CTBASESINGLETONNODEFAULTALLOC_H
#include "engine/CCountedPointer.h"
// Original PDB template: independent polymorphic class, size4; PClass private static.
template<class T> class CTBaseSingletonNoDefaultAlloc {
private: static CCountedPointer<T> PClass;
public:
    virtual ~CTBaseSingletonNoDefaultAlloc();
    static T& __fastcall Get() { return *PClass; }
#ifdef FABLE_RETAIL_SINGLETON_ISSETUP
    // Retail opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): FableWin.pdb
    // ?IsSingletonSetup@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SI_NXZ (0x018B5D80, public static __fastcall; inline in retail:
    // DrawRetailDisplay tests the PClass object word in place, `mov eax,[013B8790h] ; test eax,eax ; je`, 0x004985F8 / 0x00499167).
    static bool __fastcall IsSingletonSetup() { return !PClass.IsNull(); }
#endif
#ifdef FABLE_RETAIL_SINGLETON_SET
    // Retail opt-in (agentNF7, row 0x004A6550 CWorld::PostInit): FableWin.pdb
    // ?Set@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIXPAVCQuestManager@@@Z (also the CPlayerGui / CDriveManager instances;
    // public static __fastcall, no out-of-line copy in retail): PostInit resets the PClass slot itself (`push <new CQuestManager> ;
    // mov ecx,offset 0x013B89FC ; call 0x004A9A10` = CCountedPointer<CQuestManager>::Reset). Body = that Reset.
    static void __fastcall Set(T* item) { PClass.Reset(item); }
#endif
};

#endif
