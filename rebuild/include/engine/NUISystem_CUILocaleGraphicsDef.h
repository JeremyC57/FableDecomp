#ifndef FABLE_ENGINE_NUISYSTEM_CUILOCALEGRAPHICSDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUILOCALEGRAPHICSDEF_H
// NUISystem::CUILocaleGraphicsDef (new, chunk Q1): RTTI .?AVCUILocaleGraphicsDef@NUISystem@@ : CParentDefClassBase (rtti_bases.tsv),
// vtable 0x01235344. Ego_r.pdb layout (struct_layouts_egor.tsv, 100 bytes, base 0x40): WorldMapRegionName
// (map<EWorldMapNameGraphic, unsigned long>), HelpScreenGraphics (vector<unsigned long>), HelpRingPic; retail STLport containers
// are 12 bytes each: +0x3C, +0x48, +0x54 (operator= 0x00462FB1 assigns exactly those after the base assignment 0x00431ED0),
// sizeof 0x58. Name set (Ego_r.pdb / FableWin.pdb): ??0CUILocaleGraphicsDef@NUISystem@@QAE@XZ, ??1..UAE@XZ,
// ?Alloc@..SIPAVCDefClassBase@@XZ, ?Transfer@..UAEXAAVCPersistContext@@@Z, ?Copy@..UAEXPBVCDefClassBase@@@Z,
// ?GetSizeofClass@..UAEKXZ, ??4CUILocaleGraphicsDef@NUISystem@@QAEAAV01@ABV01@@Z (out of line in retail at 0x00462FB1).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/NUISystem_CUILocaleGraphicsDef.h needs the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CPersistContext;
enum EWorldMapNameGraphic { EWORLDMAPNAMEGRAPHIC_FORCE_DWORD = 0x7fffffff };
namespace NUISystem {
class CUILocaleGraphicsDef : public CParentDefClassBase {
public:
    CUILocaleGraphicsDef();
    virtual ~CUILocaleGraphicsDef();
    static CDefClassBase* FABLE_FASTCALL Alloc();
    virtual void Transfer(CPersistContext&);                             // slot 18
    virtual void Copy(const CDefClassBase*);                             // slot 19 0x00462FAC
    virtual unsigned long GetSizeofClass();                              // slot 20
    CUILocaleGraphicsDef& operator=(const CUILocaleGraphicsDef&);        // 0x00462FB1
    std::map<EWorldMapNameGraphic, unsigned long> WorldMapRegionName;    // +0x3c
    std::vector<unsigned long> HelpScreenGraphics;                       // +0x48
    unsigned long HelpRingPic;                                           // +0x54
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CUILocaleGraphicsDef) == 0x58);
#endif
