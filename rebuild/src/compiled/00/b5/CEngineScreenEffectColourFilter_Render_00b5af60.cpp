// CEngineScreenEffectColourFilter::Render  0x00b5af60
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
class CEngineComponentRenderParams;

class CEngineScreenEffectColourFilter { public:
    char pad0[8];
    unsigned char enabled;   // +8
    char pad9[7];
    float amount;            // +0x10
};

// the global threshold constant compared against (0x129ba3c)
extern const float DELTA_NOTIONAL_ZERO;

// member helper at 0xb5a3f0 (this-call)
void __fastcall CEngineScreenEffectColourFilter_DoRender(CEngineScreenEffectColourFilter* self);

void __fastcall Render(CEngineScreenEffectColourFilter* self, int edx_dummy, CEngineComponentRenderParams const& params)
{
    (void)edx_dummy;
    if (self->enabled) {
        if (self->amount > DELTA_NOTIONAL_ZERO) {
            CEngineScreenEffectColourFilter_DoRender(self);
        }
    }
}