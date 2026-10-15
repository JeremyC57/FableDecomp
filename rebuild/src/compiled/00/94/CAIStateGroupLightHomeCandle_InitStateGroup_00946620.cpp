// CAIStateGroup_LightHomeCandle::InitStateGroup(const CWorld& world, CThingAICreature& creature) -- nothing to set up: lighting the home candle needs no per-creature state beyond what the base state group already holds
// Leaf: InitStateGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946620, length 3 bytes (`ret 8`; vtable 0x0128BDBC slot 13, shared with
// the vtable at 0x0128BE38 - 0x38). Mangled ?InitStateGroup@CAIStateGroup_LightHomeCandle@@MAEXABVCWorld@@AAVCThingAICreature@@@Z.
// Every AI state group gets this hook when its brain first builds the group for a creature; most siblings register
// with the idle scheduler or cache a component here. The candle-lighting routine finds everything it needs at run
// time, so the hook is an empty override that only exists to satisfy the base class.
// Matching: a protected virtual (`MAE` in the mangled name) with two reference parameters and an empty body is the
// whole function; slots 0-12 are placeholders so the override lands in slot 13.
class CWorld;
class CThingAICreature;

class CAIStateGroup_LightHomeCandle {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);   // slot 13 (+0x34)
};

void CAIStateGroup_LightHomeCandle::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
}
