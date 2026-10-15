// CScriptConversation::IsValid() const -- false for an empty conversation or when any person in it is gone (no item, or the dead bit set); true otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5600, length 91 bytes.
// Leaf: ?IsValid@CScriptConversation
// Retail bytes: push esi ; mov esi,ecx ; mov eax,[esi+8] ; cmp eax,[esi+0Ch] ; jne N ; xor al,al ; pop esi ; ret ; N: mov ecx,[esi+0Ch] ; push ebp ; push edi ;
//               sub ecx,[esi+8] ; sar ecx,3 ; xor ebp,ebp ; test ecx,ecx ; jbe T ; L: mov edx,[esi+8] ; lea edi,[edx+ebp*8] ; mov ecx,edi ; call GetPItem ;
//               test eax,eax ; je F ; mov ecx,edi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; mov eax,[esi+0Ch] ; sub eax,[esi+8] ; inc ebp ;
//               sar eax,3 ; cmp ebp,eax ; jb L ; T: ... mov al,1 ; ret ; F: ... xor al,al ; ret
// Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice per person: the landed IS_THING_ALIVE idiom, item set and
// dead bit +0x91 bit 0 clear).
// Layout: PeopleInConversation (vector<CIntelligentPointer<CThing> >, 8-byte elements) has its first/last words at +8/+0xc,
// as in engine/CScriptConversation.h (the PDB vector keeps its allocator word last); retail vectors are 12 bytes, so the
// members after it sit 4 bytes lower than the header's -- the class is modelled here with the retail vector.
// Shape: size() and the element address are re-derived from the members on every iteration (unsigned index, `jb`).
#include "engine/CScriptConversation.h"

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                   // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {               // CIntelligentPointer<CThing>: 8 bytes
    virtual void v00();
    CThing* m_pItem;                           // +0x04
    CThing* GetPItem() const;                  // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CRetailPointerVector {                  // retail vector<CIntelligentPointer<CThing> >: first, last, end of storage
    CBaseIntelligentPointer* m_pFirst;
    CBaseIntelligentPointer* m_pLast;
    CBaseIntelligentPointer* m_pEnd;
    bool empty() const { return m_pFirst == m_pLast; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    const CBaseIntelligentPointer& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CScriptConversationRetail {
    void*                Manager;              // +0x00 CScriptConversationManager*
    long                 ConversationIndex;    // +0x04
    CRetailPointerVector PeopleInConversation; // +0x08
    bool IsValid() const;
};
FABLE_STATIC_ASSERT(offsetof(CScriptConversation, PeopleInConversation) == offsetof(CScriptConversationRetail, PeopleInConversation));

bool CScriptConversationRetail::IsValid() const
{
    if (PeopleInConversation.empty())
        return false;
    for (unsigned long i = 0; i < PeopleInConversation.size(); ++i) {
        if (!IS_THING_ALIVE(PeopleInConversation[i]))
            return false;
    }
    return true;
}
