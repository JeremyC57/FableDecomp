// CTCCreatureGroup::GetNearestMember(const C3DVector& pos) -- walk the member map and return the member thing nearest to the position (the running best is compared as distance-squared against the SQUARE of the stored distance-squared, as in retail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008128F0, 275 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x006A4D00 (the out-of-line const
//   accessor).
// Layout = Ego_r.pdb CTCCreatureGroup (as the landed IsActiveMember 0x00812F70): Members (STLport map<unsigned __int64,
//   CGroupMemberData>, header pointer first) +0x18; node: parent +4, left +8, right +0xc, key +0x10, CGroupMemberData
//   {PMember CIntelligentPointer<CThing>, IsActive} +0x18.
// Shape: a for loop over the map with the inline static `_Rb_global<bool>::_M_increment(node)` (a by-value node copy
//   in eax; incrementing the member in place gives other registers) and `it != Members.end()` re-read from `this` (spilled to
//   the frame); the member thing is fetched at the top of every iteration; the first member's distance-squared fills the
//   best (initialised to 0 through the zero register), later ones are compared as `d < best * best` (fst into the dead `pos` slot, fld/fmul/fxch/
//   fcompp); the sum of squares is the statement-order Z, Y, X form.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};
struct CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CBaseIntelligentPointer {
    void* __vftable;                            // +0x00
    void* PData;                                // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CGroupMemberData {
    CBaseIntelligentPointer PMember;            // +0x00
    bool IsActive;                              // +0x08
    unsigned long JoinedFrame;                  // +0x0c
    unsigned long DeactivatedFrame;             // +0x10
};
struct SGroupMemberNode {                       // STLport _Rb_tree_node<pair<const unsigned __int64, CGroupMemberData> >
    long Color;                                 // +0x00
    SGroupMemberNode* Parent;                   // +0x04
    SGroupMemberNode* Left;                     // +0x08
    SGroupMemberNode* Right;                    // +0x0c
    unsigned __int64 Key;                       // +0x10
    CGroupMemberData Value;                     // +0x18
};
struct CGroupMemberIterator {
    SGroupMemberNode* _M_node;
    CGroupMemberIterator(SGroupMemberNode* node) : _M_node(node) { }
    SGroupMemberNode* operator->() const { return _M_node; }
    bool operator!=(SGroupMemberNode* node) const { return _M_node != node; }
    static SGroupMemberNode* _M_increment(SGroupMemberNode* x)   // STLport _Rb_global<bool>::_M_increment, inlined
    {
        if (x->Right != 0) {
            x = x->Right;
            while (x->Left != 0)
                x = x->Left;
        } else {
            SGroupMemberNode* y = x->Parent;
            while (x == y->Right) {
                x = y;
                y = y->Parent;
            }
            if (x->Right != y)
                x = y;
        }
        return x;
    }
    CGroupMemberIterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
};
struct CGroupMemberMap {                        // STLport map<unsigned __int64, CGroupMemberData>
    SGroupMemberNode* Header;                   // +0x00
    unsigned long NodeCount;                    // +0x04
    unsigned long _compare;                     // +0x08
    SGroupMemberNode* begin() const { return Header->Left; }
    SGroupMemberNode* end() const { return Header; }
};
struct CTCCreatureGroup {
    void* __vftable;                            // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    unsigned long MemberCount;                  // +0x0c
    unsigned long ActiveMemberCount;            // +0x10
    bool ShutdownOnEmpty;                       // +0x14
    CGroupMemberMap Members;                    // +0x18
    CThing* GetNearestMember(const C3DVector& pos);
};

CThing* CTCCreatureGroup::GetNearestMember(const C3DVector& pos)
{
    CThing* nearest = 0;
    float nearestDistSq = 0;
    for (CGroupMemberIterator it = Members.begin(); it != Members.end(); ++it) {
        CThing* thing = it->Value.PMember.GetPItem();
        if (nearest) {
            float distSq = (thing->GetPosition() - pos).GetLengthSquared();
            if (distSq < nearestDistSq * nearestDistSq) {
                nearest = it->Value.PMember.GetPItem();
                nearestDistSq = distSq;
            }
        } else {
            nearest = thing;
            nearestDistSq = (it->Value.PMember.GetPItem()->GetPosition() - pos).GetLengthSquared();
        }
    }
    return nearest;
}
