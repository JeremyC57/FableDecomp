// CScriptThing::operator=(const CScriptThing& other) -- assign the other handle's implementation: release ours (destroy + free the count block at zero) unless both share one block, adopt theirs and add a reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB980, 74 bytes.
// Callees: the count block's destroy function (block +4, `__fastcall` with the object at block +8 in ecx),
// operator delete @ 0x00BFE9BC.
//
// Shape: the source's two words are read into ebx/edi BEFORE the block compare (object first, then the
// block -- written in load order), the destination block is re-read after the decrement and after the
// destroy call; the stores of both words are scheduled around the null test of the new block. This is
// CCountedPointer<CScriptThing>::operator= inlined into the handle's assignment (the vptr is untouched).
// Model: CScriptThing (PDB: 0xc bytes) = CBase (vptr; virtual destructor 0x0099A2E0) +
// CCountedPointer<CScriptThing> PImp {Data +4, Info +8}; the count block (CCPPointerInfo) holds the
// references at +0, the destroy function at +4 and the object it destroys at +8 (same block as the landed
// deleting destructor 0x004AB170). The engine header flattens the vptr and the counted pointer into plain
// words, so the class is modelled locally with its member names.
// Leaf: ??4CScriptThing

class CScriptThing;

class CCPPointerInfo { public:
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

struct CCountedPointer_CScriptThing {
    CScriptThing* Data;                      // +0
    CCPPointerInfo* Info;                    // +4
    CCountedPointer_CScriptThing& operator=(const CCountedPointer_CScriptThing& other)
    {
        CScriptThing* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};

struct CBase {
    virtual ~CBase();                        // 0x0099A2E0
};

class CScriptThing : public CBase { public:                // vtable 0x01238C8C
    CCountedPointer_CScriptThing PImp;       // +0x04
    virtual ~CScriptThing();
    CScriptThing& operator=(const CScriptThing& other);
};

CScriptThing& CScriptThing::operator=(const CScriptThing& other)
{
    PImp = other.PImp;
    return *this;
}