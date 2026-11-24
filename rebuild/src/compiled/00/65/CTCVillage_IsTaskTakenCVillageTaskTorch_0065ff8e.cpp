// IsListedNodeActive_0065ff8e(CNodeRef ref, CNodeList* list) -- finds the node in the intrusive list and asks its object's slot-9 virtual; false when the node is not listed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065FF8E, length 60 bytes.
// Callees: CNodeRef::Find @ 0x0065F943 (thiscall on the by-value first argument's own slot,
// three stack arguments: two 4-byte iterator copies with user copy constructors and the raw node
// pointer; it forwards to the tagged STL-style find @ 0x0065EA75 and returns `this`), then the
// found node's object (+8) slot-9 virtual (+0x24).
// The manifest label "CAIBrain::GetCurrentStateGroupDebugText" is propagated and rejected.
// Frame-pointer body with `push dword ptr [ebp+8]`, no spills, inlined begin()/end() and
// `pop ebp ; ret 8`: a size-optimised TU with /Oy off (optimize("y",off) + ("s",on); the "gsy"
// form disables the inlining and spills every temporary).
// Argument order (right to left): the raw node (`push [ebp+8]`), then end() = a copy of the
// list head, then begin() = a copy of head->next, each built in its reserved slot
// (`push ecx ; mov eax,esp ; mov [eax],ecx`: 4-byte class with a user copy constructor). The
// result is compared with the head sentinel (`cmp eax,[esi]`); ecx is unused, so this is a
// __stdcall free function (or a static member) taking the ref by value.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CActiveObject {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual bool IsActive();                // slot 9 (+0x24)
};

struct CListNode {
    CListNode*     next;                    // +0x00
    CListNode*     prev;                    // +0x04
    CActiveObject* m_pObject;               // +0x08
};

struct CListIterator {                      // 4-byte class with a user copy constructor
    CListNode* p;
    CListIterator(CListNode* node) : p(node) { }
    CListIterator(const CListIterator& other) : p(other.p) { }
};

struct CNodeList {
    CListNode* head;                        // sentinel: head->next is begin(), head is end()
    CListIterator begin() const { return CListIterator(head->next); }
    CListIterator end() const { return CListIterator(head); }
};

struct CNodeRef {                           // POD: pushed as a dword, address taken for the member call
    CListNode* p;
    CNodeRef& Find(CListIterator first, CListIterator last, CListNode* value);   // 0x0065F943
};

bool __stdcall IsListedNodeActive_0065ff8e(CNodeRef ref, CNodeList* list)
{
    ref.Find(list->begin(), list->end(), ref.p);
    if (ref.p == list->head)
        return false;
    return ref.p->m_pObject->IsActive();
}