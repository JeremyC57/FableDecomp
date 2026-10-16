// CCombatActionBase::IsThingInStruckThingList(const CThingGameObject& thing) const -- find the active strike event that finishes first and report whether the thing is in its struck-thing list
// Grade: DIFFER(121v123) -- functor build: retail loads FramesRunning and the list head first, pushes ebx at entry, uses ebx as the argument-area pointer and stores the 0 / 1000000 members from registers (xor edx / mov esi,0F4240h); ours writes immediates through eax and loads begin later (tried iterator/frame locals, a three-argument functor ctor)
// Leaf: IsThingInStruckThingList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856F00, length 121 bytes (+7 int3 padding), `ret 4`.
// Label correction: the catalog row says "CTCHeroExperience::GetRoughExperienceLevel" (a `long ()` member); this body is a
// CCombatActionBase member (FramesRunning +0x1c, StrikeEvents +0xbc) taking one reference and returning bool:
// Ego_r.pdb ?IsThingInStruckThingList@CCombatActionBase@@QBE_NABVCThingGameObject@@@Z.
// Callees: std::for_each<list<CCountedPointer<CStrikeEvent>>::const_iterator, CGetActiveStrikeEventThatIsFirstToFinish>
// @ 0x0085A960 (`__fastcall`: hidden result in ecx, iterators and the 12-byte functor on the stack).
// Shape: the functor (CurrentFrame = FramesRunning, PBestStrikeEvent = 0, SmallestEndFrame = 1000000) is built straight in the
// argument area (its copy ctor is declared); `end()` is the list head, `begin()` its next; the struck list is searched with an
// index loop (STLport `*(begin() + i)`, size re-read each pass); one shared `xor al,al` exit.
// Layout: retail CCreatureActionBase is 0xa8 (PDB 0xb4), so StrikeEvents (PDB +0xc8) sits at +0xbc; CStrikeEvent (0x28 in
// retail): StruckThingList +0x1c (vector<CThingGameObject*>).
class CThingGameObject;
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CStrikeEvent {
    bool  Legacy;                               // +0x00
    bool  PrimaryLegacy;                        // +0x01
    bool  Weapon;                               // +0x02
    void* HelperPointName;                      // +0x04 CCharString
    long  StrikeEventIndex;                     // +0x08
    long  StartFrame;                           // +0x0c
    long  EndFrame;                             // +0x10
    void* PWeapon[2];                           // +0x14 CIntelligentPointer<const CThing>
    CRetailVector<CThingGameObject*> StruckThingList;   // +0x1c
};
template <class T> struct CCountedPointer { T* Data; void* Info; };
struct SStrikeNode {
    SStrikeNode* _M_next;
    SStrikeNode* _M_prev;
    CCountedPointer<CStrikeEvent> _M_data;
};
struct CStrikeListConstIterator {               // STLport _List_iterator (one node pointer)
    SStrikeNode* _M_node;
    CStrikeListConstIterator(SStrikeNode* n) : _M_node(n) {}
};
struct CStrikeEventList {                       // STLport list (4 bytes: the head node)
    SStrikeNode* _M_node;
    CStrikeListConstIterator begin() const { return CStrikeListConstIterator(_M_node->_M_next); }
    CStrikeListConstIterator end() const { return CStrikeListConstIterator(_M_node); }
};
struct CGetActiveStrikeEventThatIsFirstToFinish {
    long          CurrentFrame;
    CStrikeEvent* PBestStrikeEvent;
    long          SmallestEndFrame;
    CGetActiveStrikeEventThatIsFirstToFinish(long frame) : CurrentFrame(frame), PBestStrikeEvent(0), SmallestEndFrame(1000000) {}
    CGetActiveStrikeEventThatIsFirstToFinish(const CGetActiveStrikeEventThatIsFirstToFinish& o);
};
CGetActiveStrikeEventThatIsFirstToFinish __fastcall for_each(CStrikeListConstIterator first, CStrikeListConstIterator last,
                                                             CGetActiveStrikeEventThatIsFirstToFinish f);   // 0x0085A960

class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    unsigned char    _pad04[0x1c - 0x04];
    long             FramesRunning;             // +0x1c
    unsigned char    _pad20[0xbc - 0x20];
    CStrikeEventList StrikeEvents;              // +0xbc
    bool IsThingInStruckThingList(const CThingGameObject& thing) const;
};

bool CCombatActionBase::IsThingInStruckThingList(const CThingGameObject& thing) const
{
    CStrikeEvent* event = for_each(StrikeEvents.begin(), StrikeEvents.end(),
                                   CGetActiveStrikeEventThatIsFirstToFinish(FramesRunning)).PBestStrikeEvent;
    if (event) {
        for (unsigned int i = 0; i < event->StruckThingList.size(); ++i)
            if (event->StruckThingList[i] == &thing)
                return true;
    }
    return false;
}
