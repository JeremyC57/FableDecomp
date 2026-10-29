// CAIStateGroupBase::PeekPlayerManager() const @ 0x008FCF80 -- *PBrain->PPlayerManager (Ego_r accessor order IsRunnableMain .. IsStopping)
class CPlayerManager;
struct I008fcf80 { char pad[0x20]; CPlayerManager* f; };
// Leaf: PeekPlayerManager
class CAIStateGroupBase { public: char pad[0x4]; I008fcf80* p; protected: const CPlayerManager& PeekPlayerManager() const; };
const CPlayerManager& CAIStateGroupBase::PeekPlayerManager() const { return *this->p->f; }