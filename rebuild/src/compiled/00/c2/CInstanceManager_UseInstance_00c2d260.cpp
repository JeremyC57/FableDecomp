// CInstanceManager::UseInstance(unsigned int index, SThemeRequest* request) -- start the request on instance[index]: hand the instance the manager's +0x31C word, the request's name string, its three play words, the manager and the request's range words
// Leaf: ?UseInstance@CInstanceManager@@QAEXIPAUSThemeRequest@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2D260, 66 bytes, thiscall, `ret 8`.
// Identity: libpk's CInstanceManager (the file string `.\CInstanceManager.cpp` of the landed Assign 0x00C2D5E0); its two
//   callers are the landed Update 0x00C2D490 (binds this spelling) and Assign 0x00C2D5E0. No PDB covers libpk: names are
//   descriptive. Label correction: the manifest row "CEditTransactionSetEngineTheme::Redo" is propagated, and the old
//   "stack-imbalanced" park misread the shape: the six request pushes are the first arguments of the 8-argument
//   (`ret 20h`) instance call, and the name-string getter between them takes no stack arguments.
// Callees: CName::GetString 0x00C16890 (a `jmp` to 0x00C16800: null or the string buffer), CInstance::Setup 0x00C38CD0
//   (eight dwords, `ret 20h`).
// Layout: the landed Update model: two 0x180-byte CInstance records at +0x18, the word +0x31C; SThemeRequest 0x1C bytes
//   (the landed Assign model: +0 word, +4 CName, +0xC/+0x10/+0x14 play words, +0x18 word).
// Shape: right-to-left arguments; the getter call is the second argument, evaluated after the six pushed words.
class CInstanceManager;

struct CName {                                  // 8 bytes with its own assignment (landed Assign 0x00C2D5E0)
    unsigned char _body[8];
    const char* GetString();                    // 0x00C16890
};
struct SThemeRequest {                          // 0x1c bytes
    long  m_0;                                  // +0x00
    CName m_name;                               // +0x04
    long  m_c;                                  // +0x0c
    long  m_10;                                 // +0x10
    long  m_14;                                 // +0x14
    long  m_18;                                 // +0x18
};
class CInstance {                               // 0x180 bytes: one playing instance
public:
    void Setup(long owner0, const char* name, long a, long b, long c, CInstanceManager* owner, long rangeLo, long rangeHi);   // 0x00C38CD0
    unsigned char _body[0x180];
};
class CInstanceManager {                        // libpk, .\CInstanceManager.cpp
public:
    void UseInstance(unsigned int index, SThemeRequest* request);
    unsigned char _pad_0x000[0x0c];
    long          m_updateA;                    // +0x0c
    long          m_updateB;                    // +0x10
    long          m_updateC;                    // +0x14
    CInstance     m_instances[2];               // +0x18
    long          m_current;                    // +0x318 index of the current instance
    long          m_owner;                      // +0x31c
};

void CInstanceManager::UseInstance(unsigned int index, SThemeRequest* request)
{
    m_instances[index].Setup(m_owner, request->m_name.GetString(), request->m_c, request->m_10, request->m_14,
                             this, request->m_18, request->m_0);
}