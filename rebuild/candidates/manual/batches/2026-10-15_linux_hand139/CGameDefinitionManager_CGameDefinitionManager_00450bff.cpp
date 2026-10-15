// CGameDefinitionManager::~CGameDefinitionManager() -- release the four cached def pointers (+0xdc, +0xd8, +0xd4, +0xd0, in reverse declaration order), then run the CDefinitionManager base destructor (tail jump)
// Leaf: ??1CGameDefinitionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00450BFF, length 115 bytes (a size-optimised TU: unaligned start, `and dword ptr [m],0`).
// Callees: each def's virtual slot 1 (Destroy) when its count reaches zero, CDefinitionManager::~CDefinitionManager @ 0x009B02E0
// (catalog "CDefinitionManager::Cleanup").
// Layout: Ego_r.pdb has no CGameDefinitionManager members; the retail object holds four CDefPointer<...> words at +0xd0..+0xdc past
// the CDefinitionManager base, so they are named descriptively.
// Shape: no vtable store (the classes are modelled without virtuals); each member is a CDefPointer whose `__forceinline` destructor
// releases through CDefBase::Release (`dec [ecx+4] ; jne ; call [eax+4]`) and nulls the field (`and [edi],0`, kept in the size TU);
// pragma optimize("s", on).
#pragma optimize("s", on)

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};
struct CDefBase : CDefVirtual {
    long m_count;                          // +4
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CDefPointer {
    CDefBase* m_p;
    __forceinline ~CDefPointer()           // forced: the size TU would call it out of line
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};

struct CDefinitionManager {
    unsigned char _body[0xd0];
    ~CDefinitionManager();                 // 0x009B02E0
};

struct CGameDefinitionManager : CDefinitionManager {
    CDefPointer m_pDef0;                   // +0xd0
    CDefPointer m_pDef1;                   // +0xd4
    CDefPointer m_pDef2;                   // +0xd8
    CDefPointer m_pDef3;                   // +0xdc
    ~CGameDefinitionManager();
};

CGameDefinitionManager::~CGameDefinitionManager()
{
}
