#ifdef FABLE_RETAIL_EXPRESSION_FRAME
#ifndef FABLE_ENGINE_CEXPRESSIONDEF_H
#define FABLE_ENGINE_CEXPRESSIONDEF_H
#include "engine/CDefString.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefIndex.h"
#include "engine/EOpinionDeedType.h"
class CPersistContext;
#ifdef _MSC_VER
enum EGameEventType;
enum EExpressionInventoryType;
#else
enum EGameEventType{};
enum EExpressionInventoryType{};
#endif
// Layout: complete original Ego18cf1/FW19641; native144 from smaller parent base.
class CExpressionDef:public CParentDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 CDefString Animation;
 CDefString AnimationInto;
 CDefString AnimationLoop;
 CDefString AnimationOutOf;
 CDefString AnimationToOther;
 CDefString AnimationToOtherInto;
 CDefString AnimationToOtherLoop;
 CDefString AnimationToOtherOutOf;
 float SpriteDuration;
 long SpriteGraphic;
 CDefString SpriteOverrideText;
 CDefString SoundCriteria;
 CDefString LoopSoundCriteria;
 CDefIndex DummyObject;
 EGameEventType GameEventType;
 CDefString ScriptName;
 bool ScriptLoadResources;
 bool DeactivateScriptAtEnd;
 bool PerformAfterScript;
 EOpinionDeedType OpinionDeedType;
 bool Extendable;
 bool SheatheWeapons;
 bool ReplaceHeldItemWhenDone;
 unsigned long TextTag;
 EExpressionInventoryType ExpressionType;
 virtual void Transfer(CPersistContext & p0);
 CExpressionDef(const CExpressionDef & p0);
 CExpressionDef();
 virtual ~CExpressionDef();
 CExpressionDef & operator=(const CExpressionDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CExpressionDef)==144);
#endif
#else
#ifndef FABLE_ENGINE_CEXPRESSIONDEF_H
#define FABLE_ENGINE_CEXPRESSIONDEF_H
// CExpressionDef (new shared header, chunk 867, row 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList): the expression
// definition. RTTI .?AVCExpressionDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb
// names (struct_layouts_egor.tsv, first member at +0x40, sizeof 0x94) at the retail offsets, 4 lower, from the smaller retail
// CParentDefClassBase (0x39, padded to 0x3c): the landed CExpressionDef::Transfer 0x004569A7 reads them in this order and asserts
// sizeof 0x90 / ExpressionType +0x8c; ConstructPCExpressionsList reads ExpressionType as `mov eax,[eax+8Ch]`.
// EExpressionInventoryType: the enum's name is both PDBs' (the Transfer specialisation ??$Transfer@W4EExpressionInventoryType@@@
// CPersistContext@@...); its enumerators are in neither PDB name list, so the four values below carry placeholder spellings
// (naming queue) taken from the UI categories the row sorts them into (TEXT_GUI_MENU_SOCIAL / RENOWN / ALIGNMENT / STEALTH).
// Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail CDefClassBase) and FABLE_RETAIL_CDEFSTRINGTABLE + FABLE_RETAIL_CCHARSTRING (CDefString).
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/EOpinionDeedType.h"
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
enum EGameEventType;                                                 // MS forward enum (engine-wide; 4 bytes)
enum EExpressionInventoryType {                                      // enumerator spellings unproven (see above)
    EXPRESSION_INVENTORY_TYPE_SOCIAL = 0,
    EXPRESSION_INVENTORY_TYPE_RENOWN = 1,
    EXPRESSION_INVENTORY_TYPE_ALIGNMENT = 2,
    EXPRESSION_INVENTORY_TYPE_STEALTH = 3
};
class CPersistContext;
class CExpressionDef : public CParentDefClassBase {
public:
    static CDefClassBase* __fastcall Alloc();
    virtual void Transfer(CPersistContext&);                          // 0x004569A7
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CDefString    Animation;                                         // +0x3c
    CDefString    AnimationInto;                                     // +0x40
    CDefString    AnimationLoop;                                     // +0x44
    CDefString    AnimationOutOf;                                    // +0x48
    CDefString    AnimationToOther;                                  // +0x4c
    CDefString    AnimationToOtherInto;                              // +0x50
    CDefString    AnimationToOtherLoop;                              // +0x54
    CDefString    AnimationToOtherOutOf;                             // +0x58
    float         SpriteDuration;                                    // +0x5c
    long          SpriteGraphic;                                     // +0x60
    CDefString    SpriteOverrideText;                                // +0x64
    CDefString    SoundCriteria;                                     // +0x68
    CDefString    LoopSoundCriteria;                                 // +0x6c
    long          DummyObject;                                       // +0x70 CDefIndex (one long, CDefIndex::Val)
    EGameEventType GameEventType;                                    // +0x74
    CDefString    ScriptName;                                        // +0x78
    bool          ScriptLoadResources;                               // +0x7c
    bool          DeactivateScriptAtEnd;                             // +0x7d
    bool          PerformAfterScript;                                // +0x7e
    EOpinionDeedType OpinionDeedType;                                // +0x80
    bool          Extendable;                                        // +0x84
    bool          SheatheWeapons;                                    // +0x85
    bool          ReplaceHeldItemWhenDone;                           // +0x86
    unsigned long TextTag;                                           // +0x88
    EExpressionInventoryType ExpressionType;                         // +0x8c
};
FABLE_STATIC_ASSERT(sizeof(CExpressionDef) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CExpressionDef, ExpressionType) == 0x8c);
#endif // FABLE_ENGINE_CEXPRESSIONDEF_H

#endif
