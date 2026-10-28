// CTCLook::InitialiseTurningViaAnimation(const C3DVector& pos, long) -- start turning the body toward a position: when the target lies (almost) straight ahead use old-style turning at full rate, otherwise pick the best-matching turning animation, scale how much of its rotation to accept (clamped to 1 unless the def allows over/understeer) and play it as the main animation
// Grade: DIFFER(532v528) -- x87 order only: the length test squares Y (on the stack) before X and, with the operands written `EPSILON * EPSILON < length`, tests `ah,41h/jne` (the `length > EPSILON * EPSILON` form of g2 gives retail's `ah,5/jp`); the dot product's Z term loads toPos.Z before facing.Z in every spelling tried. Everything else (CAnimInit temporary, GetTC, for_each, oversteer test) matches. Next try (GOTCHAS wave 24m / chunk 205): the left-square-first statement form `float s = toPos.X * toPos.X; s += toPos.Y * toPos.Y;` for the length and `length > EPSILON * EPSILON`
// Leaf: ?InitialiseTurningViaAnimation@CTCLook@@AAEXABVC3DVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753630, length 528 bytes, thiscall `ret 8` (the long is never read).
// Name: Ego_r.pdb `?InitialiseTurningViaAnimation@CTCLook@@AAEXABVC3DVector@@J@Z` (private, void, const C3DVector&, long);
//   its body is the only user of CGetBestTurningAnim (Ego_r ctor ??0CGetBestTurningAnim@?A0x247890bc@@QAE@ABVC3DVector@@0@Z
//   = retail 0x00751610, landed) and of the _For_each<pair<float,const CAnimationEntry*>*, CGetBestTurningAnim> instance
//   (retail 0x00754D80, called only from here); the stores are the PDB CTCLook members UsingOldStyleTurning +0x40,
//   FractionOfTurningToAcceptEachFrame +0x44, PCurrentTurningAnim +0x9c (PDB +0xa0), reading TurningAnims +0x8c and
//   TurningViaAnimMinAngle +0x98, and the CLookDef AllowOversteer +0x4d / AllowUndersteer +0x4e. Only caller 0x00754010.
// Callees: the physics component's virtual slot 70 (+0x118, GetFacingVector() const), C3DVector::AccurateNormalise
//   0x00A14440, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetTC on interface 0x5A, unchecked: the
//   miss path reads the dead slot), CTCAnimationComplex::GetAnimation 0x0070B460, CAnimationEntry::GetAnimateThingFlags
//   0x005DC390, CAnimInit::Init 0x0070C050, CAnimationEntry::GetAnimationTransitionInTime 0x005DC1F0 and
//   GetTimeToBlendIn 0x005DC230, CAnimateThing::SetMainAnim 0x0070D580 (Ego_r spellings).
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44, PhysicsTC +0x60; CTCPhysicsBase::Position +0x0c;
//   CAnimInit (44 bytes, Ego_r) whose CAnimFrameInfo Frame (+0x18) is zeroed by its inline constructor.
extern "C" double __cdecl cos(double x);
#pragma intrinsic(cos)

static const float TWO_PI = 6.2831855f;
extern const float EPSILON;                             // 0x0129BA3C (0.0001f)

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector& AccurateNormalise();                     // 0x00A14440
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x00754D80 (this instance)
}

class CThing;
class CAnimationEntry { public:
    long BankIndex;                                     // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
    long GetAnimationTransitionInTime() const;          // 0x005DC1F0
    long GetTimeToBlendIn(const CThing& thing) const;   // 0x005DC230
};

namespace {
class CGetBestTurningAnim { public:
    float            RotationAngle;                     // +0x00
    float            BestMatchAngle;                    // +0x04
    CAnimationEntry* PBestMatchAnim;                    // +0x08
    CGetBestTurningAnim(const C3DVector& facing, const C3DVector& target);   // 0x00751610
};
}

enum EAnimUpdateMode { EAnimUpdateMode_Force32 = 0x7fffffff };

class CAnimFrameInfo { public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) { }
};

class CAnimInit { public:
    EAnimUpdateMode UpdateMode;                         // +0x00
    unsigned char   Type;                               // +0x04
    bool            Looping;                            // +0x05
    long            Anim;                               // +0x08
    float           Speed;                              // +0x0c
    float           Alpha;                              // +0x10
    float           LastAlpha;                          // +0x14
    CAnimFrameInfo  Frame;                              // +0x18
    unsigned long   Flags;                              // +0x20
    unsigned long   TypeFlags;                          // +0x24
    long            Priority;                           // +0x28
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode mode, bool looping,
              unsigned long flags, long priority)
    {
        Init(type, typeFlags, anim, mode, looping, flags, priority);
    }
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode mode, bool looping,
              unsigned long flags, long priority);      // 0x0070C050
};

class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init, long timeToBlendIn, long transitionInTime);   // 0x0070D580
};

class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                      // 0x0070B460
};

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector() const;          // slot 70 (+0x118)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                             // +0x0c
};

class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
template <class K, class V> class CKeyPairCompareLess { public:
};
template <class K, class V, class TCompare> class CVectorMap { public:   // retail 0x10 bytes
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    value_type* _M_start;                               // +0x00
    value_type* _M_finish;                              // +0x04
    value_type* _M_end_of_storage;                      // +0x08
    TCompare    Compare;                                // +0x0c
    bool        Dirty;                                  // +0x0d
    iterator end() { return _M_finish; }
    iterator LowerBound(const K& key);                  // 0x0040F020 for the TC map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TComponentMap;

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  UsingComponents[9];                  // +0x20 bitset<274>
    TComponentMap  Components;                          // +0x44
    unsigned char  _pad54[0x60 - 0x54];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    template <class T> __forceinline bool GetTC(T** out, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            TComponentMap* map = &Components;
            TComponentMap::iterator it = map->LowerBound(type);
            *out = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};

class CLookDef { public:
    unsigned char _pad00[0x4d];
    bool AllowOversteer;                                // +0x4d
    bool AllowUndersteer;                               // +0x4e
};

template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

typedef std::pair<float, const CAnimationEntry*> TTurningAnim;

class CTCLook { public:
    void*            __vftable;                         // +0x00
    CThing*          ParentThing;                       // +0x04
    unsigned char    _pad08[0x14 - 0x08];
    CDefPointer<const CLookDef> PDef;                   // +0x14
    unsigned char    _pad18[0x40 - 0x18];
    bool             UsingOldStyleTurning;              // +0x40
    bool             TurningViaAnimationDisable;        // +0x41
    float            FractionOfTurningToAcceptEachFrame;   // +0x44
    unsigned char    _pad48[0x8c - 0x48];
    TTurningAnim*    TurningAnimsStart;                 // +0x8c vector<pair<float, const CAnimationEntry*> >
    TTurningAnim*    TurningAnimsFinish;                // +0x90
    TTurningAnim*    TurningAnimsEnd;                   // +0x94
    float            TurningViaAnimMinAngle;            // +0x98
    CAnimationEntry* PCurrentTurningAnim;               // +0x9c
private:
    void InitialiseTurningViaAnimation(const C3DVector& pos, long);
};

void CTCLook::InitialiseTurningViaAnimation(const C3DVector& pos, long)
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector facing = physics->GetFacingVector();
    C3DVector toPos(pos.X - physics->Position.X, pos.Y - physics->Position.Y, 0.0f);
    facing.Z = 0.0f;
    if (EPSILON * EPSILON < toPos.GetLengthSquared()) {
        toPos.AccurateNormalise();
        facing.AccurateNormalise();
        float fraction = 1.0f;
        bool usingOldStyleTurning = false;
        float dot = facing.Y * toPos.Y;
        dot += facing.X * toPos.X;
        dot += toPos.Z * facing.Z;
        if (dot > cos(TurningViaAnimMinAngle * 0.5f * (double)TWO_PI)) {
            usingOldStyleTurning = true;
        } else {
            TTurningAnim* last = TurningAnimsFinish;
            TTurningAnim* first = TurningAnimsStart;
            CGetBestTurningAnim best = std::for_each(first, last, CGetBestTurningAnim(facing, toPos));
            fraction = best.RotationAngle / best.BestMatchAngle;
            CAnimationEntry* anim = best.PBestMatchAnim;
            bool allowed = true;
            if (fraction > 1.0f)
                allowed = PDef->AllowOversteer;
            else if (fraction < 1.0f)
                allowed = PDef->AllowUndersteer;
            if (!allowed)
                fraction = 1.0f;
            CTCAnimationComplex* animationComplex;
            ParentThing->GetTC(&animationComplex, 0x5a);
            CAnimateThing& animation = animationComplex->GetAnimation();
            CThing* parent = ParentThing;
            animation.SetMainAnim(CAnimInit(7, 0, anim->BankIndex, (EAnimUpdateMode)1, false, anim->GetAnimateThingFlags(), 100),
                                  anim->GetTimeToBlendIn(*parent), anim->GetAnimationTransitionInTime());
            PCurrentTurningAnim = anim;
        }
        FractionOfTurningToAcceptEachFrame = fraction;
        UsingOldStyleTurning = usingOldStyleTurning;
    }
}
