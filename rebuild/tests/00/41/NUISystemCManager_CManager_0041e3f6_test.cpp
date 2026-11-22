// Behaviour test for 0x0041E3F6 NUISystem::CManager::CManager (protected) on engine/NUISystem_CManager.h. Links against the row
// object (verify_and_land's second link), so the constructor under test is the row's own ??0CManager@NUISystem@@IAE@XZ; a derived test
// class constructs it in a 0x5A-filled buffer and reads the protected members. Stubbed here: the CObservable constructor 0x0042BE7B, the 45
// vtable slots (SetMetaLayer records its layer), SetInput, SetGraphicsBank, SetIsUsingRelativeCoords, the CGraphicDataBankInit
// constructor, the six CDisplayManager texture-format pickers (engine/CDisplayManager.h), GFGetSystemManager, the bank manager's
// OpenGraphicBank (CGraphicBankManager has no shared header) and CCharString(const char*, long) / ~CCharString.
// Cases: no front-end component -> GBANK_MAIN; an active one -> GBANK_FRONT_END; an inactive one -> GBANK_MAIN; in each, the call order,
// the scalar member initialisers, the ten cleared inventory pointers, the pickers' bit depths and target formats, the bank parameters.
#include <stdio.h>
#include <string.h>
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY
#define FABLE_RETAIL_CMANAGER_WORLDINIT
#define FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK
#include "engine/NUISystem_CObservable.h"
class CGraphicDataBank;
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                  // the by-value CDefPointer returns of two stubbed slots
#define FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY
#include "engine/CGraphicDataBankInit.h"
#define FABLE_RETAIL_CDISPLAYMANAGER_ALLTEXTUREFORMATS
#include "engine/CDisplayManager.h"
class CGraphicBankManager { public:             // no shared header: the declaration the stub defines
    CCountedPointer<CGraphicDataBank> OpenGraphicBank(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);
};
class CSystemManager; class CNewFrontendGameComponent;
CSystemManager* __fastcall GFGetSystemManager();
void __fastcall SetIsUsingRelativeCoords(bool relative);

static char g_log[64]; static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }
static long g_metaLayer = 99, g_input = 99; static bool g_relative = false;
static char g_name[32]; static unsigned long g_bits[6]; static long g_fmtOff[6]; static int g_picks = 0; static char* g_fmtAddr[6];
static const CGraphicDataBankInit* g_init = 0; static CGraphicDataBankInit g_initCopy;
static bool g_bankFlags = false;
static int g_bankObject; static NUISystem::CManager* g_setThis = 0; static void* g_setData = 0;
static unsigned char g_systemBytes[0x100]; static unsigned char g_frontEndBytes[0x10]; static CGraphicBankManager g_bankManager;
CDisplayManager* GPDisplayManager;
CNewFrontendGameComponent* g_pNewFrontendGameComponent;

NUISystem::CObservable::CObservable() : m_pExclusiveObserver(0) { Log('O'); }
void NUISystem::CManager::SetInput(EInputType input) { Log('K'); g_input = input; }
void NUISystem::CManager::SetGraphicsBank(CCountedPointer<CGraphicDataBank> graphics_bank)
{ Log('G'); g_setThis = this; g_setData = graphics_bank.Data; }
void __fastcall SetIsUsingRelativeCoords(bool relative) { Log('R'); g_relative = relative; }
CGraphicDataBankInit::CGraphicDataBankInit() { Log('P'); memset(this, 0x77, sizeof(*this)); }
static bool Pick(unsigned long bits, CPixelFormat& format) { Log('x'); if (g_picks < 6) { g_bits[g_picks] = bits; g_fmtAddr[g_picks] = (char*)&format; } ++g_picks; return true; }
bool CDisplayManager::GetBestCompressedOpaqueTextureFormat(unsigned long b, CPixelFormat& f) { return Pick(b, f); }
bool CDisplayManager::GetBestCompressedAlphaTextureFormat(unsigned long b, CPixelFormat& f) { return Pick(b, f); }
bool CDisplayManager::GetBestCompressedBooleanAlphaTextureFormat(unsigned long b, CPixelFormat& f) { return Pick(b, f); }
bool CDisplayManager::GetBestAlphaTextureFormat(unsigned long b, CPixelFormat& f) { return Pick(b, f); }
bool CDisplayManager::GetBestOpaqueTextureFormat(unsigned long b, CPixelFormat& f, bool allow_alpha) { if (allow_alpha) Log('!'); return Pick(b, f); }
bool CDisplayManager::GetBestSignedTextureFormat(unsigned long b, CPixelFormat& f) { return Pick(b, f); }
CSystemManager* __fastcall GFGetSystemManager() { Log('S'); return reinterpret_cast<CSystemManager*>(g_systemBytes); }
CCountedPointer<CGraphicDataBank> CGraphicBankManager::OpenGraphicBank(const CCharString&, const CGraphicDataBankInit& init, bool a, bool b)
{
    Log('B'); g_init = &init; g_initCopy = init; g_bankFlags = a && b;
    for (int i = 0; i < 6 && i < g_picks; ++i) g_fmtOff[i] = (long)(g_fmtAddr[i] - (const char*)&init);
    CCountedPointer<CGraphicDataBank> bank; bank.Data = reinterpret_cast<CGraphicDataBank*>(&g_bankObject); return bank;
}
CCharString::CCharString(const char* text, long length) { Log('N'); strncpy(g_name, text, 31); if (length != -1) Log('?'); }
CCharString::~CCharString() { Log('n'); }
void NUISystem::CObservable::SetEvent(NUISystem::EEvent) { }
void NUISystem::CObservable::SetOwnedEvent(NUISystem::CObserver *, NUISystem::EEvent) { }
void NUISystem::CObservable::AddObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::AddExclusiveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::RemoveExclusiveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::RemoveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::ClearObservers() { }
NUISystem::CObserver * NUISystem::CObservable::GetExclusiveObserver() { return 0; }
bool NUISystem::CObservable::IsExclusivelyObserved() { return false; }
void NUISystem::CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver *) { }
void NUISystem::CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver *) { }
bool NUISystem::CManager::GetMouseDisabled() { return false; }
void NUISystem::CManager::SetKeyPressed(unsigned short) { }
unsigned short NUISystem::CManager::GetKeyPressed() { return 0; }
void NUISystem::CManager::Action(CCountedPointer<NUISystem::CAction> &) { }
void NUISystem::CManager::SetWeaponsInventory(CTCInventoryWeapons *) { }
void NUISystem::CManager::SetMapInventory(CTCInventoryMap *) { }
void NUISystem::CManager::SetQuestInventory(CTCInventoryQuests *) { }
void NUISystem::CManager::SetTCHeroExperienceInventory(CTCHeroExperience *) { }
void NUISystem::CManager::SetAbilitiesInventory(CTCInventoryAbilities *) { }
void NUISystem::CManager::SetExperienceInventory(CTCInventoryExperience *) { }
void NUISystem::CManager::SetInventory(CTCInventory *) { }
void NUISystem::CManager::SetInventoryClothing(CTCInventoryClothing *) { }
void NUISystem::CManager::SetInventoryTrade(CTCInventoryTrade *) { }
void NUISystem::CManager::AddThingActionParam(long) { }
void NUISystem::CManager::RemoveThingActionParam(long) { }
void NUISystem::CManager::SetPlayerThing(CThing *) { }
void NUISystem::CManager::AddUIActionParam(NUISystem::CActionParam *) { }
void NUISystem::CManager::RemoveUIActionParam(NUISystem::CActionParam *) { }
void NUISystem::CManager::ClearActionParams() { }
void NUISystem::CManager::SetNumberAugmentations(unsigned long) { }
unsigned long NUISystem::CManager::GetNumberAugmentations() const { return 0; }
void NUISystem::CManager::SetChange(NUISystem::CManager::EChange) { }
NUISystem::CManager::EChange NUISystem::CManager::GetChange() { return (NUISystem::CManager::EChange)0; }
void NUISystem::CManager::SetMetaLayer(long layer) { Log('L'); g_metaLayer = layer; }
long NUISystem::CManager::GetMetaLayer() { return 0; }
EEngine2DLayer NUISystem::CManager::GetLayer(long) { return (EEngine2DLayer)0; }
void NUISystem::CManager::ClearBastardChild() { }
void NUISystem::CManager::SetRightStickForce(C2DVector &) { }
C2DVector & NUISystem::CManager::GetRightStickForce() { static C2DVector s; return s; }
void NUISystem::CManager::PlaySoundW(CCharString &, long) { }
unsigned long NUISystem::CManager::PlaySample(CCharString &, long) { return 0; }
CDefPointer<NUISystem::CUIMiscThingsDef> NUISystem::CManager::GetMiscThingsDef() { return CDefPointer<NUISystem::CUIMiscThingsDef>(); }
CDefPointer<NUISystem::CUIIconsDef> NUISystem::CManager::GetIconsDef() { return CDefPointer<NUISystem::CUIIconsDef>(); }
void NUISystem::CManager::AddActionsListenIgnoreOnList(NUISystem::CComponent *, NUISystem::CComponent *, NUISystem::CComponent *, NUISystem::EEvent) { }

class TestManager : public NUISystem::CManager {
public:
    bool Scalars() const
    {
        return m_pDefinitionManager == 0 && !m_bHasErrorMessage && m_pErrorMessage == 0 && !Restoring && m_fLastRenderTime == 0.0 &&
            PMouseCursor == 0 && PDropped == 0 && DescriptionScrollingOffset == -1.0f && !DescriptionScrollingToMax && !MouseDisabled &&
            !AssignmentItemsExpressionsSwapping && !AssignmentSpellsSwapping;
    }
    bool InventoriesCleared() const
    {
        return PTCHeroExperience == 0 && PAbilitiesInventory == 0 && PWeaponsInventory == 0 && PMapInventory == 0 && PQuestInventory == 0 &&
            PExperienceInventory == 0 && PInventory == 0 && PInventoryClothing == 0 && PInventoryTrade == 0 && PPlayer == 0;
    }
    bool BanksEmpty() const { return PMeshBank.Data == 0 && PMeshBank.Info == 0 && BastardChild.Data == 0 && BastardChild.Info == 0; }
};

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("0041e3f6 FAIL %s (log %s)\n", what, g_log); g_fail = 1; } }
static void RunCase(const char* expectBank, const char* label)
{
    static unsigned char storage[sizeof(TestManager) + 16];
    memset(storage, 0x5a, sizeof(storage));
    g_n = 0; g_picks = 0; g_name[0] = 0; g_metaLayer = 99; g_input = 99; g_relative = false; g_setThis = 0; g_setData = 0; g_bankFlags = false;
    TestManager* m = new (storage) TestManager;
    Check(strcmp(g_log, "ORKPxxxxxxSNBGnL") == 0, label);
    Check(strcmp(g_name, expectBank) == 0, "bank name");
    Check(m->Scalars() && m->InventoriesCleared() && m->BanksEmpty(), "members initialised");
    Check(g_relative && g_input == NUISystem::CManager::UI_INPUT_KEYBOARD && g_metaLayer == 0, "relative coords, keyboard input, meta layer 0");
    Check(g_setThis == m && g_setData == &g_bankObject && g_bankFlags, "the opened bank is set on this manager");
    const unsigned long bits[6] = { 0x10, 0x10, 0x10, 0x20, 0x20, 0x10 };
    const long offs[6] = {
        (long)((char*)&g_initCopy.NonAlphaFormat - (char*)&g_initCopy), (long)((char*)&g_initCopy.AlphaFormat - (char*)&g_initCopy),
        (long)((char*)&g_initCopy.BooleanAlphaFormat - (char*)&g_initCopy), (long)((char*)&g_initCopy.UncompressedAlphaFormat - (char*)&g_initCopy),
        (long)((char*)&g_initCopy.UncompressedNonAlphaFormat - (char*)&g_initCopy), (long)((char*)&g_initCopy.SignedFormat - (char*)&g_initCopy) };
    for (int i = 0; i < 6; ++i) Check(g_bits[i] == bits[i] && g_fmtOff[i] == offs[i], "picker bit depth / target format");
    Check(g_initCopy.MaxGraphicWidth == 0xffffffffUL && g_initCopy.MaxGraphicHeight == 0xffffffffUL && !g_initCopy.GenerateMipmaps &&
          !g_initCopy.AllowDither, "bank parameters");
}
int main()
{
    *reinterpret_cast<CGraphicBankManager**>(g_systemBytes + 0x6c) = &g_bankManager;
    g_pNewFrontendGameComponent = 0;
    RunCase("GBANK_MAIN", "no front end: order");
    g_frontEndBytes[9] = 1;
    g_pNewFrontendGameComponent = reinterpret_cast<CNewFrontendGameComponent*>(g_frontEndBytes);
    RunCase("GBANK_FRONT_END", "active front end: order");
    g_frontEndBytes[9] = 0;
    RunCase("GBANK_MAIN", "inactive front end: order");
    if (g_fail) return 1;
    printf("0041e3f6_TEST PASS\n");
    return 0;
}
