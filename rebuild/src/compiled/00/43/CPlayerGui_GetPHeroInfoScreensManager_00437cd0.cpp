// CPlayerGui::GetPHeroInfoScreensManager() const -- the hero information screens manager CPlayerGui owns (its scoped_ptr word at +0x32C)
// Leaf: ?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@NHeroInformationScreens@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437CD0, 7 bytes: `mov eax,[ecx+32Ch] ; ret`.
// Identity (two sources): Ego_r.pdb + FableWin.pdb ?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@NHeroInformationScreens@@XZ,
//   declared by engine/CPlayerGui.h for 0x00437CD0; PHeroInfoScreensManager (boost::scoped_ptr<NHeroInformationScreens::CManager>) is the
//   Ego_r member at retail +0x32C (header MEMBERS branch); NF76's landed 0x005778CA binds this spelling.
// NF79 (round 7, q10960): re-authored on the shared header (macro block of the landed CPlayerGui::SetMoneyAsDisplaying 0x004376B0); was a
//   stand-in ?Get@T@@QAEHXZ over a local struct.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#include "engine/CPlayerGui.h"

NHeroInformationScreens::CManager* CPlayerGui::GetPHeroInfoScreensManager() const
{
    return PHeroInfoScreensManager.get();
}
