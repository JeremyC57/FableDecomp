# Naming evidence

- Ego_r 0x009EEE10 and FableWin 0x02D739D0 give the exact leaf `??0CEngineShadowRenderer@@QAE@XZ`.
- Ego_d records no named locals. Retail shows a direct constructor chain and grouped default assignments followed by InitialiseTextures.
- The known CEngineComponent base, nested CShadowClipVolume layout, FableWin constructor and native numeric stores corroborate the class and fields. The called InitialiseTextures uses the existing sibling identity.

## Behavioral fixture

The fixture constructs the selected CEngineShadowRenderer body. Its renderer and allocation substitutes capture initialization and the texture-allocation call.
