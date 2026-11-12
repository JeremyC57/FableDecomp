// CTCSmokeGenerator::FrameUpdate() -- once the generation positions exist, give every position without a live smoke thing a new
// particle-emitter effect thing there (the def's ParticleEmitter); otherwise create the positions first
// Leaf: ?FrameUpdate@CTCSmokeGenerator@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00722740, 228 bytes (+ int3 padding). __fastcall virtual, plain `ret`; 8-aligned frame (the
//   C3DVector the camera returns).
// Identity: Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCSmokeGenerator@@UAEXXZ; the only reference is the CTCSmokeGenerator vtable
//   0x01265D98's FrameUpdate slot; members per Ego_r.pdb CTCSmokeGenerator (GenerationDistance +0x0c, GenerationPositions +0x10,
//   PositionsCreated +0x1c in retail, the 12-byte STLport vector), the same model as the landed OnKill 0x007224E0 / CreatePositions.
// Callees: CTCBase::GetWorld() const 0x00686A70 (CWorld::GetGameCamera inline, +0x34), CAIGameCameraBase::PeekCurrentPos() const
//   (virtual slot 11, +0x2c; C3DVector by hidden pointer, unused), CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10, CDefClassBase::GetSubDef<CSmokeGeneratorDef> 0x00722D30 (landed; its real body is in this
//   TU, out of line under auto_inline(off), as in the retail TU), CTCDParticleEmitter::Create(long, const C3DVector&, bool)
//   0x006E0880 (__fastcall), CTCSmokeGenerator::CreatePositions 0x00722590; the def's release through CDefPointeeBase::Delete.
// Shape: an STLport vector iterator loop (pos in ebp, &pos->PSmoke strength-reduced into ebx); the camera position is read into an
//   unused const local each iteration; the def is a CDefPointer for the rest of the if-block (released after the assignment), filled
//   through CThing::PeekBaseDef(); the smoke thing goes straight into a CIntelligentPointer<CThing> temporary that is assigned
//   (CINTELLIGENTPOINTER_ASSIGN: GetPItem then SetPItem), so VC7.1 homes the new thing in the dead def word.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"                         // GetPDefFromGlobalIndex 0x009ADA10 (protected; CDefClassBase a friend)
#include "engine/CThingBaseDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CTCDParticleEmitter.h"

class CSmokeGeneratorDef : public CSubDefClassBase {                    // RTTI .?AVCSmokeGeneratorDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CSmokeGeneratorDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    float GenerationDistance;                                            // +0x28
    long  ParticleEmitter;                                               // +0x2c
};
// The landed same-TU instantiation 0x00722D30 (its real body), out of line in retail.
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

class CTCSmokeGenerator : public CTCBase {
public:
    class CGenerationPos {                                               // 0x14 bytes
    public:
        C3DVector Pos;                                                   // +0x00
        CIntelligentPointer<CThing> PSmoke;                              // +0x0c
    };
    virtual void FrameUpdate();
protected:
    void CreatePositions();                                              // 0x00722590
public:
    float GenerationDistance;                                            // +0x0c
    std::vector<CGenerationPos> GenerationPositions;                     // +0x10
    bool PositionsCreated;                                               // +0x1c
};

void CTCSmokeGenerator::FrameUpdate()
{
    if (PositionsCreated) {
        for (std::vector<CGenerationPos>::iterator it = GenerationPositions.begin(); it != GenerationPositions.end(); ++it) {
            const C3DVector camera_pos = GetWorld().GetGameCamera()->PeekCurrentPos();
            if (!it->PSmoke.GetPItem()) {
                CDefPointer<const CSmokeGeneratorDef> def;
                GetParentThing().PeekBaseDef().GetSubDef(&def);
                it->PSmoke = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(def->ParticleEmitter, it->Pos, false));
            }
        }
    } else
        CreatePositions();
}