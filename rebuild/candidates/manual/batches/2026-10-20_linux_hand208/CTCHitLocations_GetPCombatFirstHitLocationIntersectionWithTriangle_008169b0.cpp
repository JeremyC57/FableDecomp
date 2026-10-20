// CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithTriangle(const CTriangle&, C3DVector*, C3DVector*, long) -- collects up to eight enabled hit locations whose volume the triangle intersects, sorts them and returns the best one
// Leaf: GetPCombatFirstHitLocationIntersectionWithTriangle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008169B0, 234 bytes (+ int3 padding), `ret 10h`.
// Callees: the hit-location def's volume object (CHitLocationDef +0x3C) virtual slot 8 (+0x20, the triangle test
//          filling two C3DVector outs), std::sort<THitLocationIntersectionElement*, comparator> @ 0x00818C20 (SGI,
//          /Gr __fastcall: first in ecx, last in edx, the empty comparator by value), GetPBestHitLocationFromList(
//          THitLocationIntersectionElement*, ulong, C3DVector*, C3DVector*, C3DVector*) @ 0x00815F00 (__fastcall).
// Shape: a 0x100-byte array of 32-byte {pair.second, location, hit A, hit B} records in the frame, filled through a
// strength-reduced pointer (edi, written as indexed `hits[count].X = ...` stores) while the count (ebx) is below 8; the vector end is re-read each pass; the empty
// comparator temporary is value-initialised (`mov byte ptr [esp+0Ch],0`); the triangle's first word (its vertex
// pointer, a named local read after the sort) goes to the best-hit search with the two caller outs.
// Layout: Ego_r.pdb CTCHitLocations HitLocations vector<pair<NHitLocations::CHitLocation, long> > at +0x0C (begin/
// end at +0x0C/+0x10 in retail); CHitLocation (0x18): POwnerThing +0, PDef +4, Enabled +0x14.
struct CThing;
struct C3DVector {
    float X, Y, Z;
};

struct CTriangle {                                   // the caller's triangle (first word: its vertex pointer)
    C3DVector* PVertices;
};

struct CHitVolume {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual bool IntersectTriangle(CThing* owner, const CTriangle& triangle, C3DVector* hitA, C3DVector* hitB, long flags);   // slot 8 (+0x20)
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
    unsigned long count, C3DVector* vertices, C3DVector* outA, C3DVector* outB);   // 0x00815F00

struct CTCHitLocations {
    void*              __vftable;                    // +0x00
    unsigned char      _pad_0x04[0x08];
    SHitLocationEntry* HitLocations_start;           // +0x0C vector begin
    SHitLocationEntry* HitLocations_finish;          // +0x10 vector end
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithTriangle(const CTriangle& triangle,
        C3DVector* outA, C3DVector* outB, long flags);
};

NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithTriangle(const CTriangle& triangle,
    C3DVector* outA, C3DVector* outB, long flags)
{
    THitLocationIntersectionElement hits[8];
    unsigned long count = 0;
    for (SHitLocationEntry* entry = HitLocations_start; entry != HitLocations_finish; ++entry) {
        if (entry->first.Enabled) {
            C3DVector hitA;
            C3DVector hitB;
            if (entry->first.PDef->PVolume->IntersectTriangle(entry->first.POwnerThing, triangle, &hitA, &hitB, flags) && count < 8) {
                hits[count].Value = entry->second;
                hits[count].PHitLocation = &entry->first;
                hits[count].HitA = hitA;
                hits[count].HitB = hitB;
                ++count;
            }
        }
    }
    std::sort(hits, hits + count, CHitLocationIntersectionLess());
    C3DVector* vertices = triangle.PVertices;
    return GetPBestHitLocationFromList(hits, count, vertices, outA, outB);
}
