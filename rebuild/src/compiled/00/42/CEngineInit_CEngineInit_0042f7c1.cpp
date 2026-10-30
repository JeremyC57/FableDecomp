// CEngineInit::~CEngineInit() -- implicit destructor of the five CWideString resource paths
// Leaf: ??1CEngineInit@@QAE@XZ
// Identity (naming wave 11): ??1CEngineInit@@QAE@XZ (Ego_r.pdb 0x0043CB3F; Ego_r CEngineInit holds MeshResourcePath .. WriteableEngineCacheResourcePath as its five CWideString members); was CWideStringQuintet_0042f7c1.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042F7C1, length 44 bytes.
// Callees: CWideString::~CWideString @ 0x0099B510 on +0x30, +0x2c, +0x28, +0x24 and +0x20
// (reverse declaration order, the last one tail-called). No vtable store, no base call: a
// non-polymorphic aggregate whose destructor is declared and defined out of line so it is the
// TU's symbol. The manifest label (CarryCrateAndFollow dtor) is propagated garbage; the class is
// named by shape.
class CWideString { public:
    void* m_pData;
    ~CWideString();                         // 0x0099B510
};

class CEngineInit {                                    // Ego_r.pdb CEngineInit (0x68; retail keeps the paths 4 bytes lower)
public:
    unsigned char _pad[0x20];               // +0x00..+0x1f (DefinitionManager, EngineDef, ... the bank pointers)
    CWideString MeshResourcePath;           // +0x20
    CWideString GraphicsResourcePath;       // +0x24
    CWideString ShaderResourcePath;         // +0x28
    CWideString EngineCacheResourcePath;    // +0x2c
    CWideString WriteableEngineCacheResourcePath;   // +0x30
    ~CEngineInit();
};
CEngineInit::~CEngineInit()
{
}
