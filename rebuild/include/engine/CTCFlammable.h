#ifndef FABLE_ENGINE_CTCFLAMMABLE_H
#define FABLE_ENGINE_CTCFLAMMABLE_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
// Opt-in (agentNF22, row 0x00780410 CTCFlammable::SetOnFire): the Ego_r.pdb members up to FirePoints. PDef +0x0c
// (CDefPointer<const CFlammableDef>, one word), FirePoints +0x10 = vector<NFire::CFirePoint> (retail STLport, 12 bytes;
// CFirePoint is 0x2c bytes, Resistance +0x0c). The row that opts in completes NFire::CFirePoint.
#include "stlport/fable_containers.h"
namespace NFire { class CFirePoint; }
#endif
#ifdef FABLE_RETAIL_CTCFLAMMABLE_GETCLOSESTFIREPOINTTO
class C3DVector;
#endif
#ifdef FABLE_RETAIL_CTCFLAMMABLE_FRAMEUPDATE
#include "engine/CDefPointer.h"
class CFlammableDef;
#endif
// Declared callable interface only; no instance is constructed or sized by this row.
class CTCFlammable:public CTCBase {public:
#ifdef FABLE_RETAIL_FLAMMABLE_INIT_FIRE_POINTS_PASS29
private:
 void InitFirePoints();
public:
#endif
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_FLAMMABLE;}
#ifdef FABLE_RETAIL_CTCFLAMMABLE_GETCLOSESTFIREPOINTTO
 // Retail opt-in (p25Q, row 0x00780430; needs FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS): Ego_r.pdb
 // ?GetClosestFirePointTo@CTCFlammable@@AAE?AV?$_Vector_iterator@..NFire::CFirePoint..@@ABVC3DVector@@_N@Z (private; Ego_r 0x005945C0),
 // out of line in retail at 0x00780430 (`ret 8`; called by CTCFlammable::AddFireDamage 0x00780860 with `push 0 ; push pos`). The
 // iterator is retail STLport's raw NFire::CFirePoint*. Ego_d.pdb parameter names pos / unlit_only. Declaration only: no layout change.
private:
 std::vector<NFire::CFirePoint>::iterator GetClosestFirePointTo(const C3DVector& pos, bool unlit_only);   // 0x00780430
public:
#endif
#ifdef FABLE_RETAIL_CTCFLAMMABLE_FRAMEUPDATE
 // Opt-in (hand3c, row 0x00780D80 CTCFlammable::FrameUpdate; needs FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS): the rest of the Ego_r.pdb members
 // (retail offsets, the STLport vector 4 bytes shorter than Ego_r's: Burning +0x1C, FireDamageAfterFrame +0x20, FireResistance +0x24,
 // FireStrength +0x28, DamageCaused +0x2C, DamagePerPoint +0x30, HasFirePoint +0x31; the landed IsOnFire 0x00780400 reads +0x24), PDef
 // typed (CDefPointer<const CFlammableDef>, one word), IsOnFire with its body (FableWin.pdb; inline in FrameUpdate, out of line at
 // 0x00780400), the slot-9 FrameUpdate override and ExtinguishFire 0x00780C80. No layout change.
 bool IsOnFire()const { return FireResistance <= 0; } void SetOnFire();
 virtual void FrameUpdate();                        // slot 9, 0x00780D80
 void ExtinguishFire();                             // 0x00780C80
#ifdef FABLE_RETAIL_FLAMMABLE_INIT_FIRE_POINTS_PASS29
private:
#endif
 CDefPointer<const CFlammableDef> PDef;             // +0x0c
 std::vector<NFire::CFirePoint> FirePoints;         // +0x10
 bool  Burning;                                     // +0x1c
 long  FireDamageAfterFrame;                        // +0x20
 long  FireResistance;                              // +0x24
 long  FireStrength;                                // +0x28
 float DamageCaused;                                // +0x2c
 bool  DamagePerPoint;                              // +0x30
 bool  HasFirePoint;                                // +0x31
#else
 bool IsOnFire()const; void SetOnFire();
#ifdef FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
 const void* PDef;                                  // +0x0c CDefPointer<const CFlammableDef>
 std::vector<NFire::CFirePoint> FirePoints;         // +0x10
#endif
#endif
};
#endif
