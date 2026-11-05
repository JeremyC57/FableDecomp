// CBankFileManager::Close() -- drop every retail bank (popping the counted pointers off the back of the list) and clear the bank map
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7990, 135 bytes (+9 int3), __fastcall, no arguments.
// Callees: the count block's destroy function (`call [blk+4]`, object in ecx: __fastcall), operator delete
// 0x00BFE9BC (count block), free 0x00BFEA14 (list node), the Banks tree's recursive _Erase 0x009AA7C0.
// Layout: Ego_r.pdb CBankFileManager with retail containers -- Banks, a VC6-era map {head, size} plus its
// comparator word, at +4 (head +4, size +8); RetailBanks, the one-word sentinel list of
// CCountedPointer<NBankFileManager::CRetailBank> (node: next, prev, object +8, count block +0x0C), at +0x10.
// Shape: `while (!empty()) pop_back();` with the list head re-read every pass: the last node is unlinked
// (next/prev read first), its counted pointer destroyed inline (release re-reading the block, both words
// cleared) and the node freed; then the inline map clear(): a non-empty tree is erased from the root and
// the head's left/parent/right and the size reset (the head re-read for each store).
#include <stdlib.h>

void operator delete(void* p);                      // 0x00BFE9BC

#include "engine/CCountedPointer.h"


template <class T>
struct CRetailList {                                // the sentinel node pointer only
    struct SNode {
        SNode* m_pNext;
        SNode* m_pPrev;
        T m_value;
    };
    SNode* m_pNode;
    bool empty() const { return m_pNode->m_pNext == m_pNode; }
    void pop_back()
    {
        SNode* node = m_pNode->m_pPrev;
        SNode* next = node->m_pNext;
        SNode* prev = node->m_pPrev;
        prev->m_pNext = next;
        next->m_pPrev = prev;
        node->m_value.~T();
        free(node);
    }
};

struct SBankNode {                                  // VC6-era tree node
    unsigned char m_colour;                         // +0x00
    SBankNode* m_pParent;                           // +0x04
    SBankNode* m_pLeft;                             // +0x08
    SBankNode* m_pRight;                            // +0x0C
};

struct CBankTree {                                  // map<CCharString, CBankInfo>: {head, size}
    SBankNode* m_pHead;                             // +0
    unsigned long m_size;                           // +4
    void _Erase(SBankNode* root);                   // 0x009AA7C0
    void clear()
    {
        if (m_size) {
            _Erase(m_pHead->m_pParent);
            m_pHead->m_pLeft = m_pHead;
            m_pHead->m_pParent = 0;
            m_pHead->m_pRight = m_pHead;
            m_size = 0;
        }
    }
};

namespace NBankFileManager { class CRetailBank; }

struct CBankFileManager {
    void* m_00;                                     // +0x00
    CBankTree Banks;                                // +0x04
    unsigned long m_bankCompare;                    // +0x0C (map comparator word)
    CRetailList<CCountedPointer<NBankFileManager::CRetailBank> > RetailBanks;   // +0x10
    void Close();
};

void CBankFileManager::Close()
{
    while (!RetailBanks.empty())
        RetailBanks.pop_back();
    Banks.clear();
}