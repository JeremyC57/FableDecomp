// CRegionSnapshot::CRegionSnapshot(const CRegionMap& map) -- copy a region map into a flat snapshot: the map's cell size and region count, a new[] array of that many 12-byte entries (their cell coordinates zeroed by the entry constructor), filled by a copying visitor run over the map's regions
// Leaf: ??0CRegionSnapshot@@QAE@ABVCRegionMap@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C46400, 176 bytes, __fastcall `ret 4` (returns this), EH-framed (handler
//   0x01215548; state 0 = the stack visitor).
// Library: libpk (no PDB/RTTI): descriptive names. Only caller the level setter at 0x00C37050, which builds a local
//   CRegionMap(level, cell size) (ctor 0x00C467D0, destroyed by 0x00C46860), then `new` (12 bytes) this snapshot
//   from it into its +0x0C. The manifest's "CMap_AllocateWaterGrid" is propagated.
// Callees: the map's GetCellSize 0x00C465E0 (+0x34), GetCount 0x00C465D0 (region list slot 15), ForEach 0x00C465B0
//   (region list slot 10 with the visitor); operator new[] 0x00BFEB22 (no cookie: the entry has no destructor).
// Layout: the visitor is the 16-byte CRegionCopier (vtable 0x012B7D58: slot 0 Visit 0x00C46360 copying entry
//   +4, +8, +0 into m_pDest[m_index++], slot 1 its deleting destructor 0x00C46500; base vtable 0x012B7D50).
// Shape: the init list stores the cell size and count, then the zeroed entry pointer; the inline vector-new loop
//   zeroes +4/+8 of each entry (a null result gives `xor eax,eax`); the copier is built from the stored pointer and
//   a re-read count.
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22 (without it VC7.1 calls scalar new 0x00BFEA1A)

struct C2DVectorI {
    long X;                                 // +0x00
    long Y;                                 // +0x04
    C2DVectorI() : X(0), Y(0) { }
};

struct SRegionEntry {
    long       m_visited;                   // +0x00
    C2DVectorI m_cell;                      // +0x04
};

class CRegionVisitor {                      // vtable 0x012B7D50
public:
    virtual void Visit(const SRegionEntry& entry) = 0;  // slot 0
    virtual ~CRegionVisitor() { }                       // slot 1
};

class CRegionCopier : public CRegionVisitor {           // vtable 0x012B7D58
public:
    SRegionEntry* m_pDest;                  // +0x04
    long          m_index;                  // +0x08
    long          m_count;                  // +0x0c
    CRegionCopier(SRegionEntry* dest, long count) : m_pDest(dest), m_index(0), m_count(count) { }
    virtual void Visit(const SRegionEntry& entry);      // 0x00C46360
};

class CRegionMap {
public:
    void ForEach(CRegionVisitor& visitor) const;        // 0x00C465B0
    long GetCount() const;                              // 0x00C465D0
    float GetCellSize() const;                          // 0x00C465E0
};

class CRegionSnapshot {
public:
    float         m_cellSize;               // +0x00
    long          m_count;                  // +0x04
    SRegionEntry* m_entries;                // +0x08
    CRegionSnapshot(const CRegionMap& map);
};

CRegionSnapshot::CRegionSnapshot(const CRegionMap& map)
    : m_cellSize(map.GetCellSize()), m_count(map.GetCount()), m_entries(0)
{
    m_entries = new SRegionEntry[m_count];
    CRegionCopier copier(m_entries, m_count);
    map.ForEach(copier);
}