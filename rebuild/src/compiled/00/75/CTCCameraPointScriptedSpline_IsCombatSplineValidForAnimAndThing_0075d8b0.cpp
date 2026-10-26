// CTCCameraPointScriptedSpline::IsCombatSplineValidForAnimAndThing(long anim, CThing* thing) -- true for a combat spline whose ValidAnimList contains the animation index (the thing is unused)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D8B0, length 58 bytes.
// Callees: the combat-spline test on `this` @ 0x0075C4F0 (virtual slot 28 type == 7 plus a def check; the manifest's
// "CThingFilter_IsSuspect::operator()" label is propagated), the set's find @ 0x00616580 (landed
// CRetailTree_longkey::find: hidden iterator result + const long& key).
// Layout: ValidAnimList (set<long>) at +0xc0 (PDB +0xcc; retail containers before it are 0xc narrower); the
// VC6-era tree keeps its head node pointer at +0, so end() is `[esi]`.
// Shape: the iterator is a named local returned into the `push ecx` frame slot and re-read from it
// (`mov edx,[esp+4]`; comparing the call's returned pointer loads `[eax]` instead); the key is the
// argument's own slot; the nested `if` gives the shared `xor al,al` tail.
class CThing;
struct SAnimNode;

struct CAnimSetIterator {
    SAnimNode* m_node;
    CAnimSetIterator() { }
};

class CAnimSet { public:                                   // VC6 Dinkumware set<long>: {head, size}
    SAnimNode*    m_head;
    unsigned long m_size;
    CAnimSetIterator find(const long& key);         // 0x00616580
};

class CTCCameraPointScriptedSpline { public:
    unsigned char _pad00[0xc0];
    CAnimSet      ValidAnimList;                    // +0xc0 (PDB +0xcc)
    bool IsCombatSpline();                          // 0x0075C4F0
    bool IsCombatSplineValidForAnimAndThing(long anim, CThing* thing);
};

bool CTCCameraPointScriptedSpline::IsCombatSplineValidForAnimAndThing(long anim, CThing* thing)
{
    if (IsCombatSpline()) {
        CAnimSetIterator it = ValidAnimList.find(anim);
        if (it.m_node != ValidAnimList.m_head)
            return true;
    }
    return false;
}