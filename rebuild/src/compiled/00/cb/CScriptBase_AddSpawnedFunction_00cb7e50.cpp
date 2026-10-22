// CScriptBase::AddSpawnedFunction(CGuiVar* var, const CCharString& name) -- name the GUI variable and append a counted pointer to it to the struct's variable list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7E50, length 135 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0, operator new @ 0x00BFEA1A (the 12-byte count block),
// malloc @ 0x00BFEA0E (the 16-byte list node), operator delete @ 0x00BFE9BC; the block's destroy function is
// DeleteData @ 0x00CB8D10 (`__fastcall`, `if (p) p->Destroy(1)`).
// Shape: the prev link read into a local before the stores (else the lane re-reads head->prev). The counted pointer is {object, block}; its block is {count, destroy function, object} built only
// for a non-null object (`new`'s own null test), the list push_back is inlined (the head read before the node
// malloc, the value placement-copied with its null test and add-ref, then linked before the head), and the
// temporary's release calls the destroy function with the block's object in ecx before deleting the block.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);       // 0x00BFEA1A
void __cdecl operator delete(void* p);               // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CGuiVar {
    unsigned char _pad[0x24];
    CCharString   m_name;                          // +0x24
};
void __fastcall DeleteData(CGuiVar* var);          // 0x00CB8D10

template <class T> struct CCountedPointer {
    struct SBlock {
        long m_count;
        void (__fastcall *m_destroy)(T*);
        T*   m_object;
        SBlock(T* object) : m_count(1), m_destroy(DeleteData), m_object(object) {}
    };
    T*      m_object;
    SBlock* m_block;
    CCountedPointer(T* object) : m_object(object) { m_block = object ? new SBlock(object) : 0; }
    CCountedPointer(const CCountedPointer& other) : m_object(other.m_object), m_block(other.m_block)
    {
        if (m_block)
            ++m_block->m_count;
    }
    ~CCountedPointer()
    {
        if (m_block && --m_block->m_count == 0) {
            m_block->m_destroy(m_block->m_object);
            delete m_block;
        }
    }
};

template <class T> struct CRetailList {            // VC6-era list: the sentinel pointer only
    struct SNode { SNode* m_next; SNode* m_prev; T m_value; };
    SNode* m_head;
    void push_back(const T& value)
    {
        SNode* head = m_head;
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->m_value) T(value);
        SNode* prev = head->m_prev;
        node->m_next = head;
        node->m_prev = prev;
        prev->m_next = node;
        head->m_prev = node;
    }
};

struct CGuiVarTransferStruct {
    unsigned char                          _pad00[4];
    CRetailList<CCountedPointer<CGuiVar> > m_vars;   // +0x04
    void Add(CGuiVar* var, const CCharString& name);
};

void CGuiVarTransferStruct::Add(CGuiVar* var, const CCharString& name)
{
    var->m_name = name;
    m_vars.push_back(CCountedPointer<CGuiVar>(var));
}