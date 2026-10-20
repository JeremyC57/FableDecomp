// CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector* outA, C3DVector* outB, long flags) -- collect up to eight enabled hit locations whose volume the line segment crosses, sort them and return the best one seen from the start
// Leaf: GetPCombatFirstHitLocationIntersectionWithFiniteLine
//
// Retail 0x008168B0, 256 bytes incl. padding (`ret 14h`). The twin of the landed
// GetPCombatFirstHitLocationIntersectionWithTriangle 0x008169B0 (same model and shape).
// Callees: the hit-location def's volume object (CHitLocationDef +0x3C) virtual slot 7 (+0x1C, the finite-line test filling two
// C3DVector outs), std::sort<THitLocationIntersectionElement*, comparator> @ 0x00818C20 (SGI, /Gr __fastcall, the empty
// comparator by value), GetPBestHitLocationFromList(THitLocationIntersectionElement*, ulong, const C3DVector*, C3DVector*,
// C3DVector*) @ 0x00815F00 (__fastcall) with the line start.
// Shape: as 0x008169B0: the 0x100-byte array of 32-byte records filled through a strength-reduced pointer while the count is
// below 8, the vector end re-read each pass, the comparator value-initialised in the frame.
struct CThing;
struct C3DVector {
    float X, Y, Z;
};

struct CHitVolume {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool IntersectFiniteLine(CThing* owner, const C3DVector& start, const C3DVector& end, C3DVector* hitA,
                                     C3DVector* hitB, long flags);   // slot 7 (+0x1c)
};
struct CHitLocationDef {
    unsigned char _pad_0x00[0x3C];
    CHitVolume*   PVolume;                           // +0x3C
};

namespace NHitLocations {
struct CHitLocation {                                // 0x18 bytes
    CThing*          POwnerThing;                    // +0x00
    CHitLocationDef* PDef;                           // +0x04 CDefPointer<const CHitLocationDef>::Object
    void*            PArmourDef;                     // +0x08
    long             HitResponseAction;              // +0x0C
    long             BlockResponseAction;            // +0x10
    bool             Enabled;                        // +0x14
};
}

struct SHitLocationEntry {                           // pair<NHitLocations::CHitLocation, long>
    NHitLocations::CHitLocation first;               // +0x00
    long                        second;              // +0x18
};

struct THitLocationIntersectionElement {             // 32 bytes
    long                         Value;              // +0x00
    NHitLocations::CHitLocation* PHitLocation;       // +0x04
    C3DVector                    HitA;               // +0x08
    C3DVector                    HitB;               // +0x14
};

struct CHitLocationIntersectionLess {
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const;
};

namespace std {
void __fastcall sort(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last,
                     CHitLocationIntersectionLess comp);   // 0x00818C20
}

NHitLocations::CHitLocation* __fastcall GetPBestHitLocationFromList(THitLocationIntersectionElement* elements,
    unsigned long count, const C3DVector* origin, C3DVector* outA, C3DVector* outB);   // 0x00815F00

struct CTCHitLocations {
    void*              __vftable;                    // +0x00
    unsigned char      _pad_0x04[0x08];
    SHitLocationEntry* HitLocations_start;           // +0x0C vector begin
    SHitLocationEntry* HitLocations_finish;          // +0x10 vector end
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector& start,
        const C3DVector& end, C3DVector* outA, C3DVector* outB, long flags);
};

NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector& start,
    const C3DVector& end, C3DVector* outA, C3DVector* outB, long flags)
{
    THitLocationIntersectionElement hits[8];
    unsigned long count = 0;
    for (SHitLocationEntry* entry = HitLocations_start; entry != HitLocations_finish; ++entry) {
        if (entry->first.Enabled) {
            C3DVector hitA;
            C3DVector hitB;
            if (entry->first.PDef->PVolume->IntersectFiniteLine(entry->first.POwnerThing, start, end, &hitA, &hitB, flags) && count < 8) {
                hits[count].Value = entry->second;
                hits[count].PHitLocation = &entry->first;
                hits[count].HitA = hitA;
                hits[count].HitB = hitB;
                ++count;
            }
        }
    }
    std::sort(hits, hits + count, CHitLocationIntersectionLess());
    return GetPBestHitLocationFromList(hits, count, &start, outA, outB);
}
