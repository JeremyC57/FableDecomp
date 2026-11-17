// CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI&) -- when the map still needs it, mark each height-field cell of the area passable unless its theme forbids it or the ground there is steeper than a 0.7 normal
// Leaf: InitPassabilityFromThemesIfNecessary
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Shape: same-TU out-only stand-ins for GetDef<CThemeDef> (0x0063F3A0) and CMap::GetThemeIndexAt (0x0081F910) keep the
//   cell position, the weight and the def from escaping (x in edi, one shared out/def word, the def released without a
//   re-read); the theme clears passability when it lacks flag 0 or has a positive Passability (`if (...) passable =
//   false;` -- retail keeps the cell passable for Passability <= 0); the cell pointer steps before x.
// Callees: CMap theme lookup 0x0081F910 (labelled CopyPos), CGameDefinitionManager::Get 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<T> 0x0063F3A0, CMap::GetGroundFaceNormalAt 0x0081F6C0; constants 0.0f @ 0x0122DEDC, 0.7f @ 0x01255318.
// Layout: no CMap engine header; local model (height field +0x1A7C: cells +0x10 of 12 bytes, width +0x204, height +0x208; passability flag +0x1D38; theme def flags +0xA0 bit 0, passability +0x80).
// NF49 (round 7): the def classes and the manager come from the shared headers (were local CThemeDef / CDefPointer /
// CDefinitionManager / CGameDefinitionManager models plus a same-TU stand-in GetDef body over a made-up
// FindDef_CDefinitionManager_GetDef callee): the lookup at 0x0063F3A0 is the member template
// ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z (FableWin.pdb; the landed
// 0x0063DD70 / 0x0066CF00 include chain), with the header's same-TU body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, the
// stand-in's role). The descriptive CThemeDef is CEngineThemeDef (the template argument the FableWin symbol names); it has no
// shared header (engine/_quarantine only), so it stays modelled on the shared CDefClassBase as landed 0x0063DD70 does.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/C3DVector.h"
class C2DBoxI { public:
    C2DCoordI Min;
    C2DCoordI Max;
};
struct CHeightFieldCell {
    unsigned char _pad00[9];
    unsigned char Reserved : 1;
    unsigned char Passable : 1;                 // +0x09 bit 1
    unsigned char _rest : 6;
    unsigned char _pad0a[2];
};
struct CHeightField {
    unsigned char     _pad00[0x10];
    CHeightFieldCell* Cells;                    // +0x10
    unsigned char     _pad14[0x1f0];
    long              Width;                    // +0x204
    long              Height;                   // +0x208
    CHeightFieldCell* GetCell(long x, long y) { return &Cells[(Height - y) * (Width + 1) + x]; }
};
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x80 - 0x25];
    float         Passability;                  // +0x80
    unsigned char _pad84[0x1c];
    unsigned char Flags;                        // +0xa0 bit 0: passable
};
class CMap { public:
    virtual void v00();
    unsigned char _pad04[0x1a78];
    CHeightField* HeightField;                  // +0x1a7c
    unsigned char _pad1a80[0x2b8];
    bool          PassabilityNeedsInit;         // +0x1d38
    long GetThemeIndexAt(const C2DCoordI& pos, long* weight);         // 0x0081F910
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos);            // 0x0081F6C0
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area);
};
long LookUpThemeIndex_CMap(CMap* map, long x, long y, long& weight);   // stand-in callee
// Same-TU stand-in for CMap::GetThemeIndexAt (0x0081F910, compiled in this TU in retail): it reads the cell coordinates
// by value and writes the out weight only, so neither the position nor the weight escapes.
#pragma auto_inline(off)
__declspec(noinline) long CMap::GetThemeIndexAt(const C2DCoordI& pos, long* weight)
{
    long w;
    long index = LookUpThemeIndex_CMap(this, pos.X, pos.Y, w);
    *weight = w;
    return index;
}
#pragma auto_inline(on)

void CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI& area)
{
    if (!PassabilityNeedsInit)
        return;
    CHeightField* heightField = HeightField;
    C2DCoordI pos;
    for (pos.Y = area.Min.Y; pos.Y < area.Max.Y; ++pos.Y) {
        CHeightFieldCell* cell = heightField->GetCell(area.Min.X, pos.Y);
        for (pos.X = area.Min.X; pos.X < area.Max.X; ++cell, ++pos.X) {
            bool passable = true;
            long theme;
            {
                long weight;
                theme = GetThemeIndexAt(pos, &weight);
            }
            if (theme > 0) {
                CDefPointer<const CEngineThemeDef> def;
                CGameDefinitionManager::Get().GetDef(theme, &def);
                if (!(def->Flags & 1) || def->Passability > 0.0f)
                    passable = false;
            }
            if (passable) {
                C3DVector normal = GetGroundFaceNormalAt(C3DVector((float)pos.X, (float)pos.Y, 0.0f));
                if (normal.Z < 0.7f)
                    passable = false;
            }
            cell->Passable = passable;
        }
    }
}