#ifndef FABLE_ENGINE_CTCDISCOVERY_H
#define FABLE_ENGINE_CTCDISCOVERY_H

#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"

// Ego_r fields and native offsets, and original FableWin IsDiscovered 0x01F93DD0.
class CTCDiscovery : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_DISCOVERY; }
    bool IsDiscovered() const { return Discovered; }
    void SetDiscovered(bool discovered, const CThing* discoverer); // 0x007509D0
private:
    bool Discovered;
    CIntelligentPointer<const CThing> Discoverer;
};

FABLE_STATIC_ASSERT(sizeof(CTCDiscovery) == 0x18);
#endif
