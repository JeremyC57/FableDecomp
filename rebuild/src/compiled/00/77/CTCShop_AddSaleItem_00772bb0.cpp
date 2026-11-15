// CTCShop::AddSaleItem(long, const CTCShop::GoodsPlace&) -- create the goods object at a sale place, line its COUNTER_PLACEMENT dummy up with the place, freeze it, mark it created and owned by the shop's thing, and put it on sale
// Inline-budget: /d2inlT116 -- CMatrix3x4::Set(const CRightHandedSet&, const C3DVector&), defined above the row with the landed
//   0x007767A0 text (out of line in this TU: 0x007767A0 sits among the CTCShop TU's GoodsPlace / CShopItem instantiations and has
//   no callers), expanded at all three sites (object, dummy, place matrices); at the default threshold VC7.1 calls it three times
//   (DIFFER 2163v2585), /d2inlT115 likewise, /d2inlT116 is the smallest N that matches (116..400 all match). Replaces the
//   force-inline header text FABLE_RETAIL_CMATRIX3X4_SET_RHSET_INLINE (same body; no longer used by this row).
// Leaf: ?AddSaleItem@CTCShop@@QAEXJABUGoodsPlace@1@@Z
// Flags: /d2inlT116
// Retail: Fable.exe (TLC, VC7.1) @ 0x00772BB0, 2585 bytes, `ret 8`.
// Identity: Ego_r.pdb ?AddSaleItem@CTCShop@@QAEXJABUGoodsPlace@1@@Z (engine/CTCShop.h declares it); caller the landed
//   CCreatureActionShopkeeperGetOut::PerformAction 0x00842D40 (TCShop->AddSaleItem(Goods, place)); literals 0x0126A920
//   "COUNTER_PLACEMENT", 0x0126A90C "COUNTER_PLACEMENT ", 0x0123AA60 "CTCCreatedEntity", 0x0123AA74 "CTCOwnedEntity".
// Callees: CTCBase::PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayerNumber 0x00449700, CThingObject::Create 0x00703210,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, the physics TC's GetRHSet (slot 72) / ResetPosition / ResetRHSet / EnablePhysics,
//   NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0, CThing::AddTC 0x004C9D60, the interface map LowerBound
//   0x0040F020 (inlined GetTC), CBaseIntelligentPointer 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / ~ 0x00A01C10
//   (the inlined CTCCreatedEntity::SetPCreator(const CThing&), landed out of line at 0x005ED440), CTCOwnedEntity::SetPOwner 0x007E8590,
//   CThing::RemoveTC 0x004C9840, CTCParticleAttacher::DetachParticles 0x007F8860 / AttachParticles 0x007F8890, the inline
//   CTCStockItem::SetAsForSale (Ego_r ?SetAsForSale@CTCStockItem@@QAEX_N@Z; `mov byte [ecx+0Ch],1`, 0x007735B5) and the virtual
//   OnInitialActivate (CTCBase slot 6, `call [edx+18h]`).
// Headers: engine/CTCCreatedEntity.h, engine/CTCStockItem.h (FABLE_RETAIL_CTCSTOCKITEM + the overlay opt-in
//   FABLE_RETAIL_CTCSTOCKITEM_SETASFORSALE). CTCParticleAttacher has no engine header: modelled locally, as the landed
//   CTCParticleAttacher::DetachParticles 0x007F8860 and CTCLight::AttachParticles 0x00641260 do.
// Shape: the matrix block is the CTCWallMount::AttachObject 0x007DE710 model (object, dummy and place matrices through the
//   same-TU CMatrix3x4::Set, the dummy's SetInverseOfUniformScaleRotationTranslation, two PostMultiply).
// x87 orders (measured, wfB6 NOTES.md): the order of the factors / terms at 0x00772F45, 0x00772F95..0x00772FE3 and
//   0x007731FB..0x0077332B follows the creation order of c2's records for the first words of dummyRH / dummyPos / placeRH and
//   for the inverse's locals.
// FITTED (chosen by grading among natural spellings; behaviour unchanged): (a) the place's orientation is ASSIGNED to a
//   default-constructed set through a named reference (`const CRightHandedSet& rhset = place.rhset; CRightHandedSet placeRH;
//   placeRH = rhset;`, a bitwise copy: C3DVector has no operator=), while objectRH, objectPos and placePos are copy
//   constructions; (b) the inverse names its sum of squares (FABLE_RETAIL_CMATRIX3X4_SETINVERSEUSRT_SCALE_SQUARED): retail has
//   no out-of-line CMatrix3x4::SetInverseOfUniformScaleRotationTranslation to grade it against (FableWin.pdb only, donor
//   0x0261C690; its debug copy is 123 i / 345 b against 122 / 343 for this text), so it is not byte-evidenced.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_CREATEDBYOWNER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSEUSRT_SCALE_SQUARED
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SET_ORIGINAL_NAMES
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_CTCSTOCKITEM
#define FABLE_RETAIL_CTCSTOCKITEM_SETASFORSALE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingObject.h"
#include "engine/CTCCreatedEntity.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCStockItem.h"
#include "engine/CTCShop.h"

// Same-TU callee, the landed 0x007767A0 text (?Set@CMatrix3x4@@QAEXABVCRightHandedSet@@ABVC3DVector@@@Z).
void CMatrix3x4::Set(const CRightHandedSet& rhset,const C3DVector& translate)
{
    Set(rhset.GetRight(), rhset.GetForward(), rhset.GetUp(), translate);
}

class CTCParticleAttacher : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_ATTACHER; }
    void DetachParticles();                                              // 0x007F8860
    void AttachParticles();                                              // 0x007F8890
};
void CTCShop::AddSaleItem(long goods, const GoodsPlace& place)
{
    long player = PeekPlayerManager().GetMainPlayerNumber();
    CThingObject* thing = CThingObject::Create(goods, place.pos, player, &place.rhset, 0, "");
    CRightHandedSet objectRH = thing->GetPhysicsTC()->GetRHSet();
    C3DVector objectPos = thing->GetPhysicsTC()->GetPosition();
    const CRightHandedSet& rhset = place.rhset;
    CRightHandedSet placeRH;
    placeRH = rhset;
    C3DVector placePos = place.pos;
    C3DVector dummyPos;
    CRightHandedSet dummyRH;
    NGraphicAppearance::CTCGraphicAppearance* appearance = thing->GetTCGraphicAppearance();
    if (appearance && (appearance->GetDummyObject("COUNTER_PLACEMENT", 0, 0, dummyPos, dummyRH, 1.0f)
                       || appearance->GetDummyObject("COUNTER_PLACEMENT ", 0, 0, dummyPos, dummyRH, 1.0f))) {
        CMatrix3x4 objectMatrix;
        objectMatrix.Set(objectRH, objectPos);
        CMatrix3x4 dummyMatrix;
        dummyMatrix.Set(dummyRH, dummyPos);
        CMatrix3x4 placeMatrix;
        placeMatrix.Set(placeRH, placePos);
        CMatrix3x4 dummyInverse;
        dummyInverse.SetInverseOfUniformScaleRotationTranslation(dummyMatrix);
        objectMatrix.PostMultiply(dummyInverse);
        objectMatrix.PostMultiply(placeMatrix);
        thing->GetPhysicsTC()->ResetPosition(C3DVector(objectMatrix.E41, objectMatrix.E42, objectMatrix.E43));
        thing->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(objectMatrix.E31, objectMatrix.E32, objectMatrix.E33),
                                                          C3DVector(objectMatrix.E21, objectMatrix.E22, objectMatrix.E23)));
    }
    thing->GetPhysicsTC()->EnablePhysics(false);
    thing->SetAsCreatedByOwnerThingCreation(true);
    thing->AddTC("CTCCreatedEntity", false, 0);
    CTCCreatedEntity* created;
    thing->GetTC(&created);
    created->SetPCreator(GetParentThing());
    thing->AddTC("CTCOwnedEntity", false, 0);
    CTCOwnedEntity* owned;
    thing->GetTC(&owned);
    owned->SetPOwner(&GetParentThing());
    if (thing->HasTC(TCI_ON_ACTION_USE))
        thing->RemoveTC(TCI_ON_ACTION_USE);
    CTCParticleAttacher* attacher;
    if (thing->GetTC(&attacher)) {
        attacher->DetachParticles();
        attacher->AttachParticles();
    }
    CTCStockItem* stock;
    if (thing->GetTC(&stock)) {
        stock->SetAsForSale(true);
        stock->OnInitialActivate();
    }
}