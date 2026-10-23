// CTCBoastingPodium::FrameUpdate() -- once the boast list changes (frame stamp), hand the current boasts to the podium's boasting positions: with more boasts than free positions reset every position and assign boasts to the position groups picked by the boast count; with new boasts assign each unclaimed boast to the next idle position; with no boasts stop every active position; then restamp and refresh the owner
// Grade: DIFFER(1372v1372) -- same size and block order; retail holds the boast list (manager+0x68) in ebx and spills it, shares one stack slot for the interface key and the group temporaries, and pushes ebp/esi after the frame test (ours: manager spilled, separate slots, frame 0x28 v 0x1c)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00813D00, 1376 bytes, `ret`.
extern "C" void __cdecl free(void*);                    // 0x00BFEA14
void* operator new(unsigned int size);                  // 0x00BFEA0E
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    void UpdateA();                                     // 0x004C7C40
    void UpdateB(long flags);                           // 0x004C7590
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTCInline(long type)
    {
        CInterfaceMap::SEntry* first = m_interfaces.first;
        CInterfaceMap::SEntry* last = m_interfaces.last;
        long count = last - first;
        while (count > 0) {
            long half = count / 2;
            if (first[half].type < type) {
                first += half + 1;
                count -= half + 1;
            } else
                count = half;
        }
        return (T*)((first == last || type < first->type) ? last : first)->component;
    }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct SBoast { long Id; long Data; };
struct CTCBoaster {
    unsigned char _pad00[0x20];
    bool Active;                                        // +0x20
    unsigned char _pad21[0x24 - 0x21];
    long BoastId;                                       // +0x24
    unsigned char _pad28[0x2c - 0x28];
    bool Boasting;                                      // +0x2c
    void StartBoast(long id, long data);                // 0x00781F10
    void Reset();                                       // 0x00781FE0
    void StopBoast();                                   // 0x00782060
};
struct SBoastNode { SBoastNode* Next; SBoastNode* Prev; SBoast Data; };
struct CBoastManager {
    unsigned char _pad00[0x64];
    unsigned long FrameChanged;                         // +0x64
    SBoastNode* Boasts;                                 // +0x68 (STLport list head)
};
extern CBoastManager* g_pBoastManager;                  // 0x013B89FC
struct SPendingNode { SPendingNode* Next; SPendingNode* Prev; SBoast* Data; };
struct CLongVector {
    long* First; long* Last; long* End;
    CLongVector() : First(0), Last(0), End(0) { }
    ~CLongVector() { if (First) free(First); }
    void push_back(const long& value);                  // 0x00485FF3
    unsigned long size() const { return Last - First; }
};
struct SPosition { long Group; CBaseIntelligentPointer Thing; };
struct CPositionMap {
    SPosition* First; SPosition* Last; SPosition* End;
    SPosition* upper_bound(const long& key);            // 0x008146A0
    SPosition* lower_bound(const long& key);            // 0x006A29B0
    unsigned long size() const { return Last - First; }
};
unsigned long GetFrameNumber();                         // 0x0049D870

class CTCBoastingPodium {
public:
    void* __vftable;
    CThing* Owner;                                      // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CPositionMap BoastingPositions;                     // +0x0c
    long _pad18;                                        // +0x18 (CVectorMap comparator)
    long LastPositionsWithBoastCount;                   // +0x1c
    unsigned long FrameBoastListFrom;                   // +0x20
    bool BoastingActive;                                // +0x24
    long GetNumberOfPositions();                        // 0x00813B80
    void* GetWorld();                                   // 0x00686A80
    void FrameUpdate();
};

void CTCBoastingPodium::FrameUpdate()
{
    CBoastManager* manager = g_pBoastManager;
    if (FrameBoastListFrom == 0 || manager->FrameChanged >= FrameBoastListFrom) {
        SBoastNode** boasts = &manager->Boasts;
        long count = 0;
        for (SBoastNode* n = (*boasts)->Next; n != *boasts; n = n->Next)
            ++count;
        BoastingActive = count > 0;
        if (count > GetNumberOfPositions()) {
            for (unsigned long i = 0; i < BoastingPositions.size(); ++i) {
                CThing* thing = BoastingPositions.First[i].Thing.GetPItem();
                CTCBoaster* boaster;
                if (thing->m_presentBits[3] & 0x20000)
                    boaster = thing->GetTC<CTCBoaster>(0x71);
                boaster->Reset();
            }
            LastPositionsWithBoastCount = 0;
            CLongVector groups;
            long group;
            switch (count) {
            case 1: group = 1; groups.push_back(group); break;
            case 2: group = 2; groups.push_back(group); break;
            case 3: group = 1; groups.push_back(group); group = 3; groups.push_back(group); break;
            case 4: group = 2; groups.push_back(group); group = 4; groups.push_back(group); break;
            }
            SBoastNode* boast = (*boasts)->Next;
            for (unsigned long j = 0; j < groups.size(); ++j) {
                SPosition* last = BoastingPositions.upper_bound(groups.First[j]);
                for (SPosition* p = BoastingPositions.lower_bound(groups.First[j]); p != last; ++p) {
                    CThing* thing = p->Thing.GetPItem();
                    CTCBoaster* boaster;
                    if (thing->m_presentBits[3] & 0x20000)
                        boaster = thing->GetTCInline<CTCBoaster>(0x71);
                    boaster->StartBoast(boast->Data.Id, boast->Data.Data);
                    ++LastPositionsWithBoastCount;
                    boast = boast->Next;
                }
            }
        } else if (count > LastPositionsWithBoastCount) {
            SPendingNode* pending = (SPendingNode*)operator new(sizeof(SPendingNode));
            pending->Next = pending;
            pending->Prev = pending;
            for (SBoastNode* n = (*boasts)->Next; n != *boasts; n = n->Next) {
                unsigned long i;
                for (i = 0; i < BoastingPositions.size(); ++i) {
                    CThing* thing = BoastingPositions.First[i].Thing.GetPItem();
                    CTCBoaster* boaster;
                    if (thing->m_presentBits[3] & 0x20000)
                        boaster = thing->GetTC<CTCBoaster>(0x71);
                    if (boaster->Boasting && n->Data.Id == boaster->BoastId)
                        goto claimed;
                }
                {
                    SPendingNode* node = (SPendingNode*)operator new(sizeof(SPendingNode));
                    SBoast** data = &node->Data;
                    if (data)
                        *data = &n->Data;
                    node->Next = pending;
                    node->Prev = pending->Prev;
                    pending->Prev->Next = node;
                    pending->Prev = node;
                }
            claimed:;
            }
            SPendingNode* next = pending->Next;
            for (unsigned long i = 0; i < BoastingPositions.size(); ++i) {
                if (next == pending)
                    break;
                CThing* thing = BoastingPositions.First[i].Thing.GetPItem();
                CTCBoaster* boaster;
                if (thing->m_presentBits[3] & 0x20000)
                    boaster = thing->GetTC<CTCBoaster>(0x71);
                if (boaster->Active && !boaster->Boasting) {
                    boaster->StartBoast(next->Data->Id, next->Data->Data);
                    next = next->Next;
                    ++LastPositionsWithBoastCount;
                }
            }
            SPendingNode* cur = pending->Next;
            while (cur != pending) {
                SPendingNode* dead = cur;
                cur = cur->Next;
                free(dead);
            }
            pending->Next = pending;
            pending->Prev = pending;
            free(pending);
        } else if (count == 0) {
            for (unsigned long i = 0; i < BoastingPositions.size(); ++i) {
                CThing* thing = BoastingPositions.First[i].Thing.GetPItem();
                CTCBoaster* boaster;
                if (thing->m_presentBits[3] & 0x20000)
                    boaster = thing->GetTC<CTCBoaster>(0x71);
                if (boaster->Active)
                    boaster->StopBoast();
            }
            LastPositionsWithBoastCount = 0;
        }
        GetWorld();
        FrameBoastListFrom = GetFrameNumber();
    }
    Owner->UpdateA();
    Owner->UpdateB(4);
}
