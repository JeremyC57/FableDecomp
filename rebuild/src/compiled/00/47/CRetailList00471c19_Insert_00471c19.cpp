#pragma optimize("y", off)
#pragma optimize("s", on)
// CRetailList_00471c19::Insert(SIterator where, SIterator first, SIterator last) -- the range insert forwarder: the three 4-byte iterator classes by value and a zero-initialised one-byte tag local (`stosb`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471C19, length 39 bytes.
// Callees: the range insert @ 0x00471C40 (on the list: the position iterator by value -- its copy constructed in place in the reserved slot (`push ecx ; mov eax,esp ;
// mov [eax],edx`) -- the two range words and the tag's address). Shape: a frame pointer (the game module), the tag local in the one-word frame (`push ecx`), edi saved for the `stosb`, `ret 0Ch`. The chunk label
// "CEngineSceneGrid::GetCellBoundingBox" is propagated across the family; named by effect.
struct STag_00471c19 { unsigned char m_tag; };   // POD: `STag()` value-initialises it (the `xor eax,eax ; stosb` into the frame byte)

struct SIterator_00471c19 {                           // a 4-byte iterator class with a user copy constructor (passed by value through a reserved slot)
    void* m_pNode;
    SIterator_00471c19(const SIterator_00471c19& other) : m_pNode(other.m_pNode) { }
};

struct CRetailList_00471c19 {
    void* m_pHead;
    void InsertRange(SIterator_00471c19 where, void* first, void* last, const STag_00471c19& tag);   // 0x00471C40
    void Insert(SIterator_00471c19 where, void* first, void* last);
};

void CRetailList_00471c19::Insert(SIterator_00471c19 where, void* first, void* last)
{
    InsertRange(where, first, last, STag_00471c19());
}