#pragma optimize("y", off)
#pragma optimize("s", on)
// CRetailList_0053cb02::Insert(SIterator where, SIterator first, SIterator last) -- the range insert forwarder: the three 4-byte iterator classes by value and an uninitialised one-byte tag temporary homed in a dead argument slot (`lea eax,[ebp+13h]`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CB02, length 30 bytes.
// Callees: the range insert @ 0x0053CAA0 (on the list: the position iterator by value -- its copy constructed in place in the reserved slot (`push ecx ; mov eax,esp ;
// mov [eax],edx`) -- the two range words and the tag's address). Shape: a frame pointer (the game module), no frame beyond the saved ebp, `ret 0Ch`. The chunk label
// "CEngineSceneGrid::GetCellBoundingBox" is propagated across the family; named by effect.
struct STag_0053cb02 { STag_0053cb02() { } };            // a user constructor: nothing is written, the temporary is homed in the dead argument slot

struct SIterator_0053cb02 {                           // a 4-byte iterator class with a user copy constructor (passed by value through a reserved slot)
    void* m_pNode;
    SIterator_0053cb02(const SIterator_0053cb02& other) : m_pNode(other.m_pNode) { }
};

struct CRetailList_0053cb02 {
    void* m_pHead;
    void InsertRange(SIterator_0053cb02 where, void* first, void* last, const STag_0053cb02& tag);   // 0x0053CAA0
    void Insert(SIterator_0053cb02 where, void* first, void* last);
};

void CRetailList_0053cb02::Insert(SIterator_0053cb02 where, void* first, void* last)
{
    InsertRange(where, first, last, STag_0053cb02());
}
