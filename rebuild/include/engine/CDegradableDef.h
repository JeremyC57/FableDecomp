#ifndef FABLE_ENGINE_CDEGRADABLEDEF_H
#define FABLE_ENGINE_CDEGRADABLEDEF_H
// Retail model (chunk 1018, row 0x0067E290): RTTI .?AVCDegradableDef@@ : CSubDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv),
// vtable slots (rebuild/manifest/rtti-vtables.tsv) 0 scalar deleting dtor 0x004545EE, 18 Transfer 0x0067E290, 19 Copy 0x0045442F,
// 20 GetSizeofClass 0x00453884. Members (Ego_r.pdb CDegradableDef, 64 bytes; retail 0x3C with the 12-byte STLport vector):
// Degradable +0x25 (right after the packed 0x25-byte base), GraphicType +0x28, InitiallyBlocksNavigation +0x2C, Degradations +0x30.
// Nested functors (Ego_r.pdb ??RCIsHealth1GTHealth2@CDegradableDef@@QBE_NABVCDegradableInfo@@0@Z, the std::sort comparator, out of
// line in the sort COMDAT; ??RCSetEngineGraphicType@CDegradableDef@@QBEXAAVCDegradableInfo@@@Z, inline in Transfer's for_each).
// Needs FABLE_RETAIL_DEF_CLASS_BASE.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDegradableDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDegradableInfo.h"
#include "stlport/fable_containers.h"
#ifdef _MSC_VER
enum EEngineGraphicType;                                   // enumerators not modelled (MS forward enum, as NUISystem_CMesh.h)
#else
enum EEngineGraphicType { };
#endif
class CPersistContext;
class CDegradableDef : public CSubDefClassBase {
public:
    class CIsHealth1GTHealth2 {
    public:
        bool operator()(const CDegradableInfo& info1, const CDegradableInfo& info2) const;
    };
    class CSetEngineGraphicType {
    public:
        CSetEngineGraphicType(EEngineGraphicType graphicType) : GraphicType(graphicType) { }
        void operator()(CDegradableInfo& info) const { info.EngineGraphic.Type = (unsigned char)GraphicType; }
    private:
        EEngineGraphicType GraphicType;
    };
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    bool                          Degradable;                // +0x25
    EEngineGraphicType            GraphicType;               // +0x28
    bool                          InitiallyBlocksNavigation; // +0x2C
    std::vector<CDegradableInfo>  Degradations;              // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CDegradableDef) == 0x3C);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CDEGRADABLEDEF_H
