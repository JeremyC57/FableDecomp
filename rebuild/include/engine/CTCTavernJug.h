#ifndef FABLE_ENGINE_CTCTAVERNJUG_H
#define FABLE_ENGINE_CTCTAVERNJUG_H
// New header (chunk 1024, row 0x00967D40 CAIStateGroup_TavernServer::Update): Ego_r.pdb CTCTavernJug (0x38 bytes, packed):
// Fullness +0x0C, OnBar +0x10, RHSet +0x11, Pos +0x29 (the landed CTCTavernJug::ReplaceJug 0x00968090 reads the same offsets).
// Interface 0xBB = TCI_TAVERN_JUG (bit 27 of the UsingComponents word at CThing +0x34).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#pragma pack(push, 1)
class CTCTavernJug : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TAVERN_JUG; }
    void ReplaceJug();                                                   // 0x00968090
    long            Fullness;                                            // +0x0C
    bool            OnBar;                                               // +0x10
    CRightHandedSet RHSet;                                               // +0x11
    C3DVector       Pos;                                                 // +0x29
};
#pragma pack(pop)
#endif
