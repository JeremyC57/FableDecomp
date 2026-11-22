# NParticleEngine::CPSCLight::PreparePrimitives

All 2,516 bytes and 675 instructions match. All 70 primary operands pass, as do the two mapped owners and the 108-byte primitive constructor, 233-byte colour clamp and 132-byte interpolation. The 4,608-case RTM fixture runs the submitted function with shared colour arithmetic and checks light state, byte channels, world/spot radius, and transformed position.

The debug line table distinguishes normalised_time for colour from anim_percent for the upper-bounded radius fraction. Each byte-colour assignment calls Initialise with four byte channels. The midpoint uses the original conversion operator directly. The visible shared ScaleParticleDimensions body fixes the four-byte frame difference. The visible primitive light constructor fixes temporary placement. The last four reordered instructions disappear when the attenuation expression is grouped.
