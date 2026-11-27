#ifndef FABLE_ENGINE_CTEXTUREREPLACEMENTDEF_PASS29_H
#define FABLE_ENGINE_CTEXTUREREPLACEMENTDEF_PASS29_H
#ifdef FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
#include "engine/CSubDefClassBase.h"
#include "engine/CArray.h"
#include "fable_string.h"
class CTextureReplacementDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CTextureReplacementDef", -1); }
    class CEntry {
    public:
        long OriginalTexture;
        long NewTexture;
    };
    CArray<CEntry> Entries;
#ifdef FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
    long GetNoEntries() const { return Entries.size(); }
    const CEntry& GetEntry(long index) const { return Entries[index]; }
#endif
};
#endif
#endif
