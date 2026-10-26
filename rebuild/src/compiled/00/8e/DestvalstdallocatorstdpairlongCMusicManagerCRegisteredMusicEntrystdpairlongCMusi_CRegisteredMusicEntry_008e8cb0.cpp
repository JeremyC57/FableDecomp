class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
struct Sub114 { void teardown(); };
// Leaf: CRegisteredMusicEntry>_>
struct Owner { char pad[0x114]; Sub114 sub; };

struct Host {
  char pad[0x114];
  Sub114 sub;
  void DestVal_114_B();
};

void __fastcall DestVal_114_B(Host* self) {
  self->sub.teardown();
  ((CCombatActionBase*)((Owner*)self))->CCombatActionBase::~CCombatActionBase();
}