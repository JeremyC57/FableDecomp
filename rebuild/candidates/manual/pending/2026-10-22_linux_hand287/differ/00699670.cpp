// Grade: DIFFER(759v761) -- instruction stream nearly identical (same-TU growth stand-ins keep the points finish in ebx, the copy temp for the mode position, cos hoisted inside the loop guard); residue is the frame-slot assignment of the parameter copies / position / vectors (retail range,water,ground at +0x50..+0x58, position +0x2c) plus one fstp scheduled before the lea and one register copy in the inline push_back
// CGameCameraManager::UpdateHeightLimits() -- sample the ground / theme / water heights and line of sight at the observed thing and the camera mode's position, and widen the camera's minimum / maximum height to the highest sample (plus the mode's clearances)
// Leaf: ?UpdateHeightLimits@CGameCameraManager
// Layout: the landed CGameCameraManager::Update 0x0069B790 model (World +0x74, CamTools +0x78, CurrentMode +0x98,
//          PObservedThing +0x100); MinHeight/MaxHeight +0xf8/+0xfc and the height parameters at +0x134 are descriptive.
#include <stdlib.h>
#include <math.h>
#pragma intrinsic(cos)
inline void* operator new(size_t, void* where) { return where; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CThing {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual CThing* GetVehicle();                       // slot 39 (+0x9c)
    unsigned char _pad04[0x8d];
    unsigned char LifetimeFlags;                               // +0x91
    const C3DVector& GetPosition() const;                      // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* VTable;
    void* PointerInfo;
    CThing* GetPItem() const;                                  // 0x00A01B50
};
struct CWorldMap {
    C3DVector GetGroundFaceNormalAt(const C3DVector& position);   // 0x0063AB10
    float GetGroundSizeZAt(const C3DVector& position);         // 0x0063A300
    float GetThemeMinCameraSizeZOffsetAt(const C3DVector& position);   // 0x0063A540
    float GetWaterSizeZAt(const C3DVector& position);          // 0x0063A420
};
struct CWorld {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual CWorldMap* GetWorldMap();                  // slot 13 (+0x34)
};
struct __true_type { __true_type() { } };
struct CThingVector {
    CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(CThing** pos, CThing* const& x, const __true_type&, unsigned int n, bool atend);   // 0x0049C370
    void push_back(CThing* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CThing*(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CVectorVector {
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CVectorVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVectorVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(C3DVector* pos, const C3DVector& x, const __true_type&, unsigned int n, bool atend);   // 0x0066E370
    void push_back(const C3DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DVector(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
#pragma auto_inline(off)
#define GROW_BODY(T)                                                                 \
{                                                                                    \
    T* start = _M_start;                                                             \
    T* finish = _M_finish;                                                           \
    unsigned int size = finish - start;                                              \
    unsigned int len = size + (size > n ? size : n);                                 \
    T* buffer = (T*)malloc(len * sizeof(T));                                         \
    T* cur = buffer;                                                                 \
    for (T* p = start; p != pos; ++p, ++cur)                                         \
        new (cur) T(*p);                                                             \
    for (unsigned int i = 0; i < n; ++i, ++cur)                                      \
        new (cur) T(x);                                                              \
    if (!atend)                                                                      \
        for (T* p = pos; p != finish; ++p, ++cur)                                    \
            new (cur) T(*p);                                                         \
    if (start)                                                                       \
        free(start);                                                                 \
    _M_start = buffer;                                                               \
    _M_finish = cur;                                                                 \
    _M_end_of_storage = buffer + len;                                                \
}
typedef CThing* CThingRaw;
__declspec(noinline) void CThingVector::_M_insert_overflow(CThing** pos, CThing* const& x, const __true_type&, unsigned int n, bool atend)
GROW_BODY(CThingRaw)
__declspec(noinline) void CVectorVector::_M_insert_overflow(C3DVector* pos, const C3DVector& x, const __true_type&, unsigned int n, bool atend)
GROW_BODY(C3DVector)
#pragma auto_inline(on)
struct CTools {
    bool IsLineOfSight(CThingVector* ignore, const C3DVector& from, const C3DVector& to, C3DVector* hit, C3DVector* normal,
                       float* fraction, CThing** hitThing, long test, bool flag);   // 0x00877840
};
struct CCameraMode { unsigned char _pad[0x54]; C3DVector Position; };
struct CCameraHeightParams {
    unsigned char _pad[0x60];
    float GroundClearance;                                     // +0x60
    float WaterClearance;                                      // +0x64
    float HeightRange;                                         // +0x68
    float _6c;
    float FOV;                                                 // +0x70
};
struct CGameCameraManager {
    void* VTable;
    unsigned char _pad04[0x70];
    CWorld* World;                                             // +0x74
    CTools CamTools;                                           // +0x78
    unsigned char _pad79[0x1f];
    CCameraMode* CurrentMode;                                  // +0x98
    unsigned char _pad9c[0x5c];
    float MinHeight;                                           // +0xf8
    float MaxHeight;                                           // +0xfc
    CBaseIntelligentPointer PObservedThing;                    // +0x100
    unsigned char _pad108[0x2c];
    CCameraHeightParams* HeightParams;                         // +0x134
    void UpdateHeightLimits();
};

void CGameCameraManager::UpdateHeightLimits()
{
    CCameraHeightParams* params = HeightParams;
    float waterClearance = params->WaterClearance;
    float groundClearance = params->GroundClearance;
    float range = params->HeightRange;
    float fov = params->FOV * 0.0027777778f;
    const C3DVector& target = PObservedThing.GetPItem()->GetPosition();
    CWorldMap* map = World->GetWorldMap();
    CThing* vehicle = PObservedThing.GetPItem()->GetVehicle();
    CThingVector ignore;
    if (vehicle && !(vehicle->LifetimeFlags & 1))
        ignore.push_back(vehicle);
    C3DVector position = target;
    if (CurrentMode)
        position = CurrentMode->Position;
    float minHeight = target.Z;
    float maxHeight = target.Z + range;
    CVectorVector points;
    points.push_back(target);
    points.push_back(C3DVector(position));
    for (C3DVector* it = points._M_start; it != points._M_finish; ++it) {
        C3DVector normal = map->GetGroundFaceNormalAt(*it);
        if ((normal.X + normal.Y) * 0.0f + normal.Z > cos(fov * 6.2831854820251465)) {
            float ground = map->GetGroundSizeZAt(*it);
            float theme = map->GetThemeMinCameraSizeZOffsetAt(*it) + ground;
            float water = map->GetWaterSizeZAt(*it);
            float height = ground;
            C3DVector flat(it->X, it->Y, 0.0f);
            C3DVector hit;
            C3DVector hitNormal;
            float fraction;
            CThing* hitThing;
            if (!CamTools.IsLineOfSight(&ignore, *it, flat, &hit, &hitNormal, &fraction, &hitThing, 1, false))
                height = hit.Z;
            float base;
            if (height > theme) {
                if (height > water)
                    base = height + groundClearance;
                else
                    base = water + waterClearance;
            }
            else if (theme > water)
                base = theme + groundClearance;
            else
                base = water + waterClearance;
            float top = base + range;
            if (base > minHeight)
                minHeight = base;
            if (top > maxHeight)
                maxHeight = top;
        }
    }
    MinHeight = minHeight;
    MaxHeight = maxHeight;
}
