// CTCCreatureHitNotification::UpdateHitNotification() -- erase every hit flash created before the current world frame
// Leaf: ?UpdateHitNotification@CTCCreatureHitNotification
// Retail 0x008650E0, 0xC4 bytes, __fastcall plain `ret`. PDB (Ego_r): ?UpdateHitNotification@CTCCreatureHitNotification@@QAEXXZ.
// Caller: CThingCreatureBase::WorldUpdate @ 0x006666A0 (landed; "component 0xCD's update"), behind HasTC(0xCD).
// Callees: CTCBase::GetWorld() @ 0x00686A70, the static CWorld::GetWorldFrame() @ 0x0049D870, STLport
// list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::operator=(const list&) @ 0x0076FF90 (catalog
// "std::vector<CLipSyncPhoneme>::operator=" is propagated), free @ 0x00BFEA14.
// Layout (retail): CTCBase (engine/CTCBase.h); Flashes (STLport vector<NCreatureHitNotification::CFlash>) +0x0C, CFlash =
// the 4-byte list FlashGeometry + CreationFrame (8 bytes).
// Shape: the erase-or-advance loop `it = Flashes.erase(it)` / `++it` over the inline STLport vector::erase (`if (pos + 1 !=
// end()) __copy(pos + 1, finish, pos)` with the element assignment = list operator= + the frame word, `--finish`,
// `_Destroy(finish)` = the inline list destructor: clear() then the sentinel's null-tested deallocate).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#include "engine/CTCBase.h"

struct CWeaponTrailEntry { unsigned long m_data[7]; };   // 28 bytes
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };

struct CWeaponTrailList {                                // STLport list<CWeaponTrailEntry>: the node pointer only
    SListNode* m_node;
    CWeaponTrailList& operator=(const CWeaponTrailList& other);   // 0x0076FF90
    void clear()
    {
        SListNode* cur = (SListNode*)m_node->m_next;
        while (cur != m_node) {
            SListNode* tmp = cur;
            cur = (SListNode*)cur->m_next;
            free(tmp);
        }
        m_node->m_next = m_node;
        m_node->m_prev = m_node;
    }
    ~CWeaponTrailList()
    {
        clear();
        if (m_node)
            free(m_node);
    }
};

namespace NCreatureHitNotification
{
    struct CFlash {
        CWeaponTrailList FlashGeometry;                  // +0x00
        long             CreationFrame;                  // +0x04
    };
}

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T>
inline T* __copy(T* first, T* last, T* result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

struct CFlashVector {                                    // STLport vector<CFlash>
    NCreatureHitNotification::CFlash* _M_start;
    NCreatureHitNotification::CFlash* _M_finish;
    NCreatureHitNotification::CFlash* _M_end_of_storage;
    NCreatureHitNotification::CFlash* begin() { return _M_start; }
    NCreatureHitNotification::CFlash* end() { return _M_finish; }
    NCreatureHitNotification::CFlash* erase(NCreatureHitNotification::CFlash* pos)
    {
        if (pos + 1 != end())
            __copy(pos + 1, _M_finish, pos, random_access_iterator_tag(), (int*)0);
        --_M_finish;
        _Destroy(_M_finish);
        return pos;
    }
};

struct CWorld {
    static long GetWorldFrame();                         // 0x0049D870
};

struct CTCCreatureHitNotification : CTCBase {
    CFlashVector Flashes;                                // +0x0c
    CWorld* GetWorld();                                  // 0x00686A70 (CTCBase)
    void UpdateHitNotification();
};

void CTCCreatureHitNotification::UpdateHitNotification()
{
    long frame = GetWorld()->GetWorldFrame();
    for (NCreatureHitNotification::CFlash* it = Flashes.begin(); it != Flashes.end(); ) {
        if (it->CreationFrame < frame)
            it = Flashes.erase(it);
        else
            ++it;
    }
}