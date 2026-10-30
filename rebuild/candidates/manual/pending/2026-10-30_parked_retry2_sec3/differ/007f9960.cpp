// Grade: DIFFER(559v551) -- 112 difflines (parked 626v551/212); a __forceinline memberwise C3DVector copy ctor makes CTargetSorter non-trivially copyable, so it is built in place in the outgoing argument block as retail (no frame temp + rep movsd), frame 0x88 -> 0x54 (retail 0x5c); the first facing as a named (unused) local. Left: frame slot assignment (retail pos copy +0x28, first facing +0x40, box +0x5c; ours 4 bytes smaller), box arithmetic register order. Tried: search half in a block scope (630v551).
// Tried: named filter locals (0xb0 frame), filter temporaries in the FindThings call (0x88 frame).
// CTCTargetingSpirit::UpdateTargetList() -- gather the selectable things in a box around the spirit (not the players' characters), sort them by the facing-weighted target score and target the best
// Leaf: UpdateTargetList
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9960, 551 bytes, `ret`.
// Callees: CThing::GetPosition @ 0x004C73D0, the graphic's facing getter (vslot +0x118, C3DVector by hidden pointer), ftol
//   @ 0x00BFEA70, CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970 / GetPlayer @ 0x004498C0,
//   the player character getter @ 0x00487DC0, the vector's reallocating insert @ 0x0049C370 (same-TU stand-in body),
//   the CTCBase thing-search-tools getter @ 0x00686D80 (catalog `_C`), the boxed thing search @ 0x007F9C00, the target sort
//   @ 0x007FAE60 (fastcall, comparator by value), the targeting SetPTargetedThing vslot +0x88, free @ 0x00BFEA14.
// Float pool: 1.0f @ 0x0122DED8.
// Layout: Ego_r.pdb CTCTargetingSpirit MaxTargetingDistance +0x38; CThing graphic +0x60.
#include <new>
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct SScalarTag { SScalarTag() { } };
template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) {}
    ~CRetailVector() { if (first) free(first); }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x0049C370
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_7f9960(void* vector, unsigned long count);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void CRetailVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_7f9960(this, count);
    if (p)
        *p = value;
}
#pragma auto_inline(on)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct C3DSphere {
    C3DVector Centre;
    float     Radius;
    C3DSphere(const C3DVector& c, float r) : Centre(c), Radius(r) {}
};

struct CThing;
struct CThingGraphic {
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
    virtual C3DVector GetFacing() const;                // +0x118
};
struct CThing {
    unsigned char  _pad00[0x60];
    CThingGraphic* Graphic;                             // +0x60
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CPlayer { CThing* GetPCharacterThing(); };       // 0x00487DC0
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
    CPlayer* GetPlayer(long index);                     // 0x004498C0
};

struct CThingFilter_NotInList {
    CRetailVector<CThing*>* Excluded;
    long                    Unused;
    CThingFilter_NotInList(CRetailVector<CThing*>* excluded) : Excluded(excluded), Unused(0) {}
};
struct CThingFilter_IsSelectable {
    bool A, B;
    CThingFilter_IsSelectable(bool a, bool b) : A(a), B(b) {}
};
struct CThingFilter_Bind3 {
    const CThingFilter_NotInList*    F0;
    const CThingFilter_IsSelectable* F1;
    const C3DSphere*                 F2;
    CThingFilter_Bind3(const CThingFilter_NotInList& f0, const CThingFilter_IsSelectable& f1, const C3DSphere& f2)
        : F0(&f0), F1(&f1), F2(&f2) {}
};
struct CThingSearchTools {
    void FindThings(const C2DBoxI& box, const CThingFilter_Bind3& filter, CRetailVector<CThing*>* out);   // 0x007F9C00
};
struct CTargetSorter {                                  // 0x34 bytes, passed by value
    CThing*   Origin;
    C3DVector Position;
    C3DVector Facing;
    float     DistanceWeight;
    float     AngleWeight;
    long      Mode;
    float     Scale;
    bool      Flag;
    float     Bias;
    CTargetSorter(CThing* origin, const C3DVector& pos, const C3DVector& facing)
        : Origin(origin), Position(pos), Facing(facing), DistanceWeight(2.0f), AngleWeight(0.005f), Mode(0), Scale(1.0f),
          Flag(false), Bias(1.0f) {}
};
void __fastcall SortTargets(CThing** first, CThing** last, CTargetSorter sorter);   // 0x007FAE60

struct CTCTargetingSpirit {
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x38 - 0x08];
    float   MaxTargetingDistance;                       // +0x38
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    CThingSearchTools* GetThingSearchTools();           // 0x00686D80
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void SetPTargetedThing(CThing* thing);      // +0x88
    void UpdateTargetList();
};

void CTCTargetingSpirit::UpdateTargetList()
{
    C3DVector pos = ParentThing->GetPosition();
    C3DVector facing = ParentThing->Graphic->GetFacing();
    long radius = (long)MaxTargetingDistance;
    long y = (long)pos.Y;
    long x = (long)pos.X;
    long half = (long)(MaxTargetingDistance + 1.0f);
    C2DBoxI box(x - half, y - half, x + half, y + half);
    GetPlayerManager()->GetMainPlayer()->GetPCharacterThing();
    CRetailVector<CThing*> targets;
    CRetailVector<CThing*> players;
    for (long i = 0; i < 4; ++i) {
        CThing* hero = GetPlayerManager()->GetPlayer(i)->GetPCharacterThing();
        if (hero)
            players.push_back(hero);
    }
    GetThingSearchTools()->FindThings(box,
                                      CThingFilter_Bind3(CThingFilter_NotInList(&players), CThingFilter_IsSelectable(false, false),
                                                         C3DSphere(pos, (float)radius)),
                                      &targets);
    CThing* parent = ParentThing;
    SortTargets(targets.first, targets.last, CTargetSorter(parent, parent->GetPosition(), parent->Graphic->GetFacing()));
    SetPTargetedThing(targets.first != targets.last ? *targets.first : 0);
}
