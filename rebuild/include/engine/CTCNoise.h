#ifdef FABLE_RETAIL_HEARING
#ifndef FABLE_ENGINE_CTCNOISE_H
#define FABLE_ENGINE_CTCNOISE_H
#include "engine/CTCBase.h"
#include "engine/CTCNoiseDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
// Layout: complete original 0x28463, both PDBs; size80.
class CTCNoise:public CTCBase{
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCNoise(const CTCNoise & p0);
 CTCNoise(CThing & p0);
 virtual void OnCreate();
 virtual void FrameUpdate();
 virtual bool HasFrameUpdate() const;
 void SetCreator(const CThing & p0);
 const CThing * GetCreator() const;
 void SetDistanceCanBeHeardFrom(float p0);
 float GetDistanceCanBeHeardFrom() const;
private:
 const CTCNoiseDef & GetDefinition() const;
 void CreateOnceOnlyNoise();
 void CreateContinuousNoise();
 CTCNoiseDef Definition;
 long LifeTimer;
 float DistanceCanBeHeardFrom;
 CIntelligentPointer<CThing const > Creator;
public:
 virtual ~CTCNoise();
 CTCNoise & operator=(const CTCNoise & p0);
#ifdef FABLE_RETAIL_CTCNOISE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007499E0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCNoise)==80);
#endif
#else
#ifndef FABLE_ENGINE_CTCNOISE_H
#define FABLE_ENGINE_CTCNOISE_H
// CTCNoise (new, chunk 765): RTTI .?AVCTCNoise@@, single base CTCBase at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members
// (struct_layouts_egor.tsv, 80 bytes, no STL container so the offsets are retail's): Definition (CTCNoiseDef, 52 bytes) +0x0C,
// LifeTimer +0x40, DistanceCanBeHeardFrom +0x44, Creator (CIntelligentPointer<const CThing>) +0x48; GetCreator 0x007498D0 reads
// +0x48. Name set: ?GetCreator@CTCNoise@@QBEPBVCThing@@XZ. Only the members and methods rows use are modelled (no virtual overrides).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
class CTCNoise : public CTCBase {
public:
    const CThing* GetCreator() const;                                    // 0x007498D0
protected:
    unsigned char Definition[0x34];                                      // +0x0c CTCNoiseDef
    long LifeTimer;                                                      // +0x40
    float DistanceCanBeHeardFrom;                                        // +0x44
    CIntelligentPointer<const CThing> Creator;                           // +0x48
#ifdef FABLE_RETAIL_CTCNOISE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007499E0
public:
#endif
};
#endif

#endif
