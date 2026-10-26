// CTCCreatureExpression::Update(CAnimateThing& animate, const CAnimateThingUpdate& update) -- run blinking, advance every expression state's weights by the frame time, drop the non-primary states whose combination anim is gone or whose weights have both reached zero (removing that anim), then (outside movie sequences) re-add every remaining state as a type-0x12 combination anim carrying its current and previous weight
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729200, 352 bytes (`ret 8`).
// Callees: CTCCreatureExpression::UpdateBlinking @ 0x00728FF0, CExpressionState::Update(float) @ 0x00729110,
// CAnimateThing::GetCombinationAnimFromTypeAndIndex(long, long) @ 0x0070C170, RemoveCombinationAnim @ 0x0070D8E0,
// RemoveAllAnimsWithTypeFlags(unsigned long) @ 0x0070D900, CAnimInit::Set @ 0x0070C050, AddCombinationAnim @ 0x0070D780,
// CTCBase::GetWorld @ 0x00686A70, CGameScriptInterface::IsInMovieSequence (slot 395, +0x62c; CWorld +0x38), free @ 0x00BFEA14
// (the inlined list erase). Globals: 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCCreatureExpression with the retail 4-byte STLport list ExpressionStates (+0x0c); node {next, prev,
// CExpressionState (+8): AnimIndex, MaxWeight, Weight +8, OldWeight +0xc, Primary +0x10, Stage}; CAnimInit (0x2c) as the
// landed CTCBed::UpdateAnimation 0x007468C0 (Alpha +0x10, LastAlpha +0x14, Frame +0x18 zeroed by its ctor);
// CAnimateThingUpdate's first word is the frame time.
// Shape: the erase is STLport's inline list erase (unlink, free); the removal test is a named bool (the direct `if` inverts the
// last float branch); the (1, 1) frame info is a temporary assigned in the loop (VC7.1 hoists the two stores into frame slots;
// a named local or member-wise copy caches the wrong half in ebx); the movie test goes through an inline CWorld wrapper.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CAnimFrameInfo { public:                                 // CAnimInit +0x18, zeroed by its ctor
    float m_first;
    float m_second;
    CAnimFrameInfo() : m_first(0.0f), m_second(0.0f) { }
    CAnimFrameInfo(float first, float second) : m_first(first), m_second(second) { }
};
class CAnimInit { public:                                      // 0x2c bytes
    long           UpdateMode;                          // +0x00
    unsigned char  Type;                                // +0x04
    bool           Looping;                             // +0x05
    long           Anim;                                // +0x08
    float          Speed;                               // +0x0c
    float          Alpha;                               // +0x10
    float          LastAlpha;                           // +0x14
    CAnimFrameInfo Frame;                               // +0x18
    unsigned long  Flags;                               // +0x20
    unsigned long  TypeFlags;                           // +0x24
    long           Priority;                            // +0x28
    void Set(long animType, long typeFlags, long anim, long mode, bool looping, unsigned long flags, long priority);   // 0x0070C050
    CAnimInit(long animType, long typeFlags, long anim, long mode, bool looping, unsigned long flags, long priority)
    {
        Set(animType, typeFlags, anim, mode, looping, flags, priority);
    }
};
class CAnimSet;
class CAnimateThing { public:
    CAnimSet* GetCombinationAnimFromTypeAndIndex(long type, long index);   // 0x0070C170
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
    void RemoveCombinationAnim(const CAnimSet* anim);   // 0x0070D8E0
    void RemoveAllAnimsWithTypeFlags(unsigned long flags);   // 0x0070D900
};
class CAnimateThingUpdate { public:
    float FrameTime;                                    // +0x00
};
struct CExpressionState {
    long          AnimIndex;                            // +0x00
    float         MaxWeight;                            // +0x04
    float         Weight;                               // +0x08
    float         OldWeight;                            // +0x0c
    bool          Primary;                              // +0x10
    unsigned char Stage;                                // +0x11
    void Update(float dt);                              // 0x00729110
};
struct SExpressionNode {
    SExpressionNode* _M_next;
    SExpressionNode* _M_prev;
    CExpressionState _M_data;                           // +0x08
};
struct CExpressionStateList {                           // STLport list: the header node pointer
    SExpressionNode* _M_node;
    SExpressionNode* begin() const { return _M_node->_M_next; }
    SExpressionNode* end() const { return _M_node; }
    bool empty() const { return _M_node->_M_next == _M_node; }
    SExpressionNode* erase(SExpressionNode* pos)
    {
        SExpressionNode* next = pos->_M_next;
        SExpressionNode* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(pos);
        return next;
    }
};
class CGameScriptInterface { public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(00) VF10(01) VF10(02) VF10(03) VF10(04) VF10(05) VF10(06) VF10(07) VF10(08) VF10(09)
    VF10(10) VF10(11) VF10(12) VF10(13) VF10(14) VF10(15) VF10(16) VF10(17) VF10(18) VF10(19)
    VF10(20) VF10(21) VF10(22) VF10(23) VF10(24) VF10(25) VF10(26) VF10(27) VF10(28) VF10(29)
    VF10(30) VF10(31) VF10(32) VF10(33) VF10(34) VF10(35) VF10(36) VF10(37) VF10(38)
    VF(390) VF(391) VF(392) VF(393) VF(394)
#undef VF10
#undef VF
    virtual bool IsInMovieSequence() const;             // slot 395 (+0x62c)
};
class CWorld { public:
    unsigned char _pad00[0x38];
    CGameScriptInterface* PGameScriptInterface;         // +0x38
    bool IsInMovieSequence() { return PGameScriptInterface->IsInMovieSequence(); }
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCCreatureExpression : public CTCBase { public:
    void* ParentThing;                                  // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CExpressionStateList ExpressionStates;              // +0x0c
    bool Blinking;                                      // +0x10
    void UpdateBlinking(CAnimateThing& animate, const CAnimateThingUpdate& update);   // 0x00728FF0
    void Update(CAnimateThing& animate, const CAnimateThingUpdate& update);
};

void CTCCreatureExpression::Update(CAnimateThing& animate, const CAnimateThingUpdate& update)
{
    UpdateBlinking(animate, update);
    SExpressionNode* it;
    for (it = ExpressionStates.begin(); it != ExpressionStates.end(); it = it->_M_next)
        it->_M_data.Update(update.FrameTime);
    for (it = ExpressionStates.begin(); it != ExpressionStates.end(); ) {
        CAnimSet* anim = animate.GetCombinationAnimFromTypeAndIndex(0x12, it->_M_data.AnimIndex);
        bool remove = !it->_M_data.Primary && (!anim || (it->_M_data.Weight == 0.0f && it->_M_data.OldWeight == 0.0f));
        if (!remove)
            it = it->_M_next;
        else {
            animate.RemoveCombinationAnim(anim);
            it = ExpressionStates.erase(it);
        }
    }
    if (!ExpressionStates.empty()) {
        animate.RemoveAllAnimsWithTypeFlags(1);
        if (!GetWorld().IsInMovieSequence()) {
            for (it = ExpressionStates.begin(); it != ExpressionStates.end(); it = it->_M_next) {
                CAnimInit init(0x12, 1, it->_M_data.AnimIndex, 0, false, 0, 100);
                init.Frame = CAnimFrameInfo(1.0f, 1.0f);
                init.Alpha = it->_M_data.Weight;
                init.LastAlpha = it->_M_data.OldWeight;
                animate.AddCombinationAnim(init, 0, 0, false, false);
            }
        }
    }
}