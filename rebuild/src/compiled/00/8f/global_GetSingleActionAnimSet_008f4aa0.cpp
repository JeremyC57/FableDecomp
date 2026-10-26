// GetSingleActionAnimSet(CThingCreatureBase* creature, long action) -- the creature's animation set for `action` when the action's set list holds exactly one entry, else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4AA0, length 88 bytes (96 with padding).
// Callees: GetActionAnimSetList(vector<CAnimSet*>*, CThingCreatureBase*, long) @ 0x008F4990 (`__fastcall`: the list in ecx, the creature in edx, the action
// on the stack), free @ 0x00BFEA14 (the list's buffer). Shape: a `__fastcall` free function (creature in ecx, action in edx); the three-word vector local is
// zeroed from esi, `(last - first) & ~3 == 4` is `size() == 1`, the element is read before the buffer is freed, and the vector destructor is inlined once per
// return path (the single-element path has no null test on the buffer it has just read through). The chunk label (CEngineInternalPrimitiveRepeatedStaticMesh::
// GetBoundingBoxTransform) is propagated and wrong; the body is byte-identical to 0x00693250.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CAnimSet;
class CThingCreatureBase;

struct CAnimSetVector {                            // vector<CAnimSet*>: first, last, end over a malloc'd buffer
    CAnimSet** m_pFirst;
    CAnimSet** m_pLast;
    CAnimSet** m_pEnd;
    CAnimSetVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CAnimSetVector()
    {
        if (m_pFirst)
            free(m_pFirst);
    }
    unsigned long size() const { return m_pLast - m_pFirst; }
};

void __fastcall GetActionAnimSetList_008f4990(CAnimSetVector* list, CThingCreatureBase* creature, long action);   // 0x008F4990

CAnimSet* __fastcall GetSingleActionAnimSet(CThingCreatureBase* creature, long action)
{
    CAnimSetVector list;
    GetActionAnimSetList_008f4990(&list, creature, action);
    if (list.size() == 1)
        return list.m_pFirst[0];
    return 0;
}