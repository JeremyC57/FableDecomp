// Grade: DIFFER(928v982) 214 of 303 lines (was 223): the TC map through a reference local (retail keeps &thing->TCs in esi) and animsCanBeIgnored declared first; residue: frame 0xD0 v 0xBC, this in edi (retail ebp, spilled to [esp+30h]), flags byte vs dword, the mesh-loop zero register
// CTCGraphicAppearance::GetDummyObjectAssumingObjectAt(const CCharString& name, const C3DVector* atPos, const CRightHandedSet* atOrientation, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* orientation) -- as GetDummyObject, but with the meshes placed at the given position and orientation (animated lookups at time 1)
#include "engine/CCountedPointer.h"
#include <stdlib.h>
class CCharString {
public:
    void* Data;
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
    __forceinline CRightHandedSet() {}
    __forceinline CRightHandedSet(const C3DVector& forward, const C3DVector& up, int) : Forward(forward), Up(up) {}
    CRightHandedSet(const C3DVector& forward, const C3DVector& up);   // 0x004BBE10
};
struct CMeshEntry {
    long MeshIndex;
    long Data[2];
};
struct CMeshEntryLess {};
struct CMeshEntryVector {
    CMeshEntry* _M_start;
    CMeshEntry* _M_finish;
    CMeshEntry* _M_end_of_storage;
    __forceinline CMeshEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~CMeshEntryVector()
    {
        if (_M_start)
            free(_M_start);
    }
    void reserve(unsigned n);                           // 0x004C46C0
};
void __fastcall SortMeshesFrontToBack(CMeshEntry* first, CMeshEntry* last, CMeshEntryLess comp);   // 0x004C5C80
void __fastcall SortMeshesBackToFront(CMeshEntry* first, CMeshEntry* last, CMeshEntryLess comp);   // 0x004C5CE0
class C3DMeshInfo {
public:
    bool IsLoaded() const;                              // 0x00A23E40
    void BeginLoading(long priority, long a, long b);   // 0x00A256F0
};
class C3DMeshManager {
public:
    C3DMeshInfo* GetMeshInfo(long index);               // 0x00A24260
};
extern C3DMeshManager* g_MeshManager;                   // 0x013B8A04
class CBase {
public:
    CBase();                                            // 0x0099A2D0
    ~CBase();                                           // 0x0099A2E0
};
struct C3DMeshStats {
    unsigned char _pad00[0x7a];
    bool Animated;                                      // +0x7A
};
class CCPPointee {};


struct CMatrix3x4 { float M[12]; };
struct C3DAnimationBlendState { void* vptr; void* Data; };
class C3DAnimationInterpolator {
public:
    void* vptr;
    C3DAnimationBlendState StartState;
    C3DAnimationBlendState EndState;
    C3DAnimationInterpolator();                         // 0x004C2E20
    ~C3DAnimationInterpolator();                        // 0x004C2E90
};
class C3DMeshObject : public CBase {
public:
    C3DMeshInfo* MeshInfo;                              // +0x04
    CCountedPointer<C3DMeshStats> Mesh;                 // +0x08
    CMatrix3x4 PositionMatrix;                          // +0x10
    bool InversePositionMatrixSetup;                    // +0x40
    CMatrix3x4 InversePositionMatrix;                   // +0x44
    float Scale;                                        // +0x74
    __forceinline C3DMeshObject() : InversePositionMatrixSetup(false) {}
    virtual ~C3DMeshObject();                           // 0x004C3200
    bool HasDummyObject(const CCharString& name) const;          // 0x00A36430
    long GetDummyObjectCount(const CCharString& name) const;     // 0x00A36480
    void GetDummyObjectPositionStatic(const CCharString& name, long index, C3DVector* pos, C3DVector* up, C3DVector* forward) const;   // 0x00A364E0
    void GetDummyObjectPositionAnimated(const CCharString& name, long index, const C3DAnimationBlendState& start,
                                        const C3DAnimationBlendState& end, float time, long frame, C3DVector* pos,
                                        C3DVector* up, C3DVector* forward, long flags) const;   // 0x00A36A80
};
class CThing;
class CThingPhysics {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b(); virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c(); virtual void v1d(); virtual void v1e(); virtual void v1f();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v2a(); virtual void v2b(); virtual void v2c(); virtual void v2d(); virtual void v2e(); virtual void v2f();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v3a(); virtual void v3b(); virtual void v3c(); virtual void v3d(); virtual void v3e(); virtual void v3f();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
};
C3DVector __fastcall GetThingInterpolatedPosition(CThing* thing, float t);   // 0x004BBF00
class CAnimateThing {
public:
    bool AnimsCanBeIgnored() const;                     // 0x0070C100
};
class CTCAnimate {
public:
    CAnimateThing* GetAnimateThing();                   // 0x0070B470
};
struct CTCPair { long Type; CTCAnimate* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CTCMap TCs;                                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    unsigned char _pad50[0x60 - 0x50];
    class CThingPhysics* Physics;                       // +0x60
};
class CThingPlacement {
public:
    virtual void v00();
    virtual const CRightHandedSet& GetOrientation() const;   // stand-in name; the real slot is +0x120
};
class CMeshProvider {
public:
    virtual void v00();
    virtual void v01();
    virtual void GetMeshes(CMeshEntryVector* meshes);   // slot 2
};
struct CProviderNode {
    CProviderNode* _M_next;
    CProviderNode* _M_prev;
    CMeshProvider* _M_data;
};
struct CDisplayEngine {
    unsigned char _pad00[0x44];
    long Frame;                                         // +0x44
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    CDisplayEngine* GetDisplayEngine() const;           // 0x00686A80
};
class CTCGraphicAppearance : public CTCBase {
public:
    unsigned char _pad0c[0x3c - 0x0c];
    CProviderNode* Providers;                           // +0x3C (list header node)
    bool IsToReduceLODForDummiesAndHelpers() const;     // 0x004BD2E0
    void GetAnims(C3DAnimationInterpolator* interp);    // 0x004BC000
    void CreateAbilityMeshObject(const CMeshEntry& entry, float time, C3DMeshObject* object);   // 0x004BC4F0
    void GetMeshObjectAssumingObjectAt(const CMeshEntry& entry, const C3DVector* pos, const CRightHandedSet* orientation, C3DMeshObject* object);   // 0x004BD160
    bool GetDummyObjectAssumingObjectAt(const CCharString& name, const C3DVector* atPos, const CRightHandedSet* atOrientation, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* orientation);
};

bool CTCGraphicAppearance::GetDummyObjectAssumingObjectAt(const CCharString& name, const C3DVector* atPos, const CRightHandedSet* atOrientation, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* orientation)
{
    bool fullDetail = true;
    if (flags & 1)
        fullDetail = false;
    bool animsCanBeIgnored = false;
    bool staticOnly = false;
    if (flags & 4)
        staticOnly = true;
    if (!fullDetail && IsToReduceLODForDummiesAndHelpers()) {
        CThing* thing = Thing;
        if (thing->HasTC(0x5a)) {
            long key = 0x5a;
            CTCMap& tcs = thing->TCs;
            CTCPair* it = tcs.LowerBound(key);
            if (it == tcs._M_finish || it->Type > 0x5a)
                it = tcs._M_finish;
            if (it->TC->GetAnimateThing()->AnimsCanBeIgnored())
                animsCanBeIgnored = true;
        }
    }
    CMeshEntryVector meshes;
    meshes.reserve(4);
    for (CProviderNode* node = Providers->_M_next; node != Providers; node = node->_M_next)
        node->_M_data->GetMeshes(&meshes);
    if (flags & 8)
        SortMeshesFrontToBack(meshes._M_start, meshes._M_finish, CMeshEntryLess());
    else if (flags & 0x10)
        SortMeshesBackToFront(meshes._M_start, meshes._M_finish, CMeshEntryLess());
    for (CMeshEntry* entry = meshes._M_start; entry != meshes._M_finish; ++entry) {
        if (!entry->MeshIndex)
            continue;
        C3DMeshInfo* info = g_MeshManager->GetMeshInfo(entry->MeshIndex);
        if (!info->IsLoaded()) {
            if (flags & 2)
                return false;
            info->BeginLoading(-1, 0, 0);
        }
        C3DMeshObject object;
        GetMeshObjectAssumingObjectAt(*entry, atPos, atOrientation, &object);
        if (object.HasDummyObject(name)) {
            if (index >= object.GetDummyObjectCount(name))
                return false;
            if (object.Mesh.Data->Animated && !staticOnly) {
                if (animsCanBeIgnored) {
                    *pos = GetThingInterpolatedPosition(Thing, 1.0f);
                    *orientation = Thing->Physics->GetOrientation();
                } else {
                    C3DAnimationInterpolator interp;
                    GetAnims(&interp);
                    C3DVector up, forward;
                    object.GetDummyObjectPositionAnimated(name, index, interp.StartState, interp.EndState, 1.0f, GetDisplayEngine()->Frame, pos, &up, &forward, 0);
                    *orientation = CRightHandedSet(forward, up);
                }
            } else {
                C3DVector up, forward;
                object.GetDummyObjectPositionStatic(name, index, pos, &up, &forward);
                *orientation = CRightHandedSet(forward, up, 0);
            }
            return true;
        }
    }
    return false;
}
