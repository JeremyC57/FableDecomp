// CTCSmokeGenerator::FrameUpdate() -- once the generation positions exist, give every position without a live smoke thing a new particle-emitter effect thing there; otherwise create the positions
// Grade: DIFFER(230v228) -- rewritten on retail CThing/CTCBase/CIntelligentPointer/CDefPointer headers + the landed same-TU GetSubDef<CSmokeGeneratorDef> 0x00722D30 body under auto_inline(off) (b, dl=32: &PSmoke spilled, the smoke thing in ebx); scoping the def to an inner block (j) gives retail allocation (&PSmoke in ebx, smoke in the dead def slot) but releases the def before the pointer assignment (retail after); 13 grades (declaration order, named/temporary pointer, reference local, discarded camera result, while loop)
// Leaf: ?FrameUpdate@CTCSmokeGenerator@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722740, 228 bytes (plain `ret`; 8-aligned frame).
// Callees: CTCBase::GetWorld() const @ 0x00686A70, the current game camera's virtual slot 11 GetPosition (+0x2c, C3DVector through
//   the hidden pointer; the result is not used), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem
//   @ 0x00A01B90 / dtor @ 0x00A01C10, CDefClassBase::GetSubDef<CSmokeGeneratorDef> @ 0x00722D30 (landed, same TU),
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) @ 0x006E0880 (__fastcall), CTCSmokeGenerator::CreatePositions
//   @ 0x00722590; the def's release through its virtual slot 1.
// Layout: Ego_r.pdb CTCSmokeGenerator (GenerationDistance +0x0c, GenerationPositions +0x10, PositionsCreated +0x1c retail);
//   CThing PBaseDef +0x70 (retail; PDB +0x80); CSmokeGeneratorDef ParticleEmitter +0x2c; CWorld current game camera +0x34.
// Shape: the def through a CDefPointer filled by the landed same-TU GetSubDef<CSmokeGeneratorDef> body under auto_inline(off);
//   the smoke pointer is assigned from a CIntelligentPointer temporary.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CSmokeGeneratorDef : public CSubDefClassBase {                    // RTTI .?AVCSmokeGeneratorDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CSmokeGeneratorDef", -1); }
    unsigned char _pad_0x25[0x28 - 0x25];
    float         GenerationDistance;                                  // +0x28 (Ego_r.pdb)
    long          ParticleEmitter;                                     // +0x2c
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};
#pragma auto_inline(off)
template <>
bool CDefClassBase::GetSubDef<CSmokeGeneratorDef>(CDefPointer<const CSmokeGeneratorDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CSmokeGeneratorDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThingFlatDefs {                                  // the retail CThing words up to the base def
public:
    unsigned char _pad00[0x70];
    CDefPointer<const CDefClassBase> PBaseDef;          // +0x70 CDefPointer<const CThingBaseDef> (Ego_r.pdb +0x80)
};
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long type, const C3DVector& position, bool flag);    // 0x006E0880
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector GetPosition() const;              // slot 11 (+0x2c)
};
class CWorld {
public:
    unsigned char _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;              // +0x34
};
class CTCSmokeGenerator : public CTCBase {
public:
    class CGenerationPos {                              // 0x14 bytes
    public:
        C3DVector Pos;                                  // +0x00
        CIntelligentPointer<CThing> PSmoke;             // +0x0c
    };
    float GenerationDistance;                           // +0x0c
    CGenerationPos* GenerationPositions_first;          // +0x10 vector<CGenerationPos>
    CGenerationPos* GenerationPositions_last;           // +0x14
    CGenerationPos* GenerationPositions_end;            // +0x18
    bool PositionsCreated;                              // +0x1c
    virtual void FrameUpdate();
protected:
    void CreatePositions();                             // 0x00722590
};

void CTCSmokeGenerator::FrameUpdate()
{
    if (PositionsCreated) {
        for (CGenerationPos* pos = GenerationPositions_first; pos != GenerationPositions_last; ++pos) {
            C3DVector cameraPosition = GetWorld().PCurrentGameCamera->GetPosition();
            if (!pos->PSmoke.GetPItem()) {
                CThing* smoke;
                {
                    CDefPointer<const CSmokeGeneratorDef> def;
                    ((const CThingFlatDefs&)GetParentThing()).PBaseDef->GetSubDef(&def);
                    smoke = CTCDParticleEmitter::Create(def->ParticleEmitter, pos->Pos, false);
                }
                pos->PSmoke = CIntelligentPointer<CThing>(smoke);
            }
        }
    } else
        CreatePositions();
}
