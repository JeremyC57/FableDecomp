// NCreatureMode::CModeDragonStrafeBurninate::UpdateBeam(float progress) -- while the flame is active and the def's
// explode interval has passed since the last explosion: place a point along the flame line by how far progress is between
// the start and stop percentages, and when that point is on the world map drop it to the ground, create a
// DRAGON_BURNINATE_EXPLOSION object there, register it as a dependent of the dragon (type-0x41 component) and restamp
// the explosion frame
// Leaf: UpdateBeam
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC900, 0x1F6 bytes (padded to 512), `ret 4`.
// Callees: the dragon def getter 0x008E3300 (GetSubDef-shaped, out CDefPointer), the world-frame getter 0x0049D870
//   (__fastcall), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CTCGraphicAppearance::GetDummyObject 0x004C08C0,
//   CModeBase::GetDisplayEngine 0x0073A680 (its virtual slots 4 and 3, each with the CWorldMap at +0x28),
//   CWorldMap::IsPosInMap 0x004FC600 / GetHeightZBelowPos 0x0063E480, CGameDefinitionManager::Get 0x0044C6B0 (static
//   __fastcall) / GetDefGlobalIndexFromName 0x009AD410, the object Create 0x0080C460 (__fastcall), CVectorMap::LowerBound
//   0x0040F020 (inline lookup 0x41), AddDependentThing 0x0080CBF0; def release via slot 1.
// Layout: local model; the mode is the Ego_r PDB layout 4 bytes lower (LastExplodeFrame +0xA0, FlameStartPos +0xA4,
//   FlameEndPos +0xB0, FlameActive +0xBC, FlameStartPercentage +0xC0, FlameStopPercentage +0xC4); the def's explode
//   interval at +0x58.
// Shape: same-TU out-only stand-in for the dragon def getter (the out slot stays private, so the name temp reuses it);
//   both world maps named in locals before the IsPosInMap / GetHeightZBelowPos calls (retail evaluates `this` first);
//   the dependents component through the out-parameter GetInterface ternary (`mov eax,[eax+4] ; push ; mov ecx,eax`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CRightHandedSet { C3DVector Forward, Up; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CDragonDef : CDefBase {
    unsigned char _pad08[0x58 - 0x08];
    long          ExplodeInterval;              // +0x58
};
struct CDragonDefPointer {
    CDragonDef* m_p;
    CDragonDefPointer() : m_p(0) { }
};
struct CDefClassBase : CDefBase {
    bool GetDragonDef(CDragonDefPointer* out);  // 0x008E3300
};
void* FindDef_CDefClassBase_GetDragonDef(const void* owner);   // stand-in callee
// Same-TU stand-in for the GetSubDef<CDragonDef> instantiation (0x008E3300, a template member compiled in the caller's
// TU): it writes the out slot only, so the def pointer's address does not escape.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetDragonDef(CDragonDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetDragonDef(this);
    if (!def)
        return false;
    out->m_p = (CDragonDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString* name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CWorldMap {
    bool IsPosInMap(const C3DVector* position);             // 0x004FC600
    float GetHeightZBelowPos(const C3DVector* position);    // 0x0063E480
};
struct CWorldHolder { unsigned char _pad[0x28]; CWorldMap* PWorldMap; };
struct CDisplayEngine {
    virtual void _v0(); virtual void _v1(); virtual void _v2();
    virtual CWorldHolder* GetWorldB();          // slot 3
    virtual CWorldHolder* GetWorldA();          // slot 4
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
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

struct CThing;
struct CTCDependents {
    void AddDependentThing(CThing* thing);      // 0x0080CBF0
};
struct CThing {
    unsigned char         _pad0[0x20];
    unsigned long         m_presentBits[9];     // +0x20
    CInterfaceMap         m_interfaces;         // +0x44
    unsigned char         _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphic;             // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PDef;                 // +0x70
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);            // 0x009AD410
};
long __fastcall GetWorldFrame();                                        // 0x0049D870
CThing* __fastcall CreateExplosionObject(long defIndex, const C3DVector* position);   // 0x0080C460
namespace NCreatureMode {
class CModeDragonStrafeBurninate {
public:
    void*         _vptr;                        // +0x00
    CThing*       Owner;                        // +0x04
    unsigned char _pad08[0xa0 - 0x08];
    long          LastExplodeFrame;             // +0xA0
    C3DVector     FlameStartPos;                // +0xA4
    C3DVector     FlameEndPos;                  // +0xB0
    bool          FlameActive;                  // +0xBC
    float         FlameStartPercentage;         // +0xC0
    float         FlameStopPercentage;          // +0xC4
    CDisplayEngine* GetDisplayEngine();         // 0x0073A680
    void UpdateBeam(float progress);
};
void CModeDragonStrafeBurninate::UpdateBeam(float progress)
{
    if (!FlameActive)
        return;
    CDragonDefPointer slot;
    Owner->PDef->GetDragonDef(&slot);
    CDragonDef* def = slot.m_p;
    if (GetWorldFrame() - LastExplodeFrame > def->ExplodeInterval) {
        float fraction = (progress - FlameStartPercentage) / (FlameStopPercentage - FlameStartPercentage);
        C3DVector mouth;
        CRightHandedSet orientation;
        {
            CCharString dummy("DRAGON_MOUTH_DUMMY");
            Owner->PGraphic->GetDummyObject(&dummy, 0, 0, &mouth, &orientation, 1.0f);
        }
        C3DVector position;
        position = (FlameEndPos - FlameStartPos) * fraction + FlameStartPos;
        CWorldMap* mapA = GetDisplayEngine()->GetWorldA()->PWorldMap;
        if (mapA->IsPosInMap(&position)) {
            CWorldMap* mapB = GetDisplayEngine()->GetWorldB()->PWorldMap;
            position.Z = mapB->GetHeightZBelowPos(&position);
            long index;
            {
                CCharString name("DRAGON_BURNINATE_EXPLOSION");
                index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
            }
            CThing* explosion = CreateExplosionObject(index, &position);
            if (explosion) {
                CTCDependents* dependents;
                if (explosion->GetInterface(&dependents, 0x41))
                    dependents->AddDependentThing(Owner);
                LastExplodeFrame = GetWorldFrame();
            }
        }
    }
    if (--def->m_refCount == 0)
        def->Destroy();
}
}