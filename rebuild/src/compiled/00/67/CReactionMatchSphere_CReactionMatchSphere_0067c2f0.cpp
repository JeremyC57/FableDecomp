// CReactionMatch_Sphere::CReactionMatch_Sphere(EOpinionReactionType type, const COpinionSet& centre, float radius) -- store the reaction type, copy the centre opinion set and keep the squared radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067C2F0, length 46 bytes (`ret 0Ch`).
// Vtable: CReactionMatch_Sphere 0x0125B7C4. Callees: the COpinionSet copy constructor @ 0x00477428 (its manifest
// label "lua_stackspace" is a propagated name; the call copies the CArray<float> OpinionValues).
// Layout: PDB names with the retail offsets (CReactionMatch::ReactionType +0x04, COpinionSet Centre +0x08, SqRadius
// +0x14): retail CArray<T> is three pointers, so SqRadius sits 4 bytes below the PDB's +0x18 and
// engine/CReactionMatch_Sphere.h cannot be used.
// Shape: the inline CReactionMatch base constructor's store and the derived vtable store precede the member
// copy-constructor call (a member object with an out-of-line constructor); `radius * radius` is computed after it
// straight from the argument slot (`fld [esp+10h] ; fmul [esp+10h]`).
// Leaf: ??0CReactionMatch_Sphere
enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0 };

struct COpinionSet {                                    // CArray<float> OpinionValues (retail: begin/end/capacity)
    float* m_begin; float* m_end; float* m_capacity;
    COpinionSet(const COpinionSet& other);              // 0x00477428
};

struct CReactionMatch {
    EOpinionReactionType ReactionType;                  // +0x04
    CReactionMatch(EOpinionReactionType type) : ReactionType(type) {}
    virtual long GetMatchType() const;
};

struct CReactionMatch_Sphere : CReactionMatch {
    COpinionSet Centre;                                 // +0x08
    float       SqRadius;                               // +0x14
    CReactionMatch_Sphere(EOpinionReactionType type, const COpinionSet& centre, float radius);
    virtual long GetMatchType() const;
};

CReactionMatch_Sphere::CReactionMatch_Sphere(EOpinionReactionType type, const COpinionSet& centre, float radius)
    : CReactionMatch(type), Centre(centre), SqRadius(radius * radius)
{
}