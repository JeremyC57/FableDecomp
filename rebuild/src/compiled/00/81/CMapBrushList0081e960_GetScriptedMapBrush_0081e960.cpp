// CMapBrushList_0081e960::GetScriptedMapBrush(const CCharString& name) -- walk the list at +0xc for the brush whose name (the accessor @ 0x0082E250 on the node's +8: `lea eax,[ecx+48h]`, the block's CCharString) equals the given name (the inlined CCharString ==: same data block, or both present with equal lengths and Compare @ 0x00411570 == 0); return the node's brush (+8), rethrowing (`throw;` = _CxxThrowException @ 0x00BFEB84 with (0, 0)) when none matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081E960, length 90 bytes.
// Callees: the mode block's name accessor @ 0x0082E250 (manifest "CModeBlock::GetAnimFilter"; no arguments), Compare(const char*, const char*) @ 0x00411570 (`__fastcall`), _CxxThrowException @ 0x00BFEB84. Shape: the sentinel walk (`mov esi,[eax] ; cmp esi,eax` then `cmp esi,[ebx+0Ch]`), `neg eax ; sbb al,al ; inc al` for `Compare == 0`, the throw at the end of the loop, `lea eax,[esi+8]`, `ret 4`. Two byte-identical copies (0x0081E960 / 0x0081E9F0).
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
template <class T> class CBasicString { public:                       // Ego_r.pdb: the string base of CCharString (data and inline comparisons)
    CCharStringData* m_pData;
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 (Ego_r.pdb ?Compare@?$CBasicString@D@@KIJPBD0@Z, protected static)
public:
    bool operator==(const CBasicString& other) const
    {
        CCharStringData* a = m_pData;
        CCharStringData* b = other.m_pData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
class CCharString : public CBasicString<char> { public:
};
struct CModeBlock {
    CCharString& GetName();                        // 0x0082E250 (`lea eax,[ecx+48h]`)
};
struct SBrushNode {                                // a list node: next, prev, the brush's mode block
    SBrushNode* m_pNext;
    SBrushNode* m_pPrev;
    CModeBlock  m_block;                           // +0x08
};
struct CMapBrushList_0081e960 {
    unsigned char _pad[0xc];
    SBrushNode*   m_pHead;                         // +0x0c (the sentinel)
    CModeBlock* GetScriptedMapBrush(const CCharString& name);
};

CModeBlock* CMapBrushList_0081e960::GetScriptedMapBrush(const CCharString& name)
{
    SBrushNode* node = m_pHead->m_pNext;
    for (; node != m_pHead; node = node->m_pNext) {
        if (node->m_block.GetName() == name)
            return &node->m_block;
    }
    throw;                                         // _CxxThrowException(0, 0) @ 0x00BFEB84
    return &node->m_block;
}