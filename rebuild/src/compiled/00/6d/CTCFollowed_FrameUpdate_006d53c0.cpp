// CTCFollowed::FrameUpdate() -- every 10th frame (staggered by UID) culls dead followers, drops the ones no longer willing to follow (posting deed 0x31 for each), resets the idle timer when the hero moves more than 3 units, otherwise after 5 s idle posts deed 0x34 for a random follower at most every 10 s
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D53C0, 800 bytes, `ret` (aligned ebp frame from the __int64 UID math).
// Callees: CTCBase::GetWorld @ 0x00686A70 (result unused: CWorld::GetFrame @ 0x0049D870 is static), __aullrem @ 0x00BFEBB0,
//          CullNullFollowers @ 0x006D5260, LowerBound @ 0x0040F020 / 0x004365B0 (two CVectorMap instances),
//          CTCCreatureOpinionOfHero::IsWillingToContinueFollowingHero @ 0x006818F0, std::list<CIntelligentPointer<CThing> >::remove @ 0x0065DE8A,
//          CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10, notify @ 0x004378C0 on the global
//          at 0x013B8790, TC 0x7A AddDependentThing-style call @ 0x006AEBA0, CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990,
//          OnFollowerLeft @ 0x006D5050, CThing::GetPos @ 0x004C73D0, GetRandom @ 0x00686DA0, list size @ 0x0065F17B,
//          GFRandom @ 0x004CDAE0 (__fastcall range, rng), free @ 0x00BFEA14. Frames-per-second global 0x01375550.
// Layout: CTCFollowed Followers (STLport list) +0x0C, AIFollowers (STLport list, node pointer only) +0x10, FrameLastIdleReaction +0x14,
//         FramesSincePlayerMoved +0x18, LastPlayerPosition +0x1C; CThing UID +0x18, TC present bits +0x20, TC map +0x44.
// Shape: IsTimeToUpdate loads GetWorld() into a named local and the UID before the static GetFrame call; a reference local to
//        AIFollowers._M_node gives retail's cached &head (edi) in the culling loop, while the random-pick loop uses begin()/end()
//        and reloads the head each pass; CThing is modelled with a plain _vfptr word (a virtual would pad the __int64 UID to +0x20);
//        the deed TC and the TC 0x7A pointer are uninitialised when the TC is missing, as in retail; the idle-reaction follower
//        and Thing are named locals so they are read before the PostDeed pushes.
// Vtable check: RELOCATION_MATCH; the only relocation beyond callees/globals is the CIntelligentPointer vftable 0x01238C6C (rtti-vtables.tsv).
#include "fable_compiler.h"
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* PItem;
};
class CThing;
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    FABLE_FORCEINLINE CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    T* Get() const { return (T*)GetPItem(); }
};
#include "engine/C3DVector.h"
struct CTCPair { long Type; void* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x0040F020
    CTCPair* LowerBoundConst(const long& key);          // 0x004365B0
    FABLE_FORCEINLINE void* Find(long type)
    {
        long key = type;
        CTCPair* it = LowerBound(key);
        if (it == _M_finish || it->Type > type)
            it = _M_finish;
        return it->TC;
    }
    FABLE_FORCEINLINE void* FindConst(long type)
    {
        long key = type;
        CTCPair* it = LowerBoundConst(key);
        if (it == _M_finish || it->Type > type)
            it = _M_finish;
        return it->TC;
    }
};
class CThing {
public:
    void* _vfptr;
    unsigned char _pad04[0x18 - 0x04];
    unsigned __int64 UID;                               // +0x18
    unsigned long PresentBits[9];                       // +0x20
    CTCMap TCs;                                         // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPos() const;               // 0x004C73D0
};
class CTCCreatureOpinionOfHero {
public:
    bool IsWillingToContinueFollowingHero() const;      // 0x006818F0
};
#include "engine/EOpinionDeedType.h"
enum EOpinionPostDeedType { };                   // Ego_r.pdb enum (values used here by number)
class CFaction;
class CThingAICreature;
class CTCHeroOpinionDeedLog {
public:
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1);   // 0x006C6990 Ego_r.pdb / FableWin.pdb (engine/CTCHeroOpinionDeedLog.h)
};
class CTCDependents {
public:
    void AddDependentThing(CThing* thing);              // 0x006AEBA0
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui {
public:
    void UpdateMiniMapMarkers(bool value);              // 0x004378C0
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld {
public:
    static long __fastcall GetFrame();                             // 0x0049D870
};
class CRandom;
long __fastcall GFRandom(long range, CRandom* rng);     // 0x004CDAE0
extern "C" void free(void*);                            // 0x00BFEA14
struct CFollowerNode {
    CFollowerNode* _M_next;
    CFollowerNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;
};
namespace std {                                         // STLport 4.6 list (one node pointer)
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;
    void remove(const _Tp& value);                      // 0x0065DE8A for _Tp = CIntelligentPointer<CThing> (FableWin ?remove@?$list@..)
};
}
struct CFollowerList {
    CFollowerNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    CFollowerNode* begin() const { return _M_node->_M_next; }
    CFollowerNode* end() const { return _M_node; }
    long size() const;                                  // 0x0065F17B
    FABLE_FORCEINLINE CFollowerNode* erase(CFollowerNode* pos)
    {
        CFollowerNode* next = pos->_M_next;
        CFollowerNode* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        pos->_M_data.~CIntelligentPointer<CThing>();
        free(pos);
        return next;
    }
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    CRandom* GetRandom() const;                         // 0x00686DA0
protected:
    CWorld& GetWorld() const;   // 0x00686A70 (Ego_r.pdb CTCBase, protected)
};
class CTCFollowed : public CTCBase {
public:
    std::list<CIntelligentPointer<CThing> > Followers;  // +0x0C (Ego_r.pdb CTCFollowed::Followers)
    CFollowerList AIFollowers;                          // +0x10 (Ego_r.pdb CTCFollowed::AIFollowers; element modelled as CIntelligentPointer<CThing>)
    long FrameLastIdleReaction;                         // +0x14
    long FramesSincePlayerMoved;                        // +0x18
    C3DVector LastPlayerPosition;                       // +0x1C
    void CullNullFollowers();                           // 0x006D5260
    void OnFollowerLeft(CThing* follower);              // 0x006D5050
    FABLE_FORCEINLINE bool IsTimeToUpdate() const
    {
        CWorld* world = &GetWorld();
        unsigned __int64 uid = Thing->UID;
        return (world->GetFrame() + uid) % 10 == 0;
    }
    void FrameUpdate();
};

void CTCFollowed::FrameUpdate()
{
    ++FramesSincePlayerMoved;
    if (IsTimeToUpdate()) {
        CullNullFollowers();
        if (!AIFollowers.empty()) {
            CFollowerNode*& head = AIFollowers._M_node;
            CTCHeroOpinionDeedLog* deeds;
            if (Thing->HasTC(0x51))
                deeds = (CTCHeroOpinionDeedLog*)Thing->TCs.Find(0x51);
            for (CFollowerNode* node = head->_M_next; node != head; ) {
                CThing* follower = node->_M_data.Get();
                if (follower->HasTC(0x4f) && ((CTCCreatureOpinionOfHero*)follower->TCs.FindConst(0x4f))->IsWillingToContinueFollowingHero()) {
                    node = node->_M_next;
                    continue;
                }
                node = AIFollowers.erase(node);
                Followers.remove(CIntelligentPointer<CThing>(follower));
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
                CTCDependents* dependents;
                if (Thing->HasTC(0x7a))
                    dependents = (CTCDependents*)Thing->TCs.Find(0x7a);
                dependents->AddDependentThing(follower);
                deeds->PostDeed(OPINION_DEED_TYPE_FOLLOWER_QUIT, Thing->GetPos(), follower, (CThingAICreature*)follower, (EOpinionPostDeedType)1, false, false, 0, false, 0);
                OnFollowerLeft(follower);
            }
            const C3DVector& pos = Thing->GetPos();
            float dx = pos.X - LastPlayerPosition.X;
            float dy = pos.Y - LastPlayerPosition.Y;
            if (dx * dx + dy * dy > 9.0f) {
                LastPlayerPosition = Thing->GetPos();
                FramesSincePlayerMoved = 0;
            } else if (FramesSincePlayerMoved > CMainGameComponent::GetConstantFPS() * 5.0f) {
                CWorld* world = &GetWorld();
                long fps = CMainGameComponent::GetConstantFPS();
                if (world->GetFrame() - FrameLastIdleReaction > fps * 10.0f) {
                    long pick = GFRandom(AIFollowers.size(), GetRandom());
                    long n = 0;
                    for (CFollowerNode* node = AIFollowers.begin(); node != AIFollowers.end(); node = node->_M_next, ++n) {
                        if (n == pick) {
                            CIntelligentPointer<CThing>& p = node->_M_data;
                            CThing* follower = p.Get();
                            CThing* thing = Thing;
                            deeds->PostDeed(OPINION_DEED_TYPE_FOLLOWER_IDLEBORED, thing->GetPos(), follower, (CThingAICreature*)p.Get(), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                            FrameLastIdleReaction = GetWorld().GetFrame();
                            break;
                        }
                    }
                }
            }
        }
    }
    if (IsTimeToUpdate())
        CullNullFollowers();
}