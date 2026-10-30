// GFAllocEngine() -- the engine factory: new CEngine (0x178 bytes), null when the allocation fails
// Leaf: ?GFAllocEngine@@YIPAVCIEngine@@XZ
// Identity (naming wave 11): ?GFAllocEngine@@YIPAVCIEngine@@XZ (Ego_r.pdb 0x009B0B40 + FableWin.pdb); the catalog label CEnginePrimitive2DViewportManager::CreateInternalPrimitive is propagated. Callee 0x00B260B0 is CEngine::CEngine (catalog + Ego_r).
// p = new(0x178); if(p) return p->Construct(); else return 0;
// Construct() is a this-only ctor, tail-called (jmp); function pops no args (plain ret).

class CIEngine { };                                     // the engine interface (CEngine's base at offset 0)
class CEngine : public CIEngine {
public:
    CEngine();                                          // 0x00B260B0 ??0CEngine@@QAE@XZ (Ego_r.pdb 0x009B08C0)
    unsigned char _body[0x178];                         // retail new size 0x178
};
CIEngine* __fastcall GFAllocEngine()
{
    return new CEngine;
}
