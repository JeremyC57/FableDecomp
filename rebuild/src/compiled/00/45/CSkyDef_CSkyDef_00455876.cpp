// CSkyDef::CSkyDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CSkyDef vtable, then the members' constructors
// Leaf: ??0CSkyDef@@QAE@XZ
// Retail: Fable.exe @ 0x00455876. Identity: the vtable it stores, 0x0123353C, is RTTI .?AVCSkyDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CSkyDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CSkyDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSkyDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CLensFlareElementDef.h"
class CPersistContext;

class CSkyDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSkyDef();                                   // 0x00455876
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long SunRadius;
    long SunTexture;
    long MoonRadius;
    long MoonTexture;
    long SunFlareRadius;
    long SunFlareTexture;
    long StarTexture;
    long StarChartTextureSize;
    float StarSize;
    long StarChartFilter;
    long TwinkleInterval;
    float TwinkleSpeed;
    float TwinkleMin;
    float TwinkleMax;
    std::vector<CLensFlareElementDef> FlareElements;
};

CSkyDef::CSkyDef()
{
}
