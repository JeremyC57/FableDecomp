// CDefinitionManager::ProcessPropertyDefinition(SPropertyContext* context, SPropertyTable* table, long* classIndices, long classCount) -- for every property bit of every row block of a property table, map the class index through the definition manager and accumulate the matrix for every set property; returns 0 (the manifest label is propagated: this is a free `__fastcall` matrix/property walker, not a CDefinitionManager member)
// Grade: DIFFER(429v399) -- best effort: a free __fastcall matrix walker (not a CDefinitionManager member); the loop nest shape is reconstructed from the disassembly, spills differ
// Leaf: ProcessPropertyDefinition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA15C0, length 399 bytes, `ret 0xc` (`__fastcall`: context in ecx, table in edx, three stack words).
// Callees: MemAllocator_Allocate @ 0x00C8F0D0 (`__fastcall` allocator, byte count), GetPDefFromClassIndex @ 0x00C96430 (`__fastcall`
// manager, class slot), PropertyDef_AccumulateMatrixByStride @ 0x00C96760 (`__fastcall` ... four stack words).
// Layout: table {rows +0 (begin), +4 (end), row stride +8, +0x10 -> {column count}, +0x14 masks, +0x1c defs, property count +8}.
// Shape: best-effort -- the row count is (end - begin) / stride, the block count rounds up by the column count; the outer loop runs over
// the property bits, the middle over the blocks and the inner over the columns, resolving the def pointer once per block.
struct SColumns { long Count; };
struct STable {
    unsigned char* Begin;                  // +0x00
    unsigned char* End;                    // +0x04
    long Stride;                           // +0x08
    long PropertyCount;                    // +0x0c (read as +8 of the context)
    unsigned long* Masks;                  // +0x14 (indexed by class)
};
struct SContext {
    STable* Table;                         // +0x00
    long _pad04;
    long PropertyCount;                    // +0x08
    long _pad0c;
    SColumns* Columns;                     // +0x10
    long** DefMatrix;                      // +0x14
    long _pad18;
    long* DefByClass;                      // +0x1c
};
void* __fastcall MemAllocator_Allocate(void* allocator, long bytes);                             // 0x00C8F0D0
long __fastcall GetPDefFromClassIndex(void* manager, long* classIndex);                         // 0x00C96430
long __fastcall AccumulateMatrix(void* manager, long def, unsigned char* row, long* classIndex, long count, long stride);   // 0x00C96760

long __fastcall ProcessPropertyDefinition(void* manager, SContext* context, long* classIndices, long classCount, long unused)
{
    STable* table = context->Table;
    long stride = table->Stride;
    long columns = context->Columns->Count;
    long rows = (long)(table->End - table->Begin) / stride;
    long blocks = (rows + columns - 1) / columns;
    long* slots = (long*)MemAllocator_Allocate(manager, blocks * 4);
    long* firstSlot = slots;
    long first = 0;
    while (first < classCount && classIndices[first] == 0)
        ++first;
    if (first == classCount)
        return 0;
    for (long property = 0; property < context->PropertyCount; ++property) {
        long** slot = (long**)slots;
        for (long block = 0; block < rows; ++block, ++slot) {
            if (property == 0) {
                long index = GetPDefFromClassIndex(manager, classIndices + 1);
                if (index == -1)
                    return 0;
                *slot = (long*)context->DefByClass[index];
                if (*slot == 0)
                    return 0;
            }
            long row = block * stride;
            for (long column = 0; column < columns && block < rows; ++column, row += stride) {
                long cls = (*slot)[column];
                if (table->Masks[cls] & (1L << property)) {
                    long def = context->DefMatrix[cls][property];
                    if (def && AccumulateMatrix(manager, def, table->Begin + row, classIndices + 1, classCount, stride) == -1)
                        return 0;
                }
            }
        }
    }
    return 0;
}
