// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"

class CWorld
{
public:
    void SetSaveGameMarkerPos(const C3DVector* pos);
};

class CGameScriptInterface
{
public:
    void SetSaveGameMarkerPos(const C3DVector& pos);
    char m_Pad[0x4];
    CWorld* m_World;
};

void CGameScriptInterface::SetSaveGameMarkerPos(const C3DVector& pos)
{
    m_World->SetSaveGameMarkerPos(&pos);
}