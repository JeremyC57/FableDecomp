// CTCFollowed::FrameUpdate() -- every 10th frame (staggered by UID) culls dead followers, drops the ones no longer willing to follow (posting deed 0x31 for each), resets the idle timer when the hero moves more than 3 units, otherwise after 5 s idle posts deed 0x34 for a random follower at most every 10 s
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D53C0, 800 bytes, `ret` (aligned ebp frame from the __int64 UID math).
// Callees: CTCBase::GetWorld @ 0x00686A70 (result unused: CWorld::GetFrame @ 0x0049D870 is static), __aullrem @ 0x00BFEBB0,
//          CullNullFollowers @ 0x006D5260, LowerBound @ 0x0040F020 / 0x004365B0 (two CVectorMap instances),
//          CTCCreatureOpinionOfHero::IsWillingToContinueFollowingHero @ 0x006818F0, follower-registry remove @ 0x0065DE8A,
//          CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10, notify @ 0x004378C0 on the global
//          at 0x013B8790, TC 0x7A AddDependentThing-style call @ 0x006AEBA0, CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990,
//          OnFollowerLeft @ 0x006D5050, CThing::GetPosition @ 0x004C73D0, GetRandom @ 0x00686DA0, list size @ 0x0065F17B,
//          GFRandom @ 0x004CDAE0 (__fastcall range, rng), free @ 0x00BFEA14. Frames-per-second global 0x01375550.
// Layout: CTCFollowed registry +0x0C, Followers (STLport list, node pointer only) +0x10, FrameLastIdleReaction +0x14,
//         FramesSincePlayerMoved +0x18, LastPlayerPosition +0x1C; CThing UID +0x18, TC present bits +0x20, TC map +0x44.
// Shape: IsTimeToUpdate loads GetWorld() into a named local and the UID before the static GetFrame call; a reference local to
//        Followers._M_node gives retail's cached &head (edi) in the culling loop, while the random-pick loop uses begin()/end()
//        and reloads the head each pass; CThing is modelled with a plain _vfptr word (a virtual would pad the __int64 UID to +0x20);
//        the deed TC and the TC 0x7A pointer are uninitialised when the TC is missing, as in retail; the idle-reaction follower
//        and Thing are named locals so they are read before the PostDeed pushes.
// Vtable check: RELOCATION_MATCH; the only relocation beyond callees/globals is the CIntelligentPointer vftable 0x01238C6C (rtti-vtables.tsv).
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
    __forceinline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    T* Get() const { return (T*)GetPItem(); }
};
struct C3DVector { float X, Y, Z; };
struct CTCPair { long Type; void* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x0040F020
    CTCPair* LowerBoundConst(const long& key);          // 0x004365B0
    __forceinline void* Find(long type)
    {
        long key = type;
        CTCPair* it = LowerBound(key);
        if (it == _M_finish || it->Type > type)
            it = _M_finish;
        return it->TC;
    }
    __forceinline void* FindConst(long type)
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
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CTCCreatureOpinionOfHero {
public:
    bool IsWillingToContinueFollowingHero() const;      // 0x006818F0
};
class CTCHeroOpinionDeedLog {
public:
    void PostDeed(long deed, const C3DVector& pos, CThing* a, CThing* b, bool notify, long p0, long p1, long p2, long p3, long p4);   // 0x006C6990
};
class CTCDependents {
public:
    void AddDependentThing(CThing* thing);              // 0x006AEBA0
};
class CFollowerNotify {
public:
    void Notify(long value);                            // 0x004378C0
};
extern CFollowerNotify* g_FollowerNotify;               // 0x013B8790
extern long g_FramesPerSecond;                          // 0x01375550
class CWorld {
public:
    static long GetFrame();                             // 0x0049D870
};
class CRandom;
long __fastcall GFRandom(long range, CRandom* rng);     // 0x004CDAE0
extern "C" void free(void*);                            // 0x00BFEA14
struct CFollowerNode {
    CFollowerNode* _M_next;
    CFollowerNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;
};
class CFollowerRegistry {
public:
    void Remove(const CIntelligentPointer<CThing>& follower);   // 0x0065DE8A
};
struct CFollowerList {
    CFollowerNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    CFollowerNode* begin() const { return _M_node->_M_next; }
    CFollowerNode* end() const { return _M_node; }
    long size() const;                                  // 0x0065F17B
    __forceinline CFollowerNode* erase(CFollowerNode* pos)
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
    CWorld* GetWorld() const;                           // 0x00686A70
    CRandom* GetRandom() const;                         // 0x00686DA0
};
class CTCFollowed : public CTCBase {
public:
    CFollowerRegistry Registry;                         // +0x0C
    unsigned char _pad0d[3];
    CFollowerList Followers;                            // +0x10
    long FrameLastIdleReaction;                         // +0x14
    long FramesSincePlayerMoved;                        // +0x18
    C3DVector LastPlayerPosition;                       // +0x1C
    void CullNullFollowers();                           // 0x006D5260
    void OnFollowerLeft(CThing* follower);              // 0x006D5050
    __forceinline bool IsTimeToUpdate() const
    {
        CWorld* world = GetWorld();
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
        if (!Followers.empty()) {
            CFollowerNode*& head = Followers._M_node;
            CTCHeroOpinionDeedLog* deeds;
            if (Thing->HasTC(0x51))
                deeds = (CTCHeroOpinionDeedLog*)Thing->TCs.Find(0x51);
            for (CFollowerNode* node = head->_M_next; node != head; ) {
                CThing* follower = node->_M_data.Get();
                if (follower->HasTC(0x4f) && ((CTCCreatureOpinionOfHero*)follower->TCs.FindConst(0x4f))->IsWillingToContinueFollowingHero()) {
                    node = node->_M_next;
                    continue;
                }
                node = Followers.erase(node);
                Registry.Remove(CIntelligentPointer<CThing>(follower));
                g_FollowerNotify->Notify(0);
                CTCDependents* dependents;
                if (Thing->HasTC(0x7a))
                    dependents = (CTCDependents*)Thing->TCs.Find(0x7a);
                dependents->AddDependentThing(follower);
                deeds->PostDeed(0x31, Thing->GetPosition(), follower, follower, true, 0, 0, 0, 0, 0);
                OnFollowerLeft(follower);
            }
            const C3DVector& pos = Thing->GetPosition();
            float dx = pos.X - LastPlayerPosition.X;
            float dy = pos.Y - LastPlayerPosition.Y;
            if (dx * dx + dy * dy > 9.0f) {
                LastPlayerPosition = Thing->GetPosition();
                FramesSincePlayerMoved = 0;
            } else if (FramesSincePlayerMoved > g_FramesPerSecond * 5.0f) {
                CWorld* world = GetWorld();
                long fps = g_FramesPerSecond;
                if (world->GetFrame() - FrameLastIdleReaction > fps * 10.0f) {
                    long pick = GFRandom(Followers.size(), GetRandom());
                    long n = 0;
                    for (CFollowerNode* node = Followers.begin(); node != Followers.end(); node = node->_M_next, ++n) {
                        if (n == pick) {
                            CIntelligentPointer<CThing>& p = node->_M_data;
                            CThing* follower = p.Get();
                            CThing* thing = Thing;
                            deeds->PostDeed(0x34, thing->GetPosition(), follower, p.Get(), true, 0, 0, 0, 0, 0);
                            FrameLastIdleReaction = GetWorld()->GetFrame();
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