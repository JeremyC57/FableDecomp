// Grade: DIFFER(632v630) -- frame slots and register picks: retail homes the beam end at +20h and the hit position at +2Ch (ours swapped), and loads the tools source into ecx before the out-pointer push; best probe/db8/k.cpp, with 15 variants tried.
// NCreatureMode::CModeDragonFireBeam::UpdateBeam()
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
    long          BeamExplodeInterval;          // +0x5c
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
struct CTools {
    bool GetLandscapePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool includeWater);   // 0x00875550
};
struct CToolsHolder { unsigned char _pad[0x78]; CTools Tools; };   // CTools at +0x78
struct CToolsSource {
    virtual void _v0(); virtual void _v1(); virtual void _v2(); virtual void _v3(); virtual void _v4();
    virtual void _v5(); virtual void _v6(); virtual void _v7(); virtual void _v8(); virtual void _v9();
    virtual bool GetToolsHolder(CToolsHolder** out);   // slot 10 (+0x28)
};
struct CWorld { unsigned char _pad[0x34]; CToolsSource* P34; };
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
class CModeDragonFireBeam {
public:
    void*         _vptr;                        // +0x00
    CThing*       Owner;                        // +0x04
    unsigned char _pad08[0x80 - 0x08];
    long          Timer;                        // +0x80
    unsigned char PBeamWeaponEffect[8];         // +0x84
    long          LastExplodeFrame;             // +0x8c
    bool          BeamActive;                   // +0x90
    CWorld* GetWorld();                         // 0x0073A680
    void UpdateBeam();
};
void CModeDragonFireBeam::UpdateBeam()
{
    if (!BeamActive)
        return;
    CDragonDefPointer slot;
    Owner->PDef->GetDragonDef(&slot);
    CDragonDef* def = slot.m_p;
    if (GetWorldFrame() - LastExplodeFrame > def->BeamExplodeInterval) {
        C3DVector mouth;
        CRightHandedSet orientation;
        {
            CCharString dummy("DRAGON_MOUTH_DUMMY");
            Owner->PGraphic->GetDummyObject(&dummy, 0, 0, &mouth, &orientation, 1.0f);
        }
        C3DVector direction = orientation.Forward * -1.0f;
        C3DVector end = mouth + direction * 1000.0f;
        CToolsHolder* holder;
        if (GetWorld()->P34->GetToolsHolder(&holder)) {
            C3DVector hitPos;
            C3DVector hitNormal;
            if (holder->Tools.GetLandscapePointIntersectionFromTo(mouth, end, hitPos, hitNormal, true)) {
                long index;
                {
                    CCharString name("DRAGON_FIREBEAM_EXPLOSION");
                    index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
                }
                CThing* explosion = CreateExplosionObject(index, &hitPos);
                if (explosion) {
                    CTCDependents* dependents;
                    if (explosion->GetInterface(&dependents, 0x41))
                        dependents->AddDependentThing(Owner);
                    LastExplodeFrame = GetWorldFrame();
                }
                C3DVector middle = (hitPos + mouth) * 0.5f;
                {
                    CCharString name("DRAGON_FIREBEAM_EXPLOSION_INVISIBLE");
                    index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
                }
                CThing* invisible = CreateExplosionObject(index, &middle);
                if (invisible) {
                    CTCDependents* dependents;
                    if (invisible->GetInterface(&dependents, 0x41))
                        dependents->AddDependentThing(Owner);
                }
            }
        }
    }
    if (--def->m_refCount == 0)
        def->Destroy();
}
}
