// Behaviour test for 009cd3c0: CBankFile::DeleteEntry -- reset the record, delete the update data, clear the block order, bump the change count.
#include <cstdio>
static int g_deletes = 0, g_bufferDeletes = 0;
struct TRuntime { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid; TRuntime() : DataOffset(0), DataSize(0), DataType(0), Valid(false) {} };
struct TUpdate { unsigned long InfoSize; char* InfoBuffer;
    ~TUpdate() { if (InfoBuffer) { delete[] InfoBuffer; ++g_bufferDeletes; InfoBuffer = 0; } InfoSize = 0; ++g_deletes; } };
struct TBank {
    TRuntime RuntimeData[4]; TUpdate* UpdateData[2]; unsigned long updateCount; unsigned long ChangeCount; unsigned long blockCount;
    void DeleteEntry(unsigned long index)
    {
        RuntimeData[index] = TRuntime();
        if (index < updateCount) { TUpdate* u = UpdateData[index]; if (u) delete u; UpdateData[index] = 0; }
        blockCount = 0;
        ++ChangeCount;
    }
};
int main()
{
    TBank b; b.updateCount = 2; b.ChangeCount = 5; b.blockCount = 3;
    b.RuntimeData[1].DataOffset = 9; b.RuntimeData[1].Valid = true; b.RuntimeData[3].DataSize = 4;
    TUpdate* u = new TUpdate; u->InfoSize = 3; u->InfoBuffer = new char[3]; b.UpdateData[1] = u; b.UpdateData[0] = 0;
    b.DeleteEntry(1);
    if (b.RuntimeData[1].DataOffset || b.RuntimeData[1].Valid || b.UpdateData[1] || g_deletes != 1 || g_bufferDeletes != 1 || b.ChangeCount != 6 || b.blockCount) { std::printf("009cd3c0_TEST FAIL\n"); return 1; }
    b.DeleteEntry(3);   // beyond the update array: record only
    if (b.RuntimeData[3].DataSize || g_deletes != 1 || b.ChangeCount != 7) { std::printf("009cd3c0_TEST FAIL\n"); return 1; }
    std::printf("009cd3c0_TEST PASS\n");
    return 0;
}
