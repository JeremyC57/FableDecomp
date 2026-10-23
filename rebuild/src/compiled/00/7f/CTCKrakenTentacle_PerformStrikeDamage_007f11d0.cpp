// CTCKrakenTentacle::PerformStrikeDamage() -- if the kraken is alive and has a live target: turn the tentacle's facing by the def's explosion bodge angle, spawn the KRAKEN_TENTACLE_WHACK explosion that far along it (at the target's height) and hand it to the kraken, then replace the tentacle's physical obstruction with one spanning the def's min..max obstruction radii; count the strike; always clear CanStrike and the strike timer
// Leaf: PerformStrikeDamage
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F11D0, 575 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (CIntelligentPointer<CThing> vtable
//   0x01238C6C), LowerBound @ 0x0040F020 (interface 0xE2 = CTCKraken), the physics TC's GetFacingVector (slot 70, +0x118,
//   C3DVector by value), CThing::GetPosition @ 0x004C73D0, C3DVector::RotateInPlace @ 0x00A14040, CCharString
//   @ 0x0099EBF0/0x0099EAE0, CTCBase::GetDefinitions @ 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//   CTCDExplosion::Create @ 0x0080C460 (__fastcall), CTCKraken::AddStrikeExplosion @ 0x007FBA20, CTCKrakenTentacle::
//   RemoveObstruction @ 0x007F0810, CTCDPhysicalObstruction::Create @ 0x007FD4B0 (__fastcall).
// Data: "KRAKEN_TENTACLE_WHACK" 0x01271D1C, 1/360 0x01238E00.
// Layout: Ego_r.pdb CTCKrakenTentacle (PTentacleDef +0x0C, PKraken +0x10, PObstruction +0x18, CanStrike +0x28, the strike
//   timer +0x2C); CKrakenTentacleDef ObstructionRadius +0x4C .. ObstructionRadiusMax +0x5C (PDB); CTCKraken PTarget +0x20,
//   TentacleStrikeCounter retail +0x4C (PDB +0x5C: the two STLport lists before it are 4 bytes in retail, 12 in the PDB).
// Shape: the owner position is a C3DVector value copy (only X/Y reach the frame) and the origin is built (pos.X, pos.Y, 0)
//   then its Z assigned from the target, which keeps the target height on the x87 stack across the three points (popped
//   after the last) as retail; a C3DVector(x, y, z) constructor consumes it on the last point.
#include "engine/CTCBase.h"


struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void RotateInPlace(float fraction);                     // 0x00A14040
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z = s * Z; return *this; }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector operator*(float s) const { C3DVector r; r.X = X * s; r.Y = Y * s; r.Z = Z * s; return r; }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

struct CPhysicsBody {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();                       // slot 63 (+0xFC)
    virtual void v64(); virtual void v65();
    virtual void SetFacingAngleXY(float angle);             // slot 66 (+0x108)
    virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector() const;              // slot 70 (+0x118)
};

class CTCBaseComponent;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (as the landed 0x007F0B10): with it
// VC7.1 sees the key's address does not escape, so the key shares the facing angle's frame word as in retail.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CInterfaceBits {
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
struct CThing {
    unsigned char _pad00[0x20];
    CInterfaceBits present;                                 // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* m_pPhysics;                               // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->Flags91 & 1))
            return true;
    }
    return false;
}

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
#pragma pack(pop)

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CTCDExplosion {
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);   // 0x0080C460
};
struct CTCDPhysicalObstruction {
    static CThing* __fastcall Create(const C3DVector& start, const C3DVector& end, float radius);   // 0x007FD4B0
};
struct CTCBaseDefs : CTCBase {
    CDefinitionManager* GetDefinitions();                   // 0x006869D0 (CTCBase)
};
struct CKrakenTentacleDef {
    unsigned char _pad00[0x4c];
    float TentacleObstructionRadius;                        // +0x4c
    float TentacleExplosionBodgeAngle;                      // +0x50
    float TentacleExplosionRadius;                          // +0x54
    float TentacleObstructionRadiusMin;                     // +0x58
    float TentacleObstructionRadiusMax;                     // +0x5c
};
struct CTCKraken : CTCBase {
    const void* PKrakenDef;                                 // +0x0c (CDefPointer)
    CBaseIntelligentPointer PChamber;                       // +0x10
    CBaseIntelligentPointer PCentreMarker;                  // +0x18
    CBaseIntelligentPointer PTarget;                        // +0x20
    unsigned char _pad28[0x4c - 0x28];
    long TentacleStrikeCounter;                             // +0x4c (PDB +0x5c; retail STLport lists are 4 bytes)
    void AddStrikeExplosion(CThing* explosion);             // 0x007FBA20
};
struct CTCKrakenTentacle : CTCBaseDefs {
    const CKrakenTentacleDef* PTentacleDef;                 // +0x0c (CDefPointer)
    CBaseIntelligentPointer PKraken;                        // +0x10
    CIntelligentPointer<CThing> PObstruction;               // +0x18
    CBaseIntelligentPointer PRippleEffect;                  // +0x20
    bool CanStrike;                                         // +0x28
    long StrikeTimer;                                       // +0x2c
    void RemoveObstruction();                               // 0x007F0810
    void PerformStrikeDamage();
};

void CTCKrakenTentacle::PerformStrikeDamage()
{
    if (IS_THING_ALIVE(PKraken)) {
        CTCKraken* kraken;
        PKraken.GetPItem()->GetInterface(&kraken, 0xe2);
        CThing* target = kraken->PTarget.GetPItem();
        if (IS_THING_ALIVE(target)) {
            C3DVector facing = ParentThing->m_pPhysics->GetFacingVector();
            C3DVector pos = ParentThing->GetPosition();
            const CKrakenTentacleDef* def = PTentacleDef;
            facing.RotateInPlace(def->TentacleExplosionBodgeAngle * 0.0027777778f);
            C3DVector origin(pos.X, pos.Y, 0.0f);
            origin.Z = target->GetPosition().Z;
            C3DVector obstructionStart = facing * def->TentacleObstructionRadiusMin + origin;
            C3DVector obstructionEnd = facing * def->TentacleObstructionRadiusMax + origin;
            C3DVector explosionPos = facing * def->TentacleExplosionRadius + origin;
            CThing* explosion = CTCDExplosion::Create(GetDefinitions()->GetDefGlobalIndexFromName("KRAKEN_TENTACLE_WHACK"), explosionPos);
            if (explosion)
                kraken->AddStrikeExplosion(explosion);
            RemoveObstruction();
            PObstruction = CIntelligentPointer<CThing>(CTCDPhysicalObstruction::Create(obstructionStart, obstructionEnd, def->TentacleObstructionRadius));
        }
        ++kraken->TentacleStrikeCounter;
    }
    CanStrike = false;
    StrikeTimer = 0;
}