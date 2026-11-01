// CTCBob::FrameUpdate() -- bob a ship on the swell: roll and pitch its orientation and lift it with sines of the frame count over the CShipDef wave length, and show it only on its sighting days
// Grade: DIFFER(487v487) -- x87/integer scheduling ties only: in the swell block retail runs `fsin` before `lea eax,pos ; push eax ; mov ecx,ebp` (ours after), and retail stores the frame float (`fstp [esp+10h]`) before the Rhset.Up.Z copy (`mov [esp+34h],ecx`); every instruction, register and frame slot otherwise matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x007877E0, 487 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): RTTI vtable 0x0126B76C (.?AVCTCBob@@) slot 9 (FrameUpdate in every CTCBase vtable); Ego_r.pdb
//   ?FrameUpdate@CTCBob@@UAEXXZ (donor 0x0078EC60, after ??0CTCBob / OnInitialActivate, as retail 0x00787750 / 0x00787780).
// Callees: CThing::GetPBaseDef (inline, the counted copy of PBaseDef +0x70), CDefClassBase::GetSubDef<CShipDef> 0x00787AE0
//   (same retail TU, written here as a same-file auto_inline(off) body = the landed 0x00787AE0 text: only then does VC7.1 drop
//   the null test of the final release and the dead Object=0 store of the early return), CTCBase::GetWorld 0x00686A70 (x2),
//   CWorld::GetFrame 0x0049D870 (static __fastcall: the GetWorld() call only evaluates the object expression),
//   CRightHandedSet::Rotate 0x00A22F80 (x2), CTCPhysicsBase slot 30 SetPosition / slot 75 SetRHSet, CThing slot 28
//   SetAsDrawable(bool), CDefPointeeBase::Delete (slot 1). Constants: 1/15 0x0126B7DC (float), TWO_PI 0x0124F2B8.
// Body: the ship bobs on the swell: roll about Forward by sin(phase)*Roll, pitch about Right (= Forward x Up, inline GetRight)
//   by cos(phase)*Pitch, lift by sin(phase)*Swell, phase = frame / WaveLength / 15 * 2pi; it is drawable when ForceVisible
//   or (DayCount - SightingOffset) % SightingFrequency <= DaysVisible.
// Layout: CShipDef (Ego_r): SightingFrequency +0x28, SightingOffset +0x2c, DaysVisible +0x30, Swell +0x34, Pitch +0x38, Roll
//   +0x3c, WaveLength +0x40 (the shared FABLE_RETAIL_DEF_CLASS_BASE models: CSubDefClassBase is 0x25 packed); CTCBob Rhset +0x0c
//   (Up, Forward), Pos +0x24, ForceVisible +0x30; CThing PhysicsTC +0x60 / PBaseDef +0x70; CWorld PGameTimeManager +0x1c;
//   CGameTimeManager DayCount +0x14. Local CTCBase/CThing/CWorld models (engine/CTCBase.h and engine/CShipDef.h are flat
//   generated layouts without virtuals; NOTES.md header debt).
// Shape: the pitch phase is a named float computed before a named `right` vector (only then does VC7.1 keep fld/fdiv/fmul
//   ahead of the cross product as retail); probe log probe/007877e0.log (22 probes).
// Next: the remaining ties look like scheduler choices; try the swell as an inline helper returning float, or a CTCBase
//   GetWorld().GetGameTimeManager() read moved; else lane gap: x87 scheduling tie.
// Leaf: ?FrameUpdate@CTCBob@@UAEXXZ
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
#pragma intrinsic(sin, cos)
extern const double TWO_PI;                                  // 0x0124F2B8

class CShipDef : public CSubDefClassBase {                    // RTTI .?AVCShipDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CShipDef", -1); }   // ?StatGetTypeName@CShipDef@@SI?AVCCharString@@XZ (inline)
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long  SightingFrequency;                                  // +0x28
    long  SightingOffset;                                     // +0x2c
    long  DaysVisible;                                        // +0x30
    float Swell;                                              // +0x34
    float Pitch;                                              // +0x38
    float Roll;                                               // +0x3c
    float WaveLength;                                         // +0x40
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
#pragma auto_inline(off)
template <>
bool CDefClassBase::GetSubDef<CShipDef>(CDefPointer<const CShipDef>* out) const    // 0x00787AE0 (landed body, same retail TU)
{
    const CSubDefInfo* info = GetPSubDefInfo(CShipDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThingBaseDef : public CDefClassBase {
public:
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y,
                     a.Z * b.X - a.X * b.Z,
                     a.X * b.Y - a.Y * b.X);
}
class CRightHandedSet { public:
    C3DVector Up;                                             // +0x00
    C3DVector Forward;                                        // +0x0c
    CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) { }   // Ego_r ??0CRightHandedSet@@QAE@ABV0@@Z
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }
    void Rotate(const C3DVector& axis, float angle);          // 0x00A22F80
};

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);     // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rhset);     // slot 75 (+0x12c)
};

class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetAsDrawable(bool drawable);               // slot 28 (+0x70)
    unsigned char   _pad04[0x60 - 4];
    CTCPhysicsBase* PhysicsTC;                                // +0x60 (Ego_r +0x70)
    unsigned char   _pad64[0x70 - 0x64];
    CDefPointer<const CThingBaseDef> PBaseDef;                // +0x70
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
};

class CGameTimeManager { public:
    unsigned char _pad00[0x14];
    long DayCount;                                            // +0x14
    long GetDayCount() const { return DayCount; }
};
class CWorld { public:
    unsigned char _pad00[0x1c];
    CGameTimeManager* PGameTimeManager;                       // +0x1c scoped_ptr<CGameTimeManager>
    static long __fastcall GetFrame();                        // 0x0049D870
    CGameTimeManager& GetGameTimeManager() { return *PGameTimeManager; }
};

class CTCBase { public:
    virtual ~CTCBase();
    CThing*       ParentThing;                                // +0x04
    bool          ToBeRemoved;                                // +0x08
    bool          IsInGlobalUpdateTCs;                        // +0x09
protected:
    CWorld& GetWorld() const;                                 // 0x00686A70
};

class CTCBob : public CTCBase { public:                       // vtable 0x0126B76C
    CRightHandedSet Rhset;                                    // +0x0c
    C3DVector       Pos;                                      // +0x24
    bool            ForceVisible;                             // +0x30
    virtual void FrameUpdate();
};
void CTCBob::FrameUpdate()
{
    CDefPointer<const CShipDef> shipDef;
    if (!ParentThing->GetPBaseDef()->GetSubDef(&shipDef))
        return;
    float frame = (float)GetWorld().GetFrame();
    CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
    CRightHandedSet rhset = Rhset;
    C3DVector pos = Pos;
    rhset.Rotate(rhset.Forward, sin(frame / shipDef->WaveLength * (1.0f / 15.0f) * TWO_PI) * shipDef->Roll);
    float phase = frame / shipDef->WaveLength * (1.0f / 15.0f);
    C3DVector right = rhset.GetRight();
    rhset.Rotate(right, cos(phase * TWO_PI) * shipDef->Pitch);
    pos.Z += shipDef->Swell * sin(frame / shipDef->WaveLength * (1.0f / 15.0f) * TWO_PI);
    physics->SetPosition(pos);
    physics->SetRHSet(rhset);
    CWorld& world = GetWorld();
    bool drawable = ForceVisible || (world.GetGameTimeManager().GetDayCount() - shipDef->SightingOffset) % shipDef->SightingFrequency <= shipDef->DaysVisible;
    ParentThing->SetAsDrawable(drawable);
}
