// CRetailTree_stringkey::find(const CCharString&) -- lower-bound walk with the inlined CCharString::operator< (null data blocks ordered first, CBasicString::operator< @ 0x00429950 otherwise), then end() unless the key matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9370, oracle length 112 bytes.
// Grade: candidate -- still UNGRADED in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push ebx ; push ebp ; mov ebp,[ecx] ; push esi ; mov esi,[ebp+4] ; test esi,esi ; mov ebx,ebp ; je E ; mov eax,[key] ; push edi ; mov edi,[eax] ; L: mov ecx,[esi+10h] ; cmp edi,ecx ; je Lf ; test ecx,ecx ; je Rt ; test edi,edi ; je Lf ; push edi ; call CBasicString::operator< ; test al,al ; jne Rt ; Lf: mov ebx,esi ; mov esi,[esi+8] ; jmp T ; Rt: mov esi,[esi+0Ch] ; T: test esi,esi ; jne L ; pop edi ; E: cmp ebx,ebp ; je H ; mov ecx,[key] ; mov eax,[ebx+10h] ; mov ecx,[ecx] ; cmp eax,ecx ; je F ; test ecx,ecx ; je H ; test eax,eax ; je F ; push eax ; call CBasicString::operator< ; test al,al ; je F ; H: (return head) ; F: (return node)
// std::map/set::find on the retail tree keyed by CCharString (the key's data-block pointer at
// +0x10 of the node): lower_bound descends from head->parent (right when node.key < key, else
// remember the node and go left), then the result is end() (the head) when nothing was found or
// key < found.key. The string `operator<` is inlined: equal data blocks are not less, a null
// block sorts first, otherwise CBasicString<char>::operator< @ 0x00429950 decides. The node
// pointer is returned directly (`ret 4`). One of 17 bodies of this shape.
struct CBasicString {
    bool operator<(const CBasicString& other);   // 0x00429950 (__thiscall, takes the other data block)
};

struct CCharString {
    CBasicString* m_pData;
    bool operator<(const CCharString& other) const
    {
        if (m_pData == other.m_pData)
            return false;
        if (m_pData == 0)
            return true;
        if (other.m_pData == 0)
            return false;
        return m_pData->operator<(*other.m_pData);
    }
};

struct CRetailTree_stringkey {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; CCharString key; };
    node* head;
    unsigned long size;
    node* find(const CCharString& key);
};

CRetailTree_stringkey::node* CRetailTree_stringkey::find(const CCharString& key)
{
    node* result = head;
    node* n = head->parent;
    while (n) {
        if (!(n->key < key)) {            // Dinkumware _Lbound: `if (!comp(key(node), k))`
            result = n;
            n = n->left;
        }
        else
            n = n->right;
    }
    if (result == head || key < result->key)
        return head;                       // end()
    return result;
}
