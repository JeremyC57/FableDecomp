# Pass21 owner clarification

The owner confirmed that the prohibition concerns the C++ `register` keyword, not an identifier that merely contains the same letters. Original identifiers such as `RegisterTimer` and `DeregisterCombatant` remain allowed and must retain their proven spelling. Source checks must distinguish tokens from substrings for this rule.

All other current pass21 rules remain applicable. Work is local; no Git commits or pushes are authorized.

## Sprite inline-control approval
Owner replied "you can" to the scoped request for row00afcc30: guarded scoped inline-depth controls, then FABLE_FORCEINLINE on the real CParticle::SetPrimitiveInfo if needed, within the Pass21 owner ladder. Approval is local/portable and does not authorize synthetic helpers or changes outside scope. By the time approval arrived, selected main_a12_visible_scale_dimensions already expanded all three native SetPrimitiveInfo sites using /d2inlT1537 /d2inlS15. No additional device is currently necessary; earlier automatic rejection remains historical, not a current blocker.
