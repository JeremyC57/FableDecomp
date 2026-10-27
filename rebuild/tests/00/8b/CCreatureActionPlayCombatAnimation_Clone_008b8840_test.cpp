// behaviour test for 0x008B8840 CCreatureAction_PlayCombatAnimation::Clone() const (model; the base copy constructor is a stub)
#include <cstdio>
#include <cstring>
class CThing;
template <class T> class CIntelligentPointer;
static int g_copies = 0;
class CCreatureActionBase { public: virtual ~CCreatureActionBase() { } virtual CCreatureActionBase* Clone() const = 0; };
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase() { }
    CCombatActionBase(const CCombatActionBase& other) { ++g_copies; std::memcpy(_body, other._body, sizeof(_body)); }
    unsigned char _body[16];
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase {
public:
    CCreatureAction_PlayCombatAnimation() { }
    virtual CCreatureActionBase* Clone() const;
    bool FaceTarget;                                     // +0x114 (PDB +300)
    bool UsePhysics;                                     // +0x115
    bool AlwaysUseMovementInAnim;                        // +0x116
};
CCreatureActionBase* CCreatureAction_PlayCombatAnimation::Clone() const { return new CCreatureAction_PlayCombatAnimation(*this); }
int main()
{
    CCreatureAction_PlayCombatAnimation a; std::memset(a._body, 0x5A, sizeof(a._body)); a.FaceTarget = 1;
    CCreatureAction_PlayCombatAnimation* b = (CCreatureAction_PlayCombatAnimation*)a.Clone();
    int ok = b != 0 && b != &a && g_copies == 1 && b->FaceTarget == a.FaceTarget && b->_body[3] == 0x5A;
    delete b;
    if (!ok) { std::printf("008b8840_TEST FAIL\n"); return 1; }
    std::printf("008b8840_TEST PASS\n");
    return 0;
}