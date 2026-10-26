#include "engine/CDriveManager.h"  // retyped onto the PDB layout; byte parity re-verified
class CSystemManager; struct CDriveManager_Methods : CDriveManager {
    int Initialise(CSystemManager* manager);
};
int CDriveManager_Methods::Initialise(CSystemManager* manager){ this->SystemManager = manager; return 1; }