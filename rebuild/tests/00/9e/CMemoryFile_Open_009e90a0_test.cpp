// Behaviour test for 0x009E90A0 CMemoryFile::Open(): the path name is set empty, position and length are zeroed, the file is open.
#include <cstdio>
class CWideString { public: const unsigned short* Text; int Assigned; const CWideString& operator=(const unsigned short* t) { Text = t; ++Assigned; return *this; } };
class CMemoryFile { public:
    void* vtbl; long AccessType; long Position; long Length; char DataBuffer[0xc]; bool Opened; char pad[3]; CWideString Pathname;
    void Open();
};
void CMemoryFile::Open() { Pathname = (const unsigned short*)L""; Position = 0; Length = 0; Opened = true; }
int main() {
    CMemoryFile f;
    f.Position = 99; f.Length = 99; f.Opened = false; f.Pathname.Text = 0; f.Pathname.Assigned = 0;
    f.Open();
    if (f.Pathname.Assigned == 1 && f.Pathname.Text && f.Pathname.Text[0] == 0 && f.Position == 0 && f.Length == 0 && f.Opened) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}