// CFrontendProfileList::Refresh() -- clear the name list and refill it from the user-profile manager's file list (the per-file info list is discarded)
// Leaf: ?Refresh@CFrontendProfileList@@QAEXXZ
// Retail: Fable.exe @ 0x00495830, 84 bytes, plain `ret`; one caller 0x0049602B (CFrontendGameComponent, `this` = its
// SaveNames list at +0x1e1c). Catalog row label CEngineLandscapeMap::RenderBackground is wrong; the class name follows
// the existing bake (CFrontendProfileList_Refresh) -- no PDB symbol (the Ego_r.pdb SaveNames is a plain
// vector<CWideString>), so the owning class is not confirmed.
// Body: Names.erase(begin, end) (0x00412450, vector<CWideString>::erase out of line); a local vector<CDateAndTime>
// (zeroed); CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0) ->GetSortedSaveNamesAndTimes(Names, times) (0x004091C0);
// the local's destructor frees its storage (free 0x00BFEA14: malloc allocator, trivially destructible elements).
// Layout: Names at +0 (12-byte STLport vector); file-info element 0x1c bytes (LoadFileList grows it by 0x1c), type not
// identified.
// Shape: /O2 (16-aligned entry).
// Name fix (agentNF54): the local STLport vector / CMallocVector / CUserProfileManager stand-ins replaced by the shared
//   stlport/fable_containers.h, fable_string.h, engine/CDateAndTime.h and engine/CUserProfileManager.h: 0x004091C0 is
//   ?GetSortedSaveNamesAndTimes@CUserProfileManager@@QAEX...@Z (Ego_r + FableWin; was the made-up LoadFileList over a 28-byte
//   CFileListInfo), whose second argument is the vector<CDateAndTime> (CDateAndTime is 28 bytes in the Ego_r layout), as the landed
//   CFrontEndManager::GetSavedGameNames 0x00597DF7 passes it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVELIST
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDateAndTime.h"
#include "engine/CUserProfileManager.h"
class CFrontendProfileList {
public:
    std::vector<CWideString> Names;                      // +0x00
    void Refresh();
};
void CFrontendProfileList::Refresh()
{
    Names.erase(Names.begin(), Names.end());
    std::vector<CDateAndTime> times;
    CUserProfileManager::Get().GetSortedSaveNamesAndTimes(Names, times);
}