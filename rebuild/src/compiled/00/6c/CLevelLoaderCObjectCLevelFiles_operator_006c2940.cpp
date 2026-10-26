// CLevelLoader::CObject::CLevelFiles::operator=(const CLevelFiles& other) -- memberwise assignment of a level-file record: the level id and the three counted file pointers
// Leaf: ??4CLevelFiles@CObject@CLevelLoader@@QAEAAV012@ABV012@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C2940, 194 bytes (+ int3 padding), thiscall `ret 4`, returns `this`.
// Callers: one site, 0x006C2B39 (the STLport `__copy` over CLevelFiles records @ 0x006C2B10, 28-byte stride).
// Callees: operator delete @ 0x00BFE9BC; each released block's DeleteFunc (`call [info+4]`, __fastcall, object in ecx).
// Layout (Ego_r.pdb): CLevelLoader::CObject::CLevelFiles (28 bytes) = long LevelID +0, CCountedPointer<CAFile>
//   PTopologyFile +4, PNavigationFile +0xC, PThingFile +0x14 (as the landed __uninitialized_copy 0x006C2AA0);
//   CCountedPointer<T> {Data +0, Info +4}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
// Shape: the implicit (memberwise) copy assignment: LevelID, then three inline CCountedPointer::operator= -- both source
//   words read into locals before the block compare, the old block released through the member (DeleteFunc(Data) +
//   delete when the count reaches zero), the new words stored and the new block's count bumped.
// Label correction: the catalog `swap<CCompositeSurfaceInfo>` is propagated; Ego_r.pdb names
//   ??4CLevelFiles@CObject@CLevelLoader@@QAEAAV012@ABV012@@Z.
void operator delete(void* p);                          // 0x00BFE9BC

class CCPPointerInfo { public:                                 // 12 bytes (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04
    void*         Data;                                 // +0x08
};

template <class T>
class CCountedPointer {                                 // 8 bytes
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CAFile;

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                             // 28 bytes
        public:
            long LevelID;                               // +0x00
            CCountedPointer<CAFile> PTopologyFile;      // +0x04
            CCountedPointer<CAFile> PNavigationFile;    // +0x0C
            CCountedPointer<CAFile> PThingFile;         // +0x14
        };
    };
};

typedef CLevelLoader::CObject::CLevelFiles CLevelFiles;

// The implicit copy assignment, emitted out of line for the STLport __copy loop.
CLevelFiles& (CLevelFiles::*g_assign)(const CLevelFiles&) = &CLevelFiles::operator=;