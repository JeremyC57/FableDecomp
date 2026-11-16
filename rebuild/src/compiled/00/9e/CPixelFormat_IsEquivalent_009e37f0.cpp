// CPixelFormat::IsEquivalent(const CPixelFormat&) const -- same format index -- NF37 (2026-11-16) port off the address-suffixed stand-in onto the shared engine/CPixelFormat.h.
// Leaf: ?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z
// Evidence: Ego_r.pdb ?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z; the header ISEQUIVALENT opt-in names 0x009E37F0 (out of line); FormatIndex +0x00.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_ISEQUIVALENT
#include "engine/CPixelFormat.h"

bool CPixelFormat::IsEquivalent(const CPixelFormat& other) const
{
    return FormatIndex == other.FormatIndex;
}
