// CEngineWorldMap::CreateNewBankMap(const CCharString& name) -- add a new entry called `name` to the static-map bank file with a fresh CStaticMapBankStateBlock as its state block, and return the new entry's index
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3FF70, 142 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (8 bytes, then the 12-byte count block), the bank file's AddEntry(const CCharString&,
//   unsigned long, const CCharString&, CCountedPointer<CBankStateBlock>) @ 0x009CD620 (manifest "Map_InsertOrReplacePair" is a
//   propagated guess) and FindEntry(const CCharString&) @ 0x009CCDC0 (manifest "CQuestManager::OnQuestStartScreenDisplayed"
//   is propagated), operator delete @ 0x00BFE9BC, the state-block deleter @ 0x00B430A0 (`if (p) p->vdtor(1)`).
// Layout: the retail StaticMapFile (CStaticMapBankFile) sits at +0x34 -- the PDB's +0x40 less the 4-byte-narrower retail
//   CArray MapList and the 4-byte STLport list VisibleMapList; CStaticMapBankStateBlock (vtable 0x012A1F3C per
//   rtti-vtables.tsv) is 8 bytes: vptr + CStateBlock State (left uninitialised by its constructor).
// Shape: the counted pointer is a NAMED local built in registers (edi object, esi block); passing it by value copies it
//   into the argument area (`sub esp,8 ; mov eax,esp ; ... ; inc dword ptr [esi]`), the callee destroys its copy, and
//   the local is released after the FindEntry call (`dec dword ptr [esi] ; jne`).
class CCharString;

void __fastcall DeleteStaticMapBankStateBlock(void* block);   // 0x00B430A0

struct SCountBlock {
    long  m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);      // +4: per-type deleter, object in ecx
    void* m_pObject;                             // +8
    SCountBlock(void* object, void (__fastcall* destroy)(void*)) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template <class T>
class CCountedPointer { public:
    T*           m_pObject;                      // +0
    SCountBlock* m_pCount;                       // +4
    CCountedPointer(T* object, void (__fastcall* destroy)(void*)) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(object, destroy);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};

class CBankStateBlock { public:
    virtual ~CBankStateBlock();
};

class CStaticMapBankStateBlock : public CBankStateBlock { public:    // vtable 0x012A1F3C, 8 bytes
    long State;                                         // +0x04 CStateBlock
    CStaticMapBankStateBlock() {}
    virtual ~CStaticMapBankStateBlock();
};

class CStaticMapBankFile { public:
    void AddEntry(const CCharString& symbol, unsigned long type, const CCharString& filename,
                  CCountedPointer<CBankStateBlock> state);                 // 0x009CD620
    unsigned long FindEntry(const CCharString& symbol);                   // 0x009CCDC0
};

class CEngineWorldMap { public:
    unsigned char      _pad0[0x34];
    CStaticMapBankFile StaticMapFile;                                      // +0x34
    unsigned long CreateNewBankMap(const CCharString& name);
};

unsigned long CEngineWorldMap::CreateNewBankMap(const CCharString& name)
{
    CCountedPointer<CBankStateBlock> state(new CStaticMapBankStateBlock, DeleteStaticMapBankStateBlock);
    StaticMapFile.AddEntry(name, 0, name, state);
    return StaticMapFile.FindEntry(name);
}