#pragma optimize("y", off)
#pragma optimize("s", on)
// CRetailList_00579786::Insert(SIterator where, SIterator first, SIterator last) -- the range insert forwarder: the three 4-byte iterator classes by value and an uninitialised one-byte tag temporary homed in a dead argument slot (`lea eax,[ebp+13h]`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00579786, length 30 bytes.
// Callees: the range insert @ 0x005794C3 (on the list: the position iterator by value -- its copy constructed in place in the reserved slot (`push ecx ; mov eax,esp ;
// mov [eax],edx`) -- the two range words and the tag's address). Shape: a frame pointer (the game module), no frame beyond the saved ebp, `ret 0Ch`. The chunk label
// "CEngineSceneGrid::GetCellBoundingBox" is propagated across the family; named by effect.
struct STag_00579786 { STag_00579786() { } };            // a user constructor: nothing is written, the temporary is homed in the dead argument slot

struct SIterator_00579786 {                           // a 4-byte iterator class with a user copy constructor (passed by value through a reserved slot)
    void* m_pNode;
    SIterator_00579786(const SIterator_00579786& other) : m_pNode(other.m_pNode) { }
};

struct CRetailList_00579786 {
    void* m_pHead;
    void InsertRange(SIterator_00579786 where, void* first, void* last, const STag_00579786& tag);   // 0x005794C3
    void Insert(SIterator_00579786 where, void* first, void* last);
};

void CRetailList_00579786::Insert(SIterator_00579786 where, void* first, void* last)
{
    InsertRange(where, first, last, STag_00579786());
}
