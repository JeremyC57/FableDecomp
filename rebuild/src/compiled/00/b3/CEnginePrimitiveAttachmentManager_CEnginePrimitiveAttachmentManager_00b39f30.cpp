// CEnginePrimitiveAttachmentManager::CEnginePrimitiveAttachmentManager() -- the two base constructors (0x0099A2F0 on this, 0x00B59710 CEngineComponent at +4), both vtables, then the AttachedPrimitives list built with its malloc'd sentinel node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B39F30, length 61 bytes (64 with padding).
// Leaf: ??0CEnginePrimitiveAttachmentManager
// The chunk label (CSpecialAbilitiesSummonSpellDef) is propagated: the vtables stored here, 0x012A1AB0 at +0
// and 0x012A1A78 at +4, are CEnginePrimitiveAttachmentManager's (rtti-vtables.tsv).
// Callees: 0x0099A2F0 (the primary base constructor, a vptr-only root), 0x00B59710 (CEngineComponent: vptr,
// a dword, two flags; 12 bytes), 0x00BFEA0E (malloc).
// The PDB (Ego_r, 28 bytes) puts AttachedPrimitives, a list<CEngineInternalAttachedPrimitiveInfo>, at +0x10;
// the retail list is the 4-byte STLport one (a single sentinel pointer, cf. GOTCHAS 24h), so the class is
// 0x14 bytes. Shape: both base constructors, the two vtable stores, then the inlined STLport `_List_base`
// constructor: the sentinel pointer cleared (`mov dword ptr [esi+10h],0`), a 0x14-byte node malloc'd
// (next, prev, 12-byte value), its next/prev pointed at itself and only then published -- the node is
// built in a local and the member assigned last.
#include <stdlib.h>

struct CBaseClassNonCopyable {
    virtual void v00();
protected:
    CBaseClassNonCopyable();                     // 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (protected)
public:
};

class CEngineComponent { public:                       // vtable 0x012A2544
    virtual void v00();
    long GlobalPointerAddress;                                   // +0x04
    bool m_enabled;                             // +0x08
    bool KeepAlive;                                   // +0x09
    CEngineComponent();                         // 0x00B59710
};

class CEngineInternalAttachedPrimitiveInfo { public:
    long m_words[3];
};

template <class T>
struct CRetailSTLportList {                     // STLport list: the sentinel node pointer only
    struct SNode {
        SNode* next;
        SNode* prev;
        T      value;
    };
    SNode* m_node;
    CRetailSTLportList()
    {
        m_node = 0;
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->next = node;
        node->prev = node;
        m_node = node;
    }
};

class CEnginePrimitiveAttachmentManager : public CBaseClassNonCopyable, public CEngineComponent { public:
    CRetailSTLportList<CEngineInternalAttachedPrimitiveInfo> AttachedPrimitives;   // +0x10
    virtual void v01();
    CEnginePrimitiveAttachmentManager();
};

CEnginePrimitiveAttachmentManager::CEnginePrimitiveAttachmentManager()
{
}