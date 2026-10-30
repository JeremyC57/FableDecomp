// CTCGhostSword::MakeSwordsAppear() -- summon the ghost swords round the caster: take the facing vector, turn a copy a quarter turn about the up axis and scale it by the creation distance, then add one sword per spell level slot (level 0: ahead; 1: both sides; 2: ahead and both sides; 3: ahead, behind and both sides)
// Leaf: ?MakeSwordsAppear@CTCGhostSword@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C5DB0, 735 bytes of code + `nop` + a 4-entry jump table (row 752), `ret`.
// Identity: Ego_r.pdb has `?MakeSwordsAppear@CTCGhostSword@@QAEXXZ` (donor 0x007C4A50) and the private
//   `?AddSword@CTCGhostSword@@AAEXABVC3DVector@@@Z` (donor 0x007C43E0, retail 0x007C57C0); CTCGhostSword vtable 0x0126E494.
//   Jump table 0x007C6090 checked entry by entry: cases 0..3 -> +0xB4/+0xED/+0x159/+0x1FE, ours the same.
// Callees: CTCPhysicsBase virtual slot 70 GetFacingVector() (+0x118), C3DVector::Rotate(const C3DVector&, float)
//   0x00A13C60, CThing::GetPos() const 0x004C73D0, CTCGhostSword::AddSword(const C3DVector&) 0x007C57C0.
// Layout: Ego_r.pdb CTCGhostSword with the 12-byte STLport vectors (SpellLevel +0x3C); CSpecialAbilitiesGhostSwordDef
//   CreationDistance +0x54 (Ego_r 0x58 less 4: CParentDefClassBase is 0x3C in retail). CThing PhysicsTC +0x60. No engine
//   header for CTCGhostSword; the CTCPhysicsBase slots 28-68 are placeholders (NOTES.md "Descriptive").
// Shape: the sums are written `facing + pos` / `pos - side` to get retail's x87 operand order; `side *= distance` is an
//   inline in-place scale; the cases tail-return separately (retail duplicates the epilogue).
class CCharString;
enum ETCInterfaceType { };                                  // values not needed here
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle);       // 0x00A13C60
    C3DVector& operator*=(float scale) { X *= scale; Y *= scale; Z *= scale; return *this; }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
};
class CTCPhysicsBase;
class CThing { public:                                      // partial view, retail offsets
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    const C3DVector& GetPos() const;                        // 0x004C73D0
};
class CRightHandedSet;
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable { public:     // retail vtable: 28 slots (editor-only virtuals absent)
    virtual ~CTCBase();                                                          // 0  (0x00686830)
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);            // 1
public:
    virtual void OnRemove();                                                     // 2
    virtual void ConstructFromParams(const CTCParamsBase& params);               // 3
    virtual void OnCreate();                                                     // 4
    virtual void InitialActivate();                                              // 5
    virtual void OnInitialActivate();                                            // 6
    virtual void RenderUpdate(float elapsed);                                    // 7
    virtual bool HasFrameUpdate() const;                                         // 8
    virtual void FrameUpdate();                                                  // 9
    virtual void BulletTimeFrameUpdate();                                        // 10
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 11
    virtual void Copy(const CTCBase& other);                                     // 12
    virtual void OnDie();                                                        // 13
    virtual void OnKill();                                                       // 14
    virtual void OnLevelUnload();                                                // 15
    virtual void OnSetInLimbo(bool inLimbo);                                     // 16
    virtual bool Validate(CCharString& error) const;                             // 17
    virtual bool IsFreeForDeletion() const;                                      // 18
    virtual bool IsSerialiseCreatable() const;                                   // 19
    virtual bool IsSerialisableForHeroState() const;                             // 20
    virtual ETCInterfaceType GetInterfaceType() const = 0;                       // 21
    virtual CCharString GetClassTypeName() const = 0;                            // 22
    virtual bool IsComponentSerialisable() const;                                // 23
    virtual bool IsDriver() const;                                               // 24
    virtual bool IsCopyable() const;                                             // 25
    virtual void GlobalUpdate(bool force);                                       // 26
    virtual void HandleMessage(const NTCMessages::CMessageBase& message);        // 27
    CThing* ParentThing;                                                         // +0x04
    bool    ToBeRemoved;                                                         // +0x08
    bool    IsInGlobalUpdateTCs;                                                 // +0x09
};
class CTCPhysicsBase : public CTCBase { public:            // vtable 0x0125D2C4
    // slots 28-68 are not called here; no PDB-proven names in slot order (placeholders, NOTES.md "Descriptive")
    virtual void CUnprovenSlot28_0125d2c4(); virtual void CUnprovenSlot29_0125d2c4(); virtual void CUnprovenSlot30_0125d2c4(); virtual void CUnprovenSlot31_0125d2c4();
    virtual void CUnprovenSlot32_0125d2c4(); virtual void CUnprovenSlot33_0125d2c4(); virtual void CUnprovenSlot34_0125d2c4(); virtual void CUnprovenSlot35_0125d2c4();
    virtual void CUnprovenSlot36_0125d2c4(); virtual void CUnprovenSlot37_0125d2c4(); virtual void CUnprovenSlot38_0125d2c4(); virtual void CUnprovenSlot39_0125d2c4();
    virtual void CUnprovenSlot40_0125d2c4(); virtual void CUnprovenSlot41_0125d2c4(); virtual void CUnprovenSlot42_0125d2c4(); virtual void CUnprovenSlot43_0125d2c4();
    virtual void CUnprovenSlot44_0125d2c4(); virtual void CUnprovenSlot45_0125d2c4(); virtual void CUnprovenSlot46_0125d2c4(); virtual void CUnprovenSlot47_0125d2c4();
    virtual void CUnprovenSlot48_0125d2c4(); virtual void CUnprovenSlot49_0125d2c4(); virtual void CUnprovenSlot50_0125d2c4(); virtual void CUnprovenSlot51_0125d2c4();
    virtual void CUnprovenSlot52_0125d2c4(); virtual void CUnprovenSlot53_0125d2c4(); virtual void CUnprovenSlot54_0125d2c4(); virtual void CUnprovenSlot55_0125d2c4();
    virtual void CUnprovenSlot56_0125d2c4(); virtual void CUnprovenSlot57_0125d2c4(); virtual void CUnprovenSlot58_0125d2c4(); virtual void CUnprovenSlot59_0125d2c4();
    virtual void CUnprovenSlot60_0125d2c4(); virtual void CUnprovenSlot61_0125d2c4(); virtual void CUnprovenSlot62_0125d2c4(); virtual void CUnprovenSlot63_0125d2c4();
    virtual void CUnprovenSlot64_0125d2c4(); virtual void CUnprovenSlot65_0125d2c4(); virtual void CUnprovenSlot66_0125d2c4(); virtual void CUnprovenSlot67_0125d2c4();
    virtual void CUnprovenSlot68_0125d2c4();
    // overloads sit in reverse declaration order: () -> slot 70 (0x006B1410), (float) -> slot 69 (0x006B1430)
    virtual C3DVector GetFacingVector() const;                                   // 70 (+0x118)
    virtual C3DVector GetFacingVector(float time) const;                         // 69 (+0x114)
    virtual CRightHandedSet GetRHSet(float time) const;                          // 71 (+0x11C, 0x006B1450)
    virtual const CRightHandedSet& GetLastRHSet() const;                         // 72 (+0x120, 0x006B1290)
};

class CDefPointeeBase;
class CSpecialAbilitiesGhostSwordDef { public:            // retail offsets (Ego_r less 4)
    unsigned char _pad_0x00[0x3C];                          // CParentDefClassBase (retail 0x3C)
    float         TimeOfSummon[4];                          // +0x3C
    float         FadeInTime;                               // +0x4C
    float         FadeOutTime;                              // +0x50
    float         CreationDistance;                         // +0x54
};
template <class T> class CDefPointer { public:
    T* Object;                                              // +0x00
    T* operator->() const { return Object; }
};
template <class T> class CIntelligentPointer { public:
    void* __vftable;
    void* PData;
};
namespace std {
template <class _Tp> class vector { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CThingAICreature;
class CTCGhostSword : public CTCBase { public:             // vtable 0x0126E494, interface type 0x9A
    CDefPointer<const CSpecialAbilitiesGhostSwordDef>       PDef;               // +0x0C
    std::vector<CIntelligentPointer<CThingAICreature> >     Minions;            // +0x10
    std::vector<CIntelligentPointer<CThing> >               MinionEffects;      // +0x1C
    long                                                    MinionAliveTimer;   // +0x28
    long                                                    FadeInTimer;        // +0x2C
    std::vector<long>                                       CurFadeInTimer;     // +0x30
    long                                                    SpellLevel;         // +0x3C
    CIntelligentPointer<CThing>                             PItemRemoved;       // +0x40
    bool                                                    CutSceneCancelled;  // +0x48
    void MakeSwordsAppear();
private:
    void AddSword(const C3DVector& position);               // 0x007C57C0
};

void CTCGhostSword::MakeSwordsAppear()
{
    CThing* parent = ParentThing;
    C3DVector facing = parent->PhysicsTC->GetFacingVector();
    C3DVector side = facing;
    side.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 0.25f);
    side *= PDef->CreationDistance;
    C3DVector pos = parent->GetPos();
    switch (SpellLevel) {
    case 0:
        AddSword(facing + pos);
        break;
    case 1:
        AddSword(side + pos);
        AddSword(pos - side);
        break;
    case 2:
        AddSword(facing + pos);
        AddSword(side + pos);
        AddSword(pos - side);
        break;
    case 3:
        AddSword(facing + pos);
        AddSword(pos - facing);
        AddSword(side + pos);
        AddSword(pos - side);
        break;
    }
}