// Behaviour test for 0x00BC9C30 CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail
#include <cstdio>
class CBaseClass { public: CBaseClass() {} virtual ~CBaseClass() {} };
class CBaseClassNonCopyable : public CBaseClass { protected: CBaseClassNonCopyable() {} virtual ~CBaseClassNonCopyable() {} };
class CEngineInternalPrimitiveBase { public: int Dummy; };
class CEngineInternalPrimitiveSpriteTrail : public CEngineInternalPrimitiveBase { };
class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags; unsigned char Alpha;
    CEngineInternalPrimitiveBase* Parent; CEngineSubPrimitiveBase* NextSubPrimitive; CEnginePrimitiveManager* Manager;
};
class CEngineSubPrimitiveSpriteTrail : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent);
};
CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent)
    : CEngineSubPrimitiveBase(parent) { }
int main()
{
    CEngineInternalPrimitiveSpriteTrail owner;
    CEngineSubPrimitiveSpriteTrail* sub = new CEngineSubPrimitiveSpriteTrail(&owner);
    sub->NextSubPrimitive = sub;
    CEngineSubPrimitiveSpriteTrail sub2(&owner);
    if (sub2.Parent != &owner || sub2.NextSubPrimitive != 0 || sub->Parent != &owner) { std::printf("00bc9c30_TEST FAIL\n"); return 1; }
    delete sub;
    std::printf("00bc9c30_TEST PASS\n");
    return 0;
}