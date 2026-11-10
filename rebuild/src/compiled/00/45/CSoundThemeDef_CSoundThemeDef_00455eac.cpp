// CSoundThemeDef::CSoundThemeDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CSoundThemeDef vtable, then the members' constructors
// Leaf: ??0CSoundThemeDef@@QAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x00455EAC. Identity: the vtable it stores, 0x01233824, is RTTI .?AVCSoundThemeDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CSoundThemeDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CSoundThemeDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSoundThemeDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CParentDefClassBase.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CRGBColour.h"
class CPersistContext;

class CSoundThemeDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSoundThemeDef();                                   // 0x00455EAC
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CRGBColour EditColour;
    long AtmosIndex;
    long MusicSet;
    float Gain;
    float Range;
    float Granularity;
    std::multimap<CCharString, long> AtmosGroupMap;
    bool PlayOnlyWhenInside;
    bool PlayQuietlyWhenInside;
    bool PlayOnlyWhenOutside;
    bool PlayQuietlyWhenOutside;
};

CSoundThemeDef::CSoundThemeDef()
{
}
