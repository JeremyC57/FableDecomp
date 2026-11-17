#ifndef FABLETLC_WORLD_MAP_THEME_H
#define FABLETLC_WORLD_MAP_THEME_H

#include <stddef.h>
#include "rebuild_abi.h"

struct C2DCoordI
{
    long x;
    long y;
};

enum EThemePaintType
{
    EThemePaintType_Default = 0,
    EThemePaintType_Replace = 1
};

class CMap
{
#ifdef FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED
    // NF78 opt-in (round 7, row 0x0063A970 CWorldMap::SetThemeAtBlock): Ego_r.pdb and FableWin.pdb give these five CMap members
    // protected (?AddThemeDefIndexToPalette@CMap@@IAEEK@Z, ?SetEngineThemeAt@CMap@@IAEXJJKKW4EThemePaintType@@@Z,
    // ?SetEngineBlendAt@CMap@@IAEXJJEKW4EThemePaintType@@@Z, ?GetEngineThemeAt@CMap@@IBEKJJKW4EThemePaintType@@@Z,
    // ?GetEngineBlendAt@CMap@@IBEEJJKW4EThemePaintType@@@Z); CWorldMap's block accessors call them, so CWorldMap is a friend of CMap
    // (as engine/CMap.h FABLE_RETAIL_CMAP_CWORLDMAP_FRIEND). Access only: no layout change.
    friend class CWorldMap;
protected:
#else
public:
#endif
    unsigned char AddThemeDefIndexToPalette(unsigned long themeId);
    void SetEngineThemeAt(
        long x,
        long y,
        unsigned long themeId,
        unsigned long themeValue,
        EThemePaintType paintType);
    void SetEngineBlendAt(
        long x,
        long y,
        unsigned char blendValue,
        unsigned long themeId,
        EThemePaintType paintType);
    unsigned long GetEngineThemeAt(
        long x,
        long y,
        unsigned long themeId,
        EThemePaintType paintType) const;
    unsigned char GetEngineBlendAt(
        long x,
        long y,
        unsigned long themeId,
        EThemePaintType paintType) const;
};

class CWorldMap
{
public:
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1C();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2C();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3C();
    virtual void Slot40();
    virtual long GetMapNumberAt(const C2DCoordI& worldMapCoord) const;
    virtual CMap& GetMap(long mapNumber);
    virtual const CMap& PeekMap(long mapNumber) const;
    virtual void Slot50();
    virtual void Slot54();
    virtual void Slot58();
    virtual void Slot5C();
    virtual void Slot60();
    virtual bool IsPosChangeable(const C2DCoordI& worldMapCoord) const;

    C2DCoordI ConvertWorldMapCoordsToLocalCoords(
        long mapNumber,
        const C2DCoordI& worldMapCoord) const;

    void SetThemeAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned long themeId,
        unsigned long themeValue,
        EThemePaintType paintType);
    void SetThemeBlendAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned char blendValue,
        unsigned long themeId,
        EThemePaintType paintType);
    unsigned long GetThemeAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned long themeId,
        EThemePaintType paintType) const;
    unsigned long GetThemeBlendAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned long themeId,
        EThemePaintType paintType) const;
};

FABLE_STATIC_ASSERT_AT(101, sizeof(C2DCoordI) == 8);

#endif
