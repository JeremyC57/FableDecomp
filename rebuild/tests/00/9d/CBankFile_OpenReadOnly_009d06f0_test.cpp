// Behaviour test for CBankFile::OpenReadOnly 0x009D06F0: the open state is reset (read-only, flags, handle, change count and
// the valid / written / solid / updating flags); with retail banks in use and flag 4 clear the bank is looked up in the
// manager's contained-bank table: not found or a data-version mismatch returns false, otherwise the alignment and disk file are
// taken from it, the entry list (count + 1 slots, count entries) is read from the info offset, the open is announced and the
// result is true. Otherwise (dev banks, or flag 4) the dev bank file path is opened read-only and its result returned.
// The manager, streams and file are test models of the row's callees.
#include <cstdio>
#include <cstring>

struct ContainedBank { long DataVersion; long Alignment; long InfoOffset; long EntryCount; };
struct Manager {
    bool UsingRetail; bool Found; ContainedBank Bank; int DiskFile; int FindCalls;
    bool FindContainedBank(const char* handle, ContainedBank& out, int*& file)
    { ++FindCalls; (void)handle; if (!Found) return false; out = Bank; file = &DiskFile; return true; }
};
static Manager g_mgr;
static const long kDataFormatVersion = 7;

struct BankFile {
    bool ReadOnly; unsigned long OpenFlags; char BankHandle[32]; long ChangeCount;
    bool FileValid, WrittenToFile, Solid, UpdatingFlag, RetailMode;
    long Alignment; int* File; long ReadPos, ReadSlots, ReadEntries; int Notified;
    char DevPath[64]; unsigned long DevFlags; bool DevResult;
    long GetDataFormatVersion() const { return kDataFormatVersion; }
    void ReadEntryList(long pos, long slots, long entries) { ReadPos = pos; ReadSlots = slots; ReadEntries = entries; }
    void NotifyBankOpened() { ++Notified; }
    bool OpenFileReadOnly(const char* path, unsigned long flags) { std::strcpy(DevPath, path); DevFlags = flags; return DevResult; }

    bool OpenReadOnly(const char* bankHandle, unsigned long flags)
    {
        ReadOnly = true;
        OpenFlags = flags;
        std::strcpy(BankHandle, bankHandle);
        ChangeCount = 0;
        FileValid = false;
        WrittenToFile = false;
        Solid = false;
        UpdatingFlag = false;
        if (g_mgr.UsingRetail && !(flags & 4)) {
            RetailMode = true;
            ContainedBank containedBank;
            int* diskFile = 0;
            if (!g_mgr.FindContainedBank(bankHandle, containedBank, diskFile))
                return false;
            if (containedBank.DataVersion != GetDataFormatVersion())
                return false;
            Alignment = containedBank.Alignment;
            File = diskFile;
            ReadEntryList(containedBank.InfoOffset, containedBank.EntryCount + 1, containedBank.EntryCount);
            NotifyBankOpened();
            return true;
        } else {
            char path[64];
            std::strcpy(path, "dev/"); std::strcat(path, bankHandle);
            return OpenFileReadOnly(path, flags);
        }
    }
};
static void Dirty(BankFile& b)
{
    std::memset(&b, 0, sizeof(b));
    b.ReadOnly = false; b.OpenFlags = 99; b.ChangeCount = 5; b.FileValid = b.WrittenToFile = b.Solid = b.UpdatingFlag = true;
    b.Alignment = -1; b.ReadPos = -1;
}
static bool ResetOk(const BankFile& b, unsigned long flags, const char* handle)
{
    return b.ReadOnly && b.OpenFlags == flags && std::strcmp(b.BankHandle, handle) == 0 && b.ChangeCount == 0 &&
           !b.FileValid && !b.WrittenToFile && !b.Solid && !b.UpdatingFlag;
}
static int fail(int n) { std::printf("009d06f0_TEST FAIL %d\n", n); return 1; }
int main()
{
    BankFile b;
    ContainedBank cb = { kDataFormatVersion, 0x800, 0x1234, 9 };
    g_mgr.UsingRetail = true; g_mgr.Found = true; g_mgr.Bank = cb; g_mgr.FindCalls = 0;
    Dirty(b);
    if (!b.OpenReadOnly("TEXTURES", 1) || !ResetOk(b, 1, "TEXTURES") || !b.RetailMode) return fail(1);
    if (b.Alignment != 0x800 || b.File != &g_mgr.DiskFile || b.ReadPos != 0x1234 || b.ReadSlots != 10 || b.ReadEntries != 9 || b.Notified != 1) return fail(2);
    g_mgr.Found = false; Dirty(b);
    if (b.OpenReadOnly("MISSING", 0) || !ResetOk(b, 0, "MISSING") || b.Notified != 0 || b.ReadPos != -1) return fail(3);
    g_mgr.Found = true; g_mgr.Bank.DataVersion = kDataFormatVersion + 1; Dirty(b);
    if (b.OpenReadOnly("OLD", 0) || b.Alignment != -1 || b.Notified != 0) return fail(4);
    int finds = g_mgr.FindCalls;
    Dirty(b); b.DevResult = true;                       // flag 4: dev path even with retail banks
    if (!b.OpenReadOnly("LEVEL", 4) || !ResetOk(b, 4, "LEVEL") || std::strcmp(b.DevPath, "dev/LEVEL") != 0 || b.DevFlags != 4) return fail(5);
    if (g_mgr.FindCalls != finds || b.RetailMode) return fail(6);
    g_mgr.UsingRetail = false; Dirty(b); b.DevResult = false;
    if (b.OpenReadOnly("LEVEL", 2) || std::strcmp(b.DevPath, "dev/LEVEL") != 0 || b.DevFlags != 2) return fail(7);
    std::printf("009d06f0_TEST PASS\n");
    return 0;
}