// CEngineSurfaceCompositionManager::FindIndexInCompletedBank(const CEngineTextureMorph*) -- look the morph up in the completed-surface bank by its "<from>-<to>" name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AE60, length 128 bytes.
// Callees: GFIntToCharString(long) @ 0x0099F830 (twice), operator+(const CCharString&, const char*) @ 0x0099F600,
// operator+(const CCharString&, const CCharString&) @ 0x0099F570 (all __fastcall: hidden result ecx, left edx, right on the
// stack), ~CCharString @ 0x0099EAE0 (four times), the bank's index lookup by name @ 0x009CCDC0 (on +0x398; catalogued as
// a propagated "CQuestManager::OnQuestStartScreenDisplayed" label). String literal "-" @ 0x0122E630.
// Layout: the completed bank sits at +0x398 in retail; engine/CEngineSurfaceCompositionManager.h puts CompletedBankFile
// at +0x41c (PDB containers are wider), so `this` is modelled locally. The morph's two texture ids are its first words.
// Shape: both ids are read into locals before the first conversion (the first one survives the call in edi); the chained `+` evaluates right to left (the second id is converted first); the three temporaries die in
// construction order before the lookup; the name itself is homed in the dead `morph` argument slot.
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

class CEngineTextureMorph { public:
    long m_fromTexture;                            // +0x00
    long m_toTexture;                              // +0x04
};

class CBankFile
{
public:
    unsigned long FindIndexBySymbol(const CCharString&) const;   // 0x009CCDC0
};
class CBankFileAsync : public CBankFile
{
public:
};

class CGraphicDataBank : public CBankFileAsync { public:
};

class CEngineSurfaceCompositionManager { public:
    unsigned char    _pad[0x398];
    CGraphicDataBank m_completedBank;              // +0x398
    unsigned long FindIndexInCompletedBank(const CEngineTextureMorph* morph);
};

unsigned long CEngineSurfaceCompositionManager::FindIndexInCompletedBank(const CEngineTextureMorph* morph)
{
    long from = morph->m_fromTexture;
    long to = morph->m_toTexture;
    CCharString name = GFIntToCharString(from) + "-" + GFIntToCharString(to);
    return m_completedBank.FindIndexBySymbol(name);
}