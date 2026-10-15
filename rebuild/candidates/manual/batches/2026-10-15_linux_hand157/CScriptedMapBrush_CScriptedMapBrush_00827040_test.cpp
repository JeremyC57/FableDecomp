// Behaviour test for 0x00827040 CScriptedMapBrush copy constructor: every member copied, the CBaseObject
// intelligent-pointer slot reset (CBaseObject's copy ctor does not copy it).
#include <cstdio>
static int g_containerCopies = 0;
struct CBaseObject { void* IntelligentPointer; CBaseObject() : IntelligentPointer(0) {} CBaseObject(const CBaseObject&) : IntelligentPointer(0) {} };
struct CCharString { int id; CCharString() : id(0) {} CCharString(const CCharString& o) : id(o.id) {} };
struct Container { int size; Container() : size(0) {} Container(const Container& o) : size(o.size) { ++g_containerCopies; } };
struct C2DBoxI { long TLX, TLY, BRX, BRY; C2DBoxI() {} C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {} };
struct CScriptedMapBrush : CBaseObject {
    Container Cells, ThingsToDelete, ThingsToMove, ThingsToAdd; C2DBoxI GameArea; CCharString BrushName; float CreationTimeSeconds; long CreationPattern;
    CScriptedMapBrush() {}
    CScriptedMapBrush(const CScriptedMapBrush& other)
        : CBaseObject(other), Cells(other.Cells), ThingsToDelete(other.ThingsToDelete), ThingsToMove(other.ThingsToMove),
          ThingsToAdd(other.ThingsToAdd), GameArea(other.GameArea), BrushName(other.BrushName),
          CreationTimeSeconds(other.CreationTimeSeconds), CreationPattern(other.CreationPattern) {}
};
int main()
{
    CScriptedMapBrush a; int dummy; a.IntelligentPointer = &dummy; a.Cells.size = 3; a.ThingsToAdd.size = 4;
    a.GameArea.TLX = 1; a.GameArea.TLY = 2; a.GameArea.BRX = 3; a.GameArea.BRY = 4; a.BrushName.id = 9; a.CreationTimeSeconds = 1.5f; a.CreationPattern = 6;
    CScriptedMapBrush b(a);
    if (b.IntelligentPointer != 0 || b.Cells.size != 3 || b.ThingsToAdd.size != 4 || g_containerCopies != 4 || b.GameArea.BRY != 4
        || b.BrushName.id != 9 || b.CreationTimeSeconds != 1.5f || b.CreationPattern != 6) { std::printf("00827040_TEST FAIL\n"); return 1; }
    std::printf("00827040_TEST PASS\n");
    return 0;
}
