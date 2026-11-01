// CThing::GetDefName() -- the name of the thing's def, looked up by its global def index in the definition manager
// Naming wave 18: the global stand-in g_pThingManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7CC0, 34 bytes. Callees: CDefinitionManager::GetDefNameFromGlobalIndex(long)
// @ 0x009ACCC0 (returns CDefString by hidden pointer), reached through the thing manager @ 0x013B8A1C (+0x20).
// Shape: DefGlobalIndex is the unsigned short at retail +0x8c (PDB +0x9c, `movzx`); the caller's hidden result
// pointer is passed straight on (no copy) and returned in eax. CDefString has user constructors, so it returns
// through the hidden pointer.
class CDefString { public:
    void* m_pData;
    CDefString(const CDefString& other);
    ~CDefString();
};

class CDefinitionManager { public:
public:
    CDefString GetDefNameFromGlobalIndex(long) const;   // 0x009ACCC0
};

class CThingManager { public:
    unsigned char       _pad00[0x20];
    CDefinitionManager* m_pDefinitionManager;        // +0x20
};


class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
private: public:
    void*          __vftable;                        // +0x00
    unsigned char  _pad04[0x8c - 0x04];
    unsigned short DefGlobalIndex;                   // +0x8c (PDB +0x9c)
    CDefString GetDefName() const;
};

CDefString CThing::GetDefName() const
{
    return CThing::GetPCurrentThingManager()->m_pDefinitionManager->GetDefNameFromGlobalIndex(DefGlobalIndex);
}