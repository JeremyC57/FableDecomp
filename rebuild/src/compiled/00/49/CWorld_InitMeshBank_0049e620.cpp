// CWorld::InitMeshBank() -- open the main mesh bank (MBANK_ALLMESHES) through the system's mesh bank manager, take its animation manager, and make it the graphic appearances' global bank, with two loading-progress ticks
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049E620, 285 bytes (288 with padding), plain `ret`. PDB ?InitMeshBank@CWorld@@IAE_NXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, __fastcall DisplayProgress(const
//   CCharString&, float, bool, bool) 0x009E9F40, __fastcall GFGetSystemManager() 0x009A4EC0, CMeshBankManager::
//   OpenMeshBank(const CCharString&, bool) 0x00A09F20 (CCountedPointer<CMeshDataBank> by hidden pointer), operator
//   delete 0x00BFE9BC, NGraphicAppearance::CTCGraphicAppearance::SetMeshBank(CMeshDataBank&) 0x004BBFD0 (static
//   __fastcall: stores the global bank 0x013B8A04; manifest label "SetCurrentThingManager" is wrong).
// Strings: "Opening Mesh Bank" 0x01238DEC, "MBANK_ALLMESHES" 0x01238DDC, "Setting Mesh Bank" 0x01238DC8.
// Layout (engine/CWorld.h, PDB = retail here): PMainMeshBank (CCountedPointer: Data +0x3C, Info +0x40),
//   PAnimationManager +0x44; CSystemManager mesh bank manager +0x74; CMeshDataBank animation manager +0x3C0.
//   The header flattens the counted pointer into two raw fields, so `this` is a local model with the real member type.
// Shape: the progress texts are named CCharStrings in their own blocks (landed CWorldMap::SetRegionAsLoaded); the bank
//   assignment is one full expression: implicit "MBANK_ALLMESHES" temporary first, the counted-pointer result built in
//   the frame, the inline CCountedPointer operator= (source words loaded first, old block released, new one add-ref'd),
//   then the result's inline destructor (release, both words cleared) and the string's. The operator= copies the
//   source words into locals first (retail's ebx/edi pair) and the bank manager is reached through an inline
//   reference getter, which puts GFGetSystemManager ahead of the argument pushes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&, float, bool, bool); }   // 0x009E9F40   // 0x009E9F40
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
class C3DAnimationManager;
class CMeshDataBank { public:
    unsigned char        _pad[0x3c0];
    C3DAnimationManager* AnimationManager;             // +0x3C0
};
class CMeshBankManager { public:
    CCountedPointer<CMeshDataBank> OpenMeshBank(const CCharString& name, bool async);   // 0x00A09F20
};
class CSystemManager { public:
    unsigned char     _pad[0x74];
    CMeshBankManager* MeshBankManager;                 // +0x74
    CMeshBankManager& GetMeshBankManager() { return *MeshBankManager; }
};
CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    static void __fastcall SetMeshBank(CMeshDataBank& bank);   // 0x004BBFD0
};
}

class CWorld { public:
    unsigned char                  _pad00[0x3c];
    CCountedPointer<CMeshDataBank> PMainMeshBank;      // +0x3C
    C3DAnimationManager*           PAnimationManager;  // +0x44
    bool InitMeshBank();
};

bool CWorld::InitMeshBank()
{
    {
        CCharString text("Opening Mesh Bank");
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }
    PMainMeshBank = GFGetSystemManager()->GetMeshBankManager().OpenMeshBank("MBANK_ALLMESHES", false);
    PAnimationManager = PMainMeshBank->AnimationManager;
    {
        CCharString text("Setting Mesh Bank");
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }
    NGraphicAppearance::CTCGraphicAppearance::SetMeshBank(*PMainMeshBank);
    return true;
}