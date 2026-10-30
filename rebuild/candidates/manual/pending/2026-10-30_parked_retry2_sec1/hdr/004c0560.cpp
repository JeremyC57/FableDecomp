// Grade: DIFFER(786v851) -- first pass: control flow, callees and the mesh-object/interpolator layout are modelled; retail inlines the C3DMeshObject destructor at the loop's continue path (counted-pointer release + operator delete) and calls it out of line elsewhere, keeps this in a frame word, value-initialises the sort comparator temporaries and orders the three flag bools differently
// CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* out, float time) -- finds the first loaded mesh of the appearance carrying the named helper and returns the helper's position (animated, static or the thing's own position for ignorable LOD anims)
#include "engine/CCountedPointer.h"
#include <stdlib.h>
class CCharString {
public:
    void* Data;
};
struct C3DVector { float X, Y, Z; };
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
    bool HasHelper(const CCharString& name) const;      // 0x00A36100
    long GetHelperCount(const CCharString& name) const; // 0x00A36140
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;   // 0x00A361A0
    C3DVector GetHelperPointPosition(const CCharString& name, long index, const C3DAnimationBlendState& start,
                                     const C3DAnimationBlendState& end, float time, long frame) const;   // 0x00A36350
};
class CThing;
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
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* out, float time);
};

bool CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* out, float time)
{
    bool fullDetail = true;
    bool animsCanBeIgnored = false;
    if (flags & 1)
        fullDetail = false;
    bool staticOnly = false;
    if (flags & 4)
        staticOnly = true;
    if (!fullDetail && IsToReduceLODForDummiesAndHelpers()) {
        CThing* thing = Thing;
        if (thing->HasTC(0x5a)) {
            long key = 0x5a;
            CTCPair* it = thing->TCs.LowerBound(key);
            if (it == thing->TCs._M_finish || it->Type > 0x5a)
                it = thing->TCs._M_finish;
            if (it->TC->GetAnimateThing()->AnimsCanBeIgnored())
                animsCanBeIgnored = true;
        }
    }
    CMeshEntryVector meshes;
    meshes.reserve(4);
    for (CProviderNode* node = Providers->_M_next; node != Providers; node = node->_M_next)
        node->_M_data->GetMeshes(&meshes);
    if (flags & 8) {
        CMeshEntryLess less;
        SortMeshesFrontToBack(meshes._M_start, meshes._M_finish, less);
    } else if (flags & 0x10) {
        CMeshEntryLess less;
        SortMeshesBackToFront(meshes._M_start, meshes._M_finish, less);
    }
    for (CMeshEntry* entry = meshes._M_start; entry != meshes._M_finish; ++entry) {
        if (!entry->MeshIndex)
            continue;
        bool load = !(flags & 2);
        bool mustBeLoaded = true;
        if (load && !(flags & 0x20))
            mustBeLoaded = false;
        C3DMeshInfo* info = g_MeshManager->GetMeshInfo(entry->MeshIndex);
        if (!info->IsLoaded()) {
            if (load)
                info->BeginLoading(-1, 0, 0);
            if (mustBeLoaded)
                return false;
        }
        C3DMeshObject object;
        CreateAbilityMeshObject(*entry, time, &object);
        if (object.HasHelper(name)) {
            if (index >= object.GetHelperCount(name))
                return false;
            if (object.Mesh.Data->Animated && !staticOnly) {
                if (animsCanBeIgnored) {
                    *out = GetThingInterpolatedPosition(Thing, 1.0f);
                } else {
                    C3DAnimationInterpolator interp;
                    GetAnims(&interp);
                    *out = object.GetHelperPointPosition(name, index, interp.StartState, interp.EndState, time, GetDisplayEngine()->Frame);
                }
            } else {
                *out = object.GetHelperPointPositionStatic(name, index);
            }
            return true;
        }
    }
    return false;
}
