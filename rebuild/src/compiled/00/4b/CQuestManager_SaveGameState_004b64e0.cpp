// CQuestManager::SaveGameState(CPersistContext& context) -- build the quest game-state string and transfer it through the context (written as length + bytes when saving), then transfer the boasts
// Grade: RELOCATION_MATCH -- the mode test is a named bool (`bool writing = context.Mode == 2 || context.Mode == 3; if (writing) ... else ...`): that lays the writing arm first (`cmp 2 ; je W ; cmp 3 ; jne E`) and lets the compiler duplicate the WriteBuffer call per string arm; `if (a || b)` directly lays the Transfer arm first and merges the arms
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B64E0, length 142 bytes (`ret 4`).
// Callees: CCharString::CCharString() @ 0x0099E4B0, CQuestManager::SaveGameState(CCharString&) @ 0x004B5EE0,
// CDataOutputStream::WriteULONG @ 0x00993EA0, CDataOutputStream::WriteBuffer(const void*, long) @ 0x00996610 (catalog
// "PersistData_CompressAndVerify", as in the landed CContainmentVolume::SaveBinary 0x00A75400), CPersistContext::
// Transfer(CCharString&) @ 0x009BAB50 (the out-of-line reading path; catalog label propagated),
// CQuestManager::TransferBoasts(CPersistContext&) @ 0x004B05C0, CCharString::~CCharString @ 0x0099EAE0.
// Layout: CPersistContext mode +0x18 (2 and 3 are the writing modes), output stream +0x28; CCharString's block
// {char* data, long length}; the empty string literal is 0x0129AAF4.
// Shape: the string is a named local; the writing path inlines the string writer (GetLength() then
// WriteBuffer(GetPString(), GetLength()): the two null-tested accessors fold into one branch with `""`/0).
// Leaf: ?SaveGameState@CQuestManager@@QAEXAAVCPersistContext@@@Z
// Naming (NF129, 2026-11-20, q4213): 0x009BAB50 is CPersistContext::WriteString(const CCharString&) (Ego_r.pdb 0x00B0F550 + FableWin.pdb
//   0x02FEA160 ?WriteString@CPersistContext@@QAEXABVCCharString@@@Z; engine/CPersistContext.h); respelled in place in the local
//   CPersistContext model (was the stand-in Transfer_009bab50).
class CCharStringData { public:
    const char*   m_pData;
    long          m_length;
};

extern char g_EmptyCharString_0129AAF4[1];        // 0x0129AAF4

class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
    // null case written first (the landed CDataOutputStream::WriteNullTerminatedString 0x00995AE0 accessors)
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    const char* GetPString() const { return PStringData == 0 ? g_EmptyCharString_0129AAF4 : PStringData->m_pData; }
};

class CDataOutputStream { public:
    void WriteCompressed(const void* data, long size); // 0x00996610; NF299: PDB name, was WriteBuffer
public:
    void WriteULONG(unsigned long);   // 0x00993EA0
};

class CPersistContext { public:
    unsigned char      _pad00[0x18];
    long               Mode;                       // +0x18
    unsigned char      _pad1c[0xc];
    CDataOutputStream* Stream;                     // +0x28
    void WriteString(const CCharString& string);// 0x009BAB50
};

class CQuestManager { public:
    void SaveGameState(CCharString& state);        // 0x004B5EE0
    void TransferBoasts(CPersistContext& context); // 0x004B05C0
    void SaveGameState(CPersistContext& context);
};

void CQuestManager::SaveGameState(CPersistContext& context)
{
    CCharString state;
    SaveGameState(state);
    bool writing = context.Mode == 2 || context.Mode == 3;
    if (writing) {
        context.Stream->WriteULONG(state.GetLength());
        context.Stream->WriteCompressed(state.GetPString(), state.GetLength());
    } else
        context.WriteString(state);
    TransferBoasts(context);
}