// Behaviour test for CTCWillLightning::InitialiseBeams (0x007A16F0): the main GetTargets call gets the caster, the main
// beam limits and "beams already exist"; on success every beam is flagged main, takes its hand from WeaponState, gets a
// sub-beam GetTargets from its target with the sub-beam limits and is activated; the beam references stay balanced, the
// beams are flagged initialised and a type-2 caster with interface 8 picks a new lock target. On failure nothing else
// happens. The local target list is freed either way.
#include <cstdio>
#include <cstdlib>

class CThing {
public:
    unsigned char m_type;
    unsigned long m_presentBits[9];
    void* m_tc8;
    bool HasTC(long t) const { return (m_presentBits[t >> 5] & (1UL << (t & 31))) != 0; }
    template<class T> T* GetTC(long) { return (T*)m_tc8; }
};
static int g_pickCalls = 0;
class CTCTargetingPlayer { public: void SetAsPickUpNewLockTarget() { ++g_pickCalls; } };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };

class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer() { if (Info) --Info->Count; }
    T* operator->() const { return Data; }
};
template <class T> struct CRetailList { struct Node { Node* Next; Node* Prev; T Value; }; Node* Head; };

static int g_listsAlive = 0;
struct CThingPtrList {
    struct Node { Node* Next; Node* Prev; CThing* Value; };
    Node* Head;
    CThingPtrList() { Node* n = (Node*)malloc(sizeof(Node)); n->Next = n; n->Prev = n; Head = n; ++g_listsAlive; }
    ~CThingPtrList()
    {
        Node* cur = Head->Next;
        while (cur != Head) { Node* tmp = cur; cur = cur->Next; free(tmp); }
        Head->Next = Head; Head->Prev = Head;
        free(Head);
        --g_listsAlive;
    }
};

struct CLightningBeam;
typedef CRetailList<CCountedPointer<CLightningBeam> > CBeamList;
static int g_activated = 0;
struct CLightningBeam {
    CBaseIntelligentPointer Target;
    CBeamList Children;
    bool MainBeam;
    bool RightHand;
    void Activate() { ++g_activated; }
};

struct SCall { void* beams; long maxBeams; CThing* origin; void* targets; float damage, decap, delay; bool flag; };
static SCall g_calls[8];
static int g_nCalls = 0;
static bool g_mainResult = true;

class CTCWillLightning { public:
    CThing* ParentThing;
    long  WeaponState;
    bool  LeftHandAnim;
    CBeamList m_lbBeams;
    bool  BeamsInitialised;
    float m_fDamagePerHit, m_fSubBeamDamagePerHit;
    long  m_nMaxMainBeams, m_nMaxSubBeams;
    float m_fMainBeamDecapitationTime, m_fSubBeamDecapitationTime, m_fSubBeamDelay;
    bool GetTargets(CBeamList* beams, long maxBeams, CThing* origin, CThingPtrList* targets,
                    float damagePerHit, float decapitationTime, float delay, bool subBeams)
    {
        SCall c = { beams, maxBeams, origin, targets, damagePerHit, decapitationTime, delay, subBeams };
        g_calls[g_nCalls++] = c;
        return g_nCalls == 1 ? g_mainResult : true;
    }
    bool IsRightHanded() const
    {
        switch (WeaponState) {
        case 0: return !LeftHandAnim;
        case 1: return false;
        case 2: return true;
        }
        return true;
    }
    void InitialiseBeams();
};

void CTCWillLightning::InitialiseBeams()
{
    CThingPtrList targets;
    CThing* parent = ParentThing;
    if (GetTargets(&m_lbBeams, m_nMaxMainBeams, parent, &targets, m_fDamagePerHit,
                   m_fMainBeamDecapitationTime, 0.0f, m_lbBeams.Head->Next != m_lbBeams.Head)) {
        for (CBeamList::Node* it = m_lbBeams.Head->Next; it != m_lbBeams.Head; it = it->Next) {
            CCountedPointer<CLightningBeam> beam = it->Value;
            beam->MainBeam = true;
            beam->RightHand = IsRightHanded();
            CThing* target = beam->Target.GetPItem();
            GetTargets(&beam->Children, m_nMaxSubBeams, target, &targets, m_fSubBeamDamagePerHit,
                       m_fSubBeamDecapitationTime, m_fSubBeamDelay, false);
            beam->Activate();
        }
        BeamsInitialised = true;
        CThing* caster = ParentThing;
        if (caster->m_type == 2 && caster->HasTC(8))
            caster->GetTC<CTCTargetingPlayer>(8)->SetAsPickUpNewLockTarget();
    }
}

int main()
{
    CThing caster, t1, t2;
    caster.m_type = 2;
    for (int i = 0; i < 9; ++i) caster.m_presentBits[i] = 0;
    caster.m_presentBits[0] = 1UL << 8;
    CTCTargetingPlayer targeting;
    caster.m_tc8 = &targeting;
    CLightningBeam b1, b2;
    b1.Target.m_pItem = &t1; b2.Target.m_pItem = &t2;
    b1.MainBeam = b2.MainBeam = false;
    CCPPointerInfo i1 = { 1 }, i2 = { 1 };
    CBeamList::Node head, n1, n2;
    n1.Value.Data = &b1; n1.Value.Info = &i1; n2.Value.Data = &b2; n2.Value.Info = &i2;
    head.Next = &n1; n1.Next = &n2; n2.Next = &head;
    CTCWillLightning w;
    w.ParentThing = &caster; w.WeaponState = 0; w.LeftHandAnim = true;
    w.m_lbBeams.Head = &head; w.BeamsInitialised = false;
    w.m_fDamagePerHit = 5.0f; w.m_fSubBeamDamagePerHit = 2.0f; w.m_nMaxMainBeams = 3; w.m_nMaxSubBeams = 4;
    w.m_fMainBeamDecapitationTime = 1.5f; w.m_fSubBeamDecapitationTime = 0.5f; w.m_fSubBeamDelay = 0.25f;
    w.InitialiseBeams();
    int ok = g_nCalls == 3 && g_activated == 2 && w.BeamsInitialised && g_pickCalls == 1 && g_listsAlive == 0;
    ok = ok && g_calls[0].beams == &w.m_lbBeams && g_calls[0].maxBeams == 3 && g_calls[0].origin == &caster
         && g_calls[0].damage == 5.0f && g_calls[0].decap == 1.5f && g_calls[0].delay == 0.0f && g_calls[0].flag;
    ok = ok && g_calls[1].beams == &b1.Children && g_calls[1].maxBeams == 4 && g_calls[1].origin == &t1
         && g_calls[1].damage == 2.0f && g_calls[1].decap == 0.5f && g_calls[1].delay == 0.25f && !g_calls[1].flag
         && g_calls[1].targets == g_calls[0].targets && g_calls[2].origin == &t2;
    n1.Value.Info = 0; n2.Value.Info = 0;
    ok = ok && b1.MainBeam && b2.MainBeam && !b1.RightHand && !b2.RightHand && i1.Count == 1 && i2.Count == 1;
    w.WeaponState = 2; w.BeamsInitialised = false; g_nCalls = 0; g_mainResult = false;   // failure path
    w.InitialiseBeams();
    ok = ok && g_nCalls == 1 && !w.BeamsInitialised && g_activated == 2 && g_listsAlive == 0;
    if (!ok) { std::printf("007a16f0_TEST FAIL\n"); return 1; }
    std::printf("007a16f0_TEST PASS\n");
    return 0;
}