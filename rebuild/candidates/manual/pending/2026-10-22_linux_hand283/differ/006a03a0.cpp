// CCreatureAction_UseObject::Initialise() -- after the base Initialise, pick where the creature stands to use the
// Grade: DIFFER(ours v retail) -- 827v825 (first pass, 6 grades): frame 0x44 v 0x48 (retail keeps one more word), zero held in edi not ebp, and the creature C2DCoordI converts X before Y (retail: Y to a temp first); loop, compares and copies otherwise in place
//   object: from the object's sorted "use" animations take the end position/orientation of the one whose end point is
//   nearest the creature (repeats of one name are counted as variants); without animations ask the object's physics
//   for an approach point and widen the approach radius by the creature's diameter. Then decide whether the creature
//   is already there (2D distance under the diameter and height within 2.5) and reset the per-use flags.
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A03A0, length 832 bytes, plain `ret` (virtual).
// Callees: CCreatureActionBase::Initialise 0x00694D10, GetPItem 0x00A01B50, CTCGraphicAppearance::GetAnimationNames
//   (vector<CCharString>*, const CCharString&) 0x004BDD70 / GetAnimationEnd(name, variant, flags, C3DVector*,
//   CRightHandedSet*, float) 0x004C08C0, string sort 0x005E3A50 (__fastcall), Compare 0x00411570 (__fastcall),
//   CThing::GetPosition 0x004C73D0, CTCPhysicsBase approach point 0x006AF640, static creature radius 0x00661CC0,
//   _ftol 0x00BFEA70, C2DCoordI distance 0x009E2430 (__fastcall), ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Layout (retail bytes): action +0x08 PCreature, +0x10/+0x14 longs, +0xa8 PObject, +0xb0 long, +0xb4 CRightHandedSet,
//   +0xcc C3DVector UsePosition, +0xd8 float ApproachRadius, +0xdc..+0xdf flags; thing +0x60 physics, +0x64 graphic.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
struct CCharStringData {
    char*         PData;
    unsigned long Length;
};
long __fastcall Compare(const char* a, const char* b);       // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                          // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->PData, other.PStringData->PData) ? false : true;
        return false;
    }
};
extern CCharString g_UseAnimationPrefix;                     // 0x013BAC9C
struct CStringVector {                                       // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CStringVector()
    {
        for (CCharString* it = _M_start; it != _M_finish; ++it)
            it->~CCharString();
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
};
void __fastcall SortStrings(CCharString* first, CCharString* last);   // 0x005E3A50

struct C3DVector {
    float X, Y, Z;
};
struct CRightHandedSet {
    C3DVector Up, Forward;
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float LengthSquared() const { return Y * Y + X * X; }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
    C2DCoordI(const C3DVector& v) : X((long)v.X), Y((long)v.Y) { }
};
float __fastcall Distance(const C2DCoordI& a, const C2DCoordI& b);   // 0x009E2430
#include <math.h>

struct CTCGraphicAppearance {
    void GetAnimationNames(CStringVector* out, const CCharString& prefix);   // 0x004BDD70
    bool GetAnimationEnd(const CCharString* name, long variant, unsigned long flags, C3DVector* position,
                         CRightHandedSet* orientation, float speed);         // 0x004C08C0
};
struct CTCPhysicsBase {
    void GetApproachPoint(C3DVector* point, float* radius);  // 0x006AF640
};
class CThing {
public:
    unsigned char         _pad00[0x60];
    CTCPhysicsBase*       PPhysics;                          // +0x60
    CTCGraphicAppearance* PGraphicAppearance;                // +0x64
    const C3DVector* GetPosition() const;                    // 0x004C73D0
    static float GetCollisionRadius();                       // 0x00661CC0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CCreatureActionBase {
    virtual void v00();
    virtual void Initialise();                               // 0x00694D10
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                       // +0x08
    long                    _10;                             // +0x10
    long                    _14;                             // +0x14
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_UseObject : CCreatureActionBase {
    unsigned char           _pad18[0xa8 - 0x18];
    CBaseIntelligentPointer PObject;                         // +0xa8
    long                    _b0;                             // +0xb0
    CRightHandedSet         UseOrientation;                  // +0xb4
    C3DVector               UsePosition;                     // +0xcc
    float                   ApproachRadius;                  // +0xd8
    bool                    Started;                         // +0xdc
    bool                    NeedsToMove;                     // +0xdd
    bool                    InPosition;                      // +0xde
    bool                    StartInPosition;                 // +0xdf
    virtual void Initialise();
};

void CCreatureAction_UseObject::Initialise()
{
    CCreatureActionBase::Initialise();
    _10 = -9;
    _14 = -9;
    CStringVector animations;
    CTCGraphicAppearance* appearance = PObject.GetPItem()->PGraphicAppearance;
    if (appearance) {
        appearance->GetAnimationNames(&animations, g_UseAnimationPrefix);
        SortStrings(animations._M_start, animations._M_finish);
        long variant = 0;
        if (!animations.empty()) {
            appearance->GetAnimationEnd(animations._M_start, 0, 0, &UsePosition, &UseOrientation, 1.0f);
            for (unsigned long i = 1; i < animations.size(); ++i) {
                const CCharString& previous = animations._M_start[i - 1];
                CCharString& name = animations._M_start[i];
                if (name == previous)
                    ++variant;
                else
                    variant = 0;
                C3DVector position;
                CRightHandedSet orientation;
                appearance->GetAnimationEnd(&name, variant, 0, &position, &orientation, 1.0f);
                const C3DVector* creature = GetCreature()->GetPosition();
                C2DVector offset(position.X - creature->X, position.Y - creature->Y);
                const C3DVector* here = GetCreature()->GetPosition();
                if (offset.LengthSquared() < C2DVector(UsePosition.X - here->X, UsePosition.Y - here->Y).LengthSquared()) {
                    UsePosition = position;
                    UseOrientation = orientation;
                }
            }
        } else {
            PObject.GetPItem()->PPhysics->GetApproachPoint(&UsePosition, &ApproachRadius);
            ApproachRadius += GetCreature()->GetCollisionRadius() * 2.0f;
        }
    } else {
        PObject.GetPItem()->PPhysics->GetApproachPoint(&UsePosition, &ApproachRadius);
        ApproachRadius += GetCreature()->GetCollisionRadius() * 2.0f;
    }
    Started = true;
    CThing* creature = GetCreature();
    const C3DVector* position = GetCreature()->GetPosition();
    float distance = Distance(C2DCoordI(UsePosition), C2DCoordI((long)position->X, (long)position->Y));
    if (distance < creature->GetCollisionRadius() * 2.0f) {
        float height = UsePosition.Z;
        if (fabs(height - GetCreature()->GetPosition()->Z) < 2.5f) {
            NeedsToMove = false;
            if (ApproachRadius == 0.0f)
                InPosition = StartInPosition;
        } else {
            NeedsToMove = true;
            InPosition = false;
        }
    } else {
        NeedsToMove = true;
        InPosition = false;
    }
    NeedsToMove = false;
    InPosition = false;
    _b0 = 0;
}
