// CSimVoicesDef::CSimVoicesDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CSimVoicesDef vtable, then the members' constructors
// Leaf: ??0CSimVoicesDef@@QAE@XZ
// Retail: Fable.exe @ 0x00455E72. Identity: the vtable it stores, 0x012337BC, is RTTI .?AVCSimVoicesDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CSimVoicesDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model; CSimVoice::CSimVoice 0x00455E39 (??0CSimVoice@@QAE@XZ) on Entry.
// Layout: Ego_r.pdb CSimVoicesDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSimVoicesDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CPersistContext;
#include "engine/CInitBaseClass.h"
#include "engine/CVectorMap.h"
enum EReactionSpeechType { };                                   // enumerators not needed here (no engine header)
class CSimVoice : public CInitBaseClass {                       // RTTI .?AVCSimVoice@@ : CInitBaseClass; vtable 0x012337AC; no engine header
public:
    CSimVoice();                                                // 0x00455E39 (??0CSimVoice@@QAE@XZ, bound by landed callers)
    virtual ~CSimVoice();                                       // 0x00455E59
    CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType, long> > Voices;   // +0x08 (Ego_r.pdb CSimVoice::Voices)
};
FABLE_STATIC_ASSERT(sizeof(CSimVoice) == 0x18);

class CSimVoicesDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSimVoicesDef();                                   // 0x00455E72
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CSimVoice Entry;
};

CSimVoicesDef::CSimVoicesDef()
{
}
