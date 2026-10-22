#include "engine/CDriveManager.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSystemManager; struct CDriveManager_Methods : CDriveManager {
    int Initialise(CSystemManager* manager);
};
int CDriveManager_Methods::Initialise(CSystemManager* manager){ this->SystemManager = manager; return 1; }