// Behaviour test for CDriveManager::CreateDirectoryW @ 0x009E4DA0: same control flow over a fake drive / directory system.
#include <cstdio>
typedef unsigned short wchar16;

static int g_drive = 3;            // current drive number (C:)
static int g_chdriveCalls = 0;
static int g_lastChdrive = 0;
static int g_existing[8];          // per item: 1 = exists, 0 = missing
static int g_makeFails[8];         // per item: _wmkdir fails
static int g_items = 0;
static int g_made = 0;
static int g_driveOk = 1;

static void fake_wsplitpath(const wchar16* path, wchar16* drive) { drive[0] = path[0]; drive[1] = path[0] ? ':' : 0; drive[2] = 0; }
static int fake_getdrive() { return g_drive; }
static int fake_chdrive(int d) { ++g_chdriveCalls; g_lastChdrive = d; g_drive = d; return 0; }
static int fake_wchdir(const wchar16* item) { return g_existing[item[0]] ? 0 : -1; }
static int fake_wmkdir(const wchar16* item) { if (g_makeFails[item[0]]) return -1; g_existing[item[0]] = 1; ++g_made; return 0; }

class CDriveManager { public:
    long DriveExists(wchar16) { return g_driveOk ? 1 : 3; }
    long ConvertDriveLetterToNumber(wchar16 driveLetter) { return driveLetter - L'@'; }
    wchar16 GetDriveFromPath(const wchar16* path) { wchar16 drive[3]; fake_wsplitpath(path, drive); return drive[0]; }
    bool GetItemFromPath(const wchar16*, unsigned long index, wchar16* item) { if ((long)index >= g_items) return false; item[0] = (wchar16)index; item[1] = 0; return true; }
    long CreateDirectoryW(const wchar16* fullPath)
    {
        long result = 0;
        wchar16 driveNumber = GetDriveFromPath(fullPath);
        long oldDrive = -1;
        if (driveNumber) {
            result = DriveExists(driveNumber);
            driveNumber = (wchar16)ConvertDriveLetterToNumber(driveNumber);
            oldDrive = fake_getdrive();
            fake_chdrive(driveNumber);
        }
        if (!driveNumber || result == 1) {
            wchar16 item[262];
            for (unsigned long index = 0; GetItemFromPath(fullPath, index, item) == true; ++index) {
                if (fake_wchdir(item) != 0) {
                    if (fake_wmkdir(item) == -1 || fake_wchdir(item) != 0) {
                        result = 0;
                        break;
                    }
                }
            }
        }
        if (oldDrive != -1)
            fake_chdrive(oldDrive);
        return result;
    }
};

static void reset(int items) { g_drive = 3; g_chdriveCalls = 0; g_items = items; g_made = 0; g_driveOk = 1; for (int i = 0; i < 8; ++i) { g_existing[i] = 0; g_makeFails[i] = 0; } }

int main()
{
    CDriveManager m;
    wchar16 dpath[] = { 'D', ':', '\\', 'x', 0 };
    wchar16 rpath[] = { 0 };
    bool ok = true;
    // drive D: exists, two items missing, one present: all made, drive selected (4) then restored (3)
    reset(3); g_existing[1] = 1;
    long r = m.CreateDirectoryW(dpath);
    ok = ok && r == 1 && g_made == 2 && g_chdriveCalls == 2 && g_drive == 3;
    // second item cannot be made -> 0, drive still restored
    reset(3); g_makeFails[1] = 1;
    r = m.CreateDirectoryW(dpath);
    ok = ok && r == 0 && g_made == 1 && g_drive == 3;
    // drive missing (3): no items made, drive restored
    reset(2); g_driveOk = 0;
    r = m.CreateDirectoryW(dpath);
    ok = ok && r == 3 && g_made == 0 && g_chdriveCalls == 2;
    // no drive letter: items made, no drive change, result stays 0
    reset(2);
    r = m.CreateDirectoryW(rpath);
    ok = ok && r == 0 && g_made == 2 && g_chdriveCalls == 0;
    // the drive number: 'D' - '@' = 4
    reset(0); m.CreateDirectoryW(dpath);
    ok = ok && g_chdriveCalls == 2;
    reset(0); g_drive = 9; { wchar16 p[] = { 'D', 0 }; long dummy = m.CreateDirectoryW(p); (void)dummy; }
    ok = ok && g_drive == 9;
    if (!ok) { std::printf("009e4da0_TEST FAIL\n"); return 1; }
    std::printf("009e4da0_TEST PASS\n");
    return 0;
}