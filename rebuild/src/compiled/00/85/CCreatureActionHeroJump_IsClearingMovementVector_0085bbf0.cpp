// Leaf: ?IsClearingMovementVector@CCreatureActionHeroJump@@UBE_NXZ
class CCreatureActionHeroJump { public: virtual bool IsClearingMovementVector() const; };
bool CCreatureActionHeroJump::IsClearingMovementVector() const
{
    return false;
}