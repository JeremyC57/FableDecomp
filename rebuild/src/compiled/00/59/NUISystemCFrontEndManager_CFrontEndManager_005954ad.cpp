// NUISystem::CFrontEndManager::~CFrontEndManager() -- release frontend screens and owned UI state.
// Retail: 0x005954ad,213 bytes,thiscall.
// Flags: /Os
// Leaf: ??1CFrontEndManager@NUISystem@@UAE@XZ
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_COBSERVER_DESTRUCTOR
#define FABLE_RETAIL_CMANAGER_CLEAR
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CManager.h"
NUISystem::CFrontEndManager::~CFrontEndManager()
{
 CloseVirtualKeyboard(false);
 std::map<FRONTEND_SCREEN,CComponent*>::const_iterator iter;
 for(iter=m_mapScreens.begin();iter!=m_mapScreens.end();++iter)
  if(iter->second)delete iter->second;
 CManager::Clear();
 m_pInstance=0;
}