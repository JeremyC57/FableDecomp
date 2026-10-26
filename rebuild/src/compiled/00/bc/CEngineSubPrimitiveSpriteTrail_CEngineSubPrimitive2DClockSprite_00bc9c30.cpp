// CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail(parent) -- stores parent and empty chain link.
// Retail00bc9c30 corrected34bytes. VT012a69d4 slot0=00bca060; base ctor0099a2f0
// writes CBaseClassNonCopyable VT0129a7c4 only. PDB CEngineSubPrimitiveBase size24,
// RenderFlags+4 Alpha+8 Parent+12 NextSubPrimitive+16 Manager+20; no engine header exists.
class CEngineInternalPrimitiveBase; class CEnginePrimitiveManager;
struct CBaseClassNonCopyable {
 CBaseClassNonCopyable(); //0099a2f0
 virtual ~CBaseClassNonCopyable(); //0099a300
};
class CEngineSubPrimitiveBase : public CBaseClassNonCopyable { public:
 // Native pooled primitive deallocator00beb550 is a no-op (single RET).
 static void operator delete(void*) {}
 unsigned int RenderFlags; unsigned char Alpha;
 CEngineInternalPrimitiveBase* Parent;
 CEngineSubPrimitiveBase* NextSubPrimitive;
 CEnginePrimitiveManager* Manager;
 CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* p):Parent(p),NextSubPrimitive(0){}
};
struct CEngineSubPrimitiveSpriteTrail : CEngineSubPrimitiveBase {
 CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveBase* p);
};
CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveBase* p):CEngineSubPrimitiveBase(p){}