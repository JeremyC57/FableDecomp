// Grade: DIFFER(898v969) -- ours gets an aligned ebp frame (push ebp; mov ebp,esp; and esp,-8; sub esp,0x5c; this in edi) where retail has a plain sub esp,0x54 frame with this in ebp; the volume loop then diverges in temporaries and ours is 71 bytes short; no spelling of the 8-byte entry temporaries removed the alignment
// CPhysicsMesh::GetContainmentDataFromFileData(const C3DMeshFile& file) -- read the mesh file's volumes: NAV_VOLUME_<n> volumes go to the navigation-volume map keyed by n-1, the rest to the named containment-volume map; both maps are re-sorted when touched
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6D670, 976 bytes incl. padding (969 code), `ret 4`.
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / 0x0099EAE0, GetSubString 0x0099EC70, GFCharStringToInt 0x0099E7F0, operator delete 0x00BFE9BC, C3DMeshFile GetRootChunk 0x00A32620 / HasVolumes 0x00A9B2C0 / GetVolumesChunk 0x00A9B3D0 (manifest: CCamera/CMap names are folds), GetFirstVolume 0x00AAA830 / GetNextVolume 0x00AAA8C0 (manifest: FindFirstBone/FindNextBone), C3DMeshVolume ctor 0x00A2E9A0, nav vector _M_insert_overflow 0x00A72CC0, named push_back 0x00A73530, sorts 0x00A73B20 / 0x00A73BA0.
// Layout: CPhysicsMesh: nav-volume vector +0x54 (sorted flag +0x61), named-volume vector +0x64 (sorted flag +0x71), materials +0xbc (retail, Ego_r.pdb less 4 per STLport container).
// Shape: loop over the volume chunks with CCountedPointer iterators; the "NAV_VOLUME_" prefix test picks the map; each map is re-sorted only when its dirty flag is set.
#include <string.h>
inline void* operator new(unsigned int, void* p) { return p; }
void operator delete(void* p);                         // 0x00BFE9BC
struct CStringData {
    char*         PData;
    unsigned long StringLength;
};
struct CCharString {
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString GetSubString(long start, long end) const;   // 0x0099EC70
    const char* GetString() const { return PStringData ? PStringData->PData : ""; }
    unsigned long Length() const { return PStringData ? PStringData->StringLength : 0; }
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (Info != o.Info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = o.Data;
            Info = o.Info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
};
template <class T> struct CIVCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CIVCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};
struct C3DMeshFileVolumeChunk;
struct C3DMeshFileVolumeData {
    void Initialise();                                 // 0x00A753F0
};
struct C3DMeshFileVolumesChunk {
    CCountedPointer<C3DMeshFileVolumeChunk> GetFirstVolume() const;   // 0x00AAA830
    CCountedPointer<C3DMeshFileVolumeChunk> GetNextVolume(const CCountedPointer<C3DMeshFileVolumeChunk>& current) const;   // 0x00AAA8C0
};
struct C3DMeshFileRootChunk {
    bool HasVolumes() const;                           // 0x00A9B2C0
    CCountedPointer<C3DMeshFileVolumesChunk> GetVolumesChunk() const;   // 0x00A9B3D0
};
struct C3DMeshFile {
    CIVCountedPointer<C3DMeshFileRootChunk> GetRootChunk() const;   // 0x00A32620
};
struct C3DMeshVolume {
    CCharString                            Name;
    CCountedPointer<C3DMeshFileVolumeData> Volume;
    C3DMeshVolume(C3DMeshFileVolumeChunk* chunk, void* materials);   // 0x00A2E9A0
};
struct CNavVolumeEntry {
    long          first;
    C3DMeshVolume second;
    CNavVolumeEntry(long f, const C3DMeshVolume& s) : first(f), second(s) {}
};
struct CNamedVolumeEntry {
    CCharString   first;
    C3DMeshVolume second;
    CNamedVolumeEntry(const CCharString& f, const C3DMeshVolume& s) : first(f), second(s) {}
};
struct __false_type {};
struct CNavVolumeMap {
    CNavVolumeEntry* _M_start;
    CNavVolumeEntry* _M_finish;
    CNavVolumeEntry* _M_end_of_storage;
    bool             Compare;                          // +0x0c
    bool             NeedsSort;                        // +0x0d
    void _M_insert_overflow(CNavVolumeEntry* pos, const CNavVolumeEntry& x, const __false_type&, unsigned int n, bool atEnd);   // 0x00A72CC0
    void push_back(const CNavVolumeEntry& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CNavVolumeEntry(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void __fastcall SortNavVolumes(CNavVolumeEntry* first, CNavVolumeEntry* last, bool compare);   // 0x00A73B20
struct CNamedVolumeMap {
    CNamedVolumeEntry* _M_start;
    CNamedVolumeEntry* _M_finish;
    CNamedVolumeEntry* _M_end_of_storage;
    bool               Compare;                        // +0x0c
    bool               NeedsSort;                      // +0x0d
    void push_back(const CNamedVolumeEntry& x);        // 0x00A73530
};
void __fastcall SortNamedVolumes(CNamedVolumeEntry* first, CNamedVolumeEntry* last, bool compare);   // 0x00A73BA0

struct CPhysicsMesh {
    unsigned char   _pad00[0x54];
    CNavVolumeMap   NavigationVolumes;                 // +0x54
    CNamedVolumeMap ContainmentVolumes;                // +0x64
    unsigned char   _pad74[0x48];
    void*           Materials;                         // +0xbc
    void GetContainmentDataFromFileData(const C3DMeshFile& file);
};

void CPhysicsMesh::GetContainmentDataFromFileData(const C3DMeshFile& file)
{
    CCharString prefix("NAV_VOLUME_");
    CIVCountedPointer<C3DMeshFileRootChunk> root = file.GetRootChunk();
    if (root.Data->HasVolumes()) {
        CCountedPointer<C3DMeshFileVolumesChunk> chunk = root.Data->GetVolumesChunk();
        CCountedPointer<C3DMeshFileVolumeChunk> volume = chunk.Data->GetFirstVolume();
        while (volume.Data) {
            C3DMeshVolume meshVolume(volume.Data, Materials);
            meshVolume.Volume.Data->Initialise();
            CCharString name(meshVolume.Name);
            if (strncmp(name.GetString(), prefix.GetString(), prefix.Length()) == 0) {
                NavigationVolumes.push_back(CNavVolumeEntry(GFCharStringToInt(name.GetSubString(prefix.Length(), name.Length())) - 1, meshVolume));
                NavigationVolumes.NeedsSort = true;
            } else {
                ContainmentVolumes.push_back(CNamedVolumeEntry(name, meshVolume));
                ContainmentVolumes.NeedsSort = true;
            }
            volume = chunk.Data->GetNextVolume(volume);
        }
        if (NavigationVolumes.NeedsSort) {
            SortNavVolumes(NavigationVolumes._M_start, NavigationVolumes._M_finish, NavigationVolumes.Compare);
            NavigationVolumes.NeedsSort = false;
        }
        if (ContainmentVolumes.NeedsSort) {
            SortNamedVolumes(ContainmentVolumes._M_start, ContainmentVolumes._M_finish, ContainmentVolumes.Compare);
            ContainmentVolumes.NeedsSort = false;
        }
    }
}
