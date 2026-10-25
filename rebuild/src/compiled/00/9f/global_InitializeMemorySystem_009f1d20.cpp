// InitializeMemorySystem() -- read boot.ini into a GlobalAlloc buffer and walk it as `name = value` lines (skipping leading whitespace, `//` comment lines and trailing whitespace, then trimming around the first '='); nothing is kept from the parse in retail, so only the scan (its isspace calls) survives before the buffer is freed
// Leaf: ?InitializeMemorySystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F1D20, 347 bytes, no arguments, plain `ret`. Single caller: 0x009F20E3 in the
//   constructor at 0x009F20B0 (it initialises and enters a critical section at 0x013CAA70 first, then clears its tables),
//   so the catalog label "InitializeMemorySystem" names the caller's purpose; the body is a boot.ini reader whose
//   per-setting handling was compiled out (no stores of the parsed name / value survive).
// Callees: CreateFileA (IAT 0x0143FD48), GetLastError (0x0143FD58), GetFileSize (0x0143FDF0), GlobalAlloc (0x0143FDD4),
//   ReadFile (0x0143FED0), CloseHandle (0x0143FE30), GlobalFree (0x0143FDF8), isspace (MSVCR71 import through the
//   direct thunk 0x00BFEDBE).
// Globals: the file name "boot.ini" 0x0129C770.
// Shape: CreateFileA(GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN); a failed open calls
//   GetLastError and returns; GlobalAlloc(GPTR, size); every scan is a `p < end && test` while loop. The '=' search is an
//   inline helper: written in place, VC7.1 threads the empty-search exit straight past the `p < equals` test (retail
//   jumps to that test).
extern "C" __declspec(dllimport) void* __stdcall CreateFileA(const char* name, unsigned long access, unsigned long share,
                                                             void* security, unsigned long disposition,
                                                             unsigned long flags, void* templateFile);
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void* file, unsigned long* sizeHigh);
extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int flags, unsigned long bytes);
extern "C" __declspec(dllimport) int __stdcall ReadFile(void* file, void* buffer, unsigned long bytesToRead,
                                                        unsigned long* bytesRead, void* overlapped);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* object);
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* memory);
extern "C" int __cdecl isspace(int ch);

inline char* FindChar(char* p, char* end, char ch)
{
    while (p < end && *p != ch)
        ++p;
    return p;
}

void InitializeMemorySystem()
{
    void* file = CreateFileA("boot.ini", 0x80000000, 1, 0, 3, 0x08000000, 0);
    if (file == (void*)-1) {
        GetLastError();
        return;
    }
    unsigned long size = GetFileSize(file, 0);
    char* buffer = (char*)GlobalAlloc(0x40, size);
    unsigned long bytesRead;
    ReadFile(file, buffer, size, &bytesRead, 0);
    CloseHandle(file);

    char* end = buffer + size;
    char* p = buffer;
    while (p < end) {
        while (p < end && isspace(*p))
            ++p;
        char* lineEnd = p;
        while (lineEnd < end && *lineEnd != '\n' && *lineEnd != '\r')
            ++lineEnd;
        if (!(p + 1 < end && p[0] == '/' && p[1] == '/')) {
            char* last = lineEnd;
            while (p < last && isspace(last[-1]))
                --last;
            char* equals = FindChar(p, last, '=');
            if (p < equals && equals < last) {
                char* nameEnd = equals;
                while (p < nameEnd && isspace(nameEnd[-1]))
                    --nameEnd;
                char* value = equals + 1;
                while (value < last && isspace(*value))
                    ++value;
            }
        }
        p = lineEnd;
    }
    GlobalFree(buffer);
}