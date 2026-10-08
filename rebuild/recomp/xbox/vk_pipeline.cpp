// Vulkan renderer, part 2: render passes, vertex fetch and primitive assembly, shaders
// (generated GLSL compiled with glslang), pipelines, textures and samplers.
#include "vk_renderer.hpp"

#include "nv2a_methods.h"
#include "nv2a_shaders.hpp"
#include "pc_textures.hpp"
#include "nv2a_texture.hpp"
#include "settings.hpp"
#include "xhost.hpp"

#include "../posix/third_party/stb_image_write.h"

#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#if __has_include(<glslang/SPIRV/GlslangToSpv.h>)
#include <glslang/SPIRV/GlslangToSpv.h>
#else
#include <SPIRV/GlslangToSpv.h>  // glslang built in-tree
#endif

#include <algorithm>
#include <atomic>
#include <cmath>
#include <unistd.h>
#include <unordered_map>
#include <vector>

namespace xb::gpu {
extern uint64_t g_frameCount;

using namespace vk;
extern std::atomic<uint32_t> g_vblanks;

namespace {
// Texture formats that read a depth surface (Z24: 0x2A/0x2B swizzled, 0x2E/0x2F linear;
// Z16: 0x2C/0x2D, 0x30/0x31).
bool isDepthFormat(uint32_t color) { return color >= 0x2A && color <= 0x31; }

// Generated fragment shader sources by key (captures write the ones they use to psh_<key>.frag).
uint64_t g_lastFragKey = 0;
float g_uiClass[5];  // last draw's screen-space classification (flat, effect, max z, min x, max x) for captures  // the fragment shader of the draw being recorded
std::unordered_map<uint64_t, std::string>& fragSources() {
    static std::unordered_map<uint64_t, std::string> m;
    return m;
}


uint64_t fnv(const void* p, size_t n, uint64_t h = 1469598103934665603ull) {
    const uint8_t* b = static_cast<const uint8_t*>(p);
    for (size_t i = 0; i < n; ++i) h = (h ^ b[i]) * 1099511628211ull;
    return h;
}
uint64_t mix(uint64_t h, uint64_t v) { return fnv(&v, 8, h); }

// Content hash for texture change detection: 16 bytes per step (FNV byte by byte was the
// renderer's top CPU cost).
uint64_t fastHash(const uint8_t* p, size_t n) {
    uint64_t h = 0x9E3779B97F4A7C15ull ^ n, h2 = 0xC2B2AE3D27D4EB4Full;
    size_t i = 0;
    for (; i + 16 <= n; i += 16) {
        uint64_t a, b;
        std::memcpy(&a, p + i, 8);
        std::memcpy(&b, p + i + 8, 8);
        h = (h ^ a) * 0x100000001B3ull;
        h2 = (h2 ^ b) * 0x9FB21C651E98DF25ull;
    }
    for (; i < n; ++i) h = (h ^ p[i]) * 0x100000001B3ull;
    return h ^ (h2 >> 1) ^ (h2 << 31);
}

// GL enums the Kelvin class uses for blend/depth/stencil state.
VkBlendFactor blendFactor(uint32_t gl) {
    switch (gl) {
    case 0x0000: return VK_BLEND_FACTOR_ZERO;
    case 0x0001: return VK_BLEND_FACTOR_ONE;
    case 0x0300: return VK_BLEND_FACTOR_SRC_COLOR;
    case 0x0301: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case 0x0302: return VK_BLEND_FACTOR_SRC_ALPHA;
    case 0x0303: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case 0x0304: return VK_BLEND_FACTOR_DST_ALPHA;
    case 0x0305: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case 0x0306: return VK_BLEND_FACTOR_DST_COLOR;
    case 0x0307: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case 0x0308: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    case 0x8001: return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case 0x8002: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case 0x8003: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case 0x8004: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    default: return VK_BLEND_FACTOR_ONE;
    }
}
VkBlendOp blendOp(uint32_t gl) {
    switch (gl) {
    case 0x800A: return VK_BLEND_OP_SUBTRACT;
    case 0x800B: return VK_BLEND_OP_REVERSE_SUBTRACT;
    case 0x8007: return VK_BLEND_OP_MIN;
    case 0x8008: return VK_BLEND_OP_MAX;
    default: return VK_BLEND_OP_ADD;
    }
}
VkCompareOp compareOp(uint32_t gl) { return static_cast<VkCompareOp>((gl - 0x200) & 7); }  // GL_NEVER.. order matches
VkStencilOp stencilOp(uint32_t gl) {
    switch (gl) {
    case 0x0000: return VK_STENCIL_OP_ZERO;
    case 0x1E00: return VK_STENCIL_OP_KEEP;
    case 0x1E01: return VK_STENCIL_OP_REPLACE;
    case 0x1E02: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case 0x1E03: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    case 0x150A: return VK_STENCIL_OP_INVERT;
    case 0x8507: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case 0x8508: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    default: return VK_STENCIL_OP_KEEP;
    }
}

// Kelvin vertex attribute format -> Vulkan format (type in bits 0-3, size in bits 4-7).
VkFormat attribFormat(uint32_t fmt) {
    const uint32_t type = fmt & 0xF, size = (fmt >> 4) & 0xF;
    switch (type) {
    case 0:  // UB_D3D: D3DCOLOR (BGRA bytes)
        return size == 4 ? VK_FORMAT_B8G8R8A8_UNORM : size == 3 ? VK_FORMAT_B8G8R8_UNORM : size == 2 ? VK_FORMAT_R8G8_UNORM : VK_FORMAT_R8_UNORM;
    case 4:  // UB_OGL
        return size == 4 ? VK_FORMAT_R8G8B8A8_UNORM : size == 3 ? VK_FORMAT_R8G8B8_UNORM : size == 2 ? VK_FORMAT_R8G8_UNORM : VK_FORMAT_R8_UNORM;
    case 1:  // S1: normalized shorts
        return size == 4 ? VK_FORMAT_R16G16B16A16_SNORM : size == 3 ? VK_FORMAT_R16G16B16_SNORM : size == 2 ? VK_FORMAT_R16G16_SNORM : VK_FORMAT_R16_SNORM;
    case 5:  // S32K: integer shorts
        return size == 4 ? VK_FORMAT_R16G16B16A16_SSCALED : size == 3 ? VK_FORMAT_R16G16B16_SSCALED : size == 2 ? VK_FORMAT_R16G16_SSCALED : VK_FORMAT_R16_SSCALED;
    case 6:  // CMP: packed 11:11:10, decoded in the shader
        return VK_FORMAT_R32_UINT;
    default:  // F
        return size == 4 ? VK_FORMAT_R32G32B32A32_SFLOAT : size == 3 ? VK_FORMAT_R32G32B32_SFLOAT : size == 2 ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32_SFLOAT;
    }
}
uint32_t attribBytes(uint32_t fmt) {
    const uint32_t type = fmt & 0xF, size = (fmt >> 4) & 0xF;
    switch (type) {
    case 0: case 4: return size;
    case 1: case 5: return 2 * size;
    case 6: return 4;
    default: return 4 * size;
    }
}

// Vertex formats: 3-component 8/16-bit and scaled formats are optional in Vulkan and most
// phone GPUs lack them. Attributes in a format the device cannot fetch are converted to
// 32-bit floats (always supported) on upload. FABLE_VERTEX_FLOAT=1 forces that, for testing.
bool vertexFormatSupported(VkFormat f) {
    static const bool force = getenv("FABLE_VERTEX_FLOAT") != nullptr;
    static std::unordered_map<int, bool> cache;
    const bool isFloat = f == VK_FORMAT_R32_SFLOAT || f == VK_FORMAT_R32G32_SFLOAT || f == VK_FORMAT_R32G32B32_SFLOAT ||
                         f == VK_FORMAT_R32G32B32A32_SFLOAT || f == VK_FORMAT_R32_UINT;
    if (isFloat) return true;
    if (force) return false;
    auto it = cache.find(f);
    if (it != cache.end()) return it->second;
    VkFormatProperties fp;
    vkGetPhysicalDeviceFormatProperties(ctx().phys, f, &fp);
    const bool ok = (fp.bufferFeatures & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) != 0;
    return cache[f] = ok;
}

// n vertices of a Kelvin attribute (UB_D3D, UB_OGL, S1 or S32K) as floats, same component count.
void attribToFloat(const uint8_t* src, uint32_t stride, uint32_t n, uint32_t fmt, std::vector<float>& out) {
    const uint32_t type = fmt & 0xF, size = (fmt >> 4) & 0xF;
    out.resize(static_cast<size_t>(n) * size);
    float* o = out.data();
    for (uint32_t v = 0; v < n; ++v, src += stride)
        for (uint32_t c = 0; c < size; ++c) {
            switch (type) {
            case 0: {  // D3DCOLOR: BGRA bytes, x = red (size 1, 2: R8, R8G8 order)
                const uint32_t b = size >= 3 ? (c < 3 ? 2 - c : 3) : c;
                *o++ = src[b] / 255.0f;
                break;
            }
            case 4: *o++ = src[c] / 255.0f; break;
            case 1: {
                int16_t x;
                std::memcpy(&x, src + 2 * c, 2);
                *o++ = std::max(x / 32767.0f, -1.0f);
                break;
            }
            default: {  // 5: S32K
                int16_t x;
                std::memcpy(&x, src + 2 * c, 2);
                *o++ = static_cast<float>(x);
                break;
            }
            }
        }
}

bool g_glslangReady = false;

// Per-60-frame draw statistics (XBOX_LOG >= 1), logged from endFrame.
struct DrawStats {
    uint64_t draws = 0, verts = 0, garbage = 0, oob = 0, nopass = 0, nopipe = 0, noset = 0, textured = 0, program = 0;
    uint32_t target = 0, w = 0, h = 0, fmt = 0;
} g_ds;
}  // namespace

void logDrawStats(uint64_t frame) {
    XLOG(1, "Vulkan: frame %llu: %llu draws (%llu verts, %llu textured, %llu programmable); skipped: garbage %llu, "
            "out of range %llu, no pass %llu, no pipeline %llu, no set %llu; target 0x%08X %ux%u fmt 0x%X",
         static_cast<unsigned long long>(frame), static_cast<unsigned long long>(g_ds.draws), static_cast<unsigned long long>(g_ds.verts),
         static_cast<unsigned long long>(g_ds.textured), static_cast<unsigned long long>(g_ds.program),
         static_cast<unsigned long long>(g_ds.garbage), static_cast<unsigned long long>(g_ds.oob), static_cast<unsigned long long>(g_ds.nopass),
         static_cast<unsigned long long>(g_ds.nopipe), static_cast<unsigned long long>(g_ds.noset), g_ds.target, g_ds.w, g_ds.h, g_ds.fmt);
    g_ds = {};
}
namespace {

std::vector<uint32_t> compileGlsl(EShLanguage stage, const std::string& src) {
    if (getenv("FABLE_DUMP_SHADERS")) {  // each generated shader to shader_NNNN.{vert,frag}
        static int n = 0;
        char name[64];
        snprintf(name, sizeof name, "shader_%04d.%s", n++, stage == EShLangVertex ? "vert" : "frag");
        if (FILE* f = fopen(name, "w")) {
            fputs(src.c_str(), f);
            fclose(f);
        }
    }
    // SPIR-V cache on disk (FABLE_CACHE_DIR): glslang is slow, a shader seen before loads instantly.
    static const char* cacheDir = featureOff("diskcache") ? nullptr : getenv("FABLE_CACHE_DIR");
    std::string cacheFile;
    if (cacheDir && *cacheDir) {
        char name[40];
        snprintf(name, sizeof name, "/%016llx.%s.spv", static_cast<unsigned long long>(fastHash(reinterpret_cast<const uint8_t*>(src.data()), src.size())),
                 stage == EShLangVertex ? "v" : "f");
        cacheFile = std::string(cacheDir) + name;
        if (FILE* f = fopen(cacheFile.c_str(), "rb")) {
            std::vector<uint32_t> spv;
            uint32_t w[1024];
            for (size_t n; (n = fread(w, 4, 1024, f)) > 0;) spv.insert(spv.end(), w, w + n);
            fclose(f);
            if (spv.size() > 5 && spv[0] == 0x07230203u) return spv;
        }
    }
    if (!g_glslangReady) {
        glslang::InitializeProcess();
        g_glslangReady = true;
    }
    glslang::TShader sh(stage);
    const char* text = src.c_str();
    sh.setStrings(&text, 1);
    sh.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    sh.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1);
    sh.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);
    if (!sh.parse(GetDefaultResources(), 450, false, EShMsgDefault)) {
        XLOG(0, "GLSL compile failed: %s\n%s", sh.getInfoLog(), src.c_str());
        return {};
    }
    glslang::TProgram prog;
    prog.addShader(&sh);
    if (!prog.link(EShMsgDefault)) {
        XLOG(0, "GLSL link failed: %s", prog.getInfoLog());
        return {};
    }
    std::vector<uint32_t> spv;
    glslang::GlslangToSpv(*prog.getIntermediate(stage), spv);
    if (!cacheFile.empty()) {
        const std::string tmp = cacheFile + ".tmp";
        if (FILE* f = fopen(tmp.c_str(), "wb")) {
            const bool ok = fwrite(spv.data(), 4, spv.size(), f) == spv.size();
            fclose(f);
            if (ok) rename(tmp.c_str(), cacheFile.c_str());
        }
    }
    return spv;
}

}  // namespace

void VkRenderer::vblank() { ++g_vblanks; }

void VkRenderer::savePipelineCache() {
    const char* dir = featureOff("diskcache") ? nullptr : getenv("FABLE_CACHE_DIR");
    if (!dir || !pipeCache_ || !pipeCacheDirty_) return;
    pipeCacheDirty_ = false;
    size_t n = 0;
    if (vkGetPipelineCacheData(ctx().device, pipeCache_, &n, nullptr) != VK_SUCCESS || !n) return;
    std::vector<char> data(n);
    if (vkGetPipelineCacheData(ctx().device, pipeCache_, &n, data.data()) != VK_SUCCESS) return;
    const std::string path = std::string(dir) + "/pipelines.bin", tmp = path + ".tmp";
    if (FILE* f = fopen(tmp.c_str(), "wb")) {
        const bool ok = fwrite(data.data(), 1, n, f) == n;
        fclose(f);
        if (ok) rename(tmp.c_str(), path.c_str());
    }
}

// ============================================================================================
// pipeline objects
// ============================================================================================
void VkRenderer::initPipelineObjects() {
    VkDevice dev = ctx().device;
    {  // The driver's pipeline cache, kept across runs in FABLE_CACHE_DIR (see savePipelineCache).
        std::vector<char> data;
        const char* dir = featureOff("diskcache") ? nullptr : getenv("FABLE_CACHE_DIR");
        if (dir)
            if (FILE* f = fopen((std::string(dir) + "/pipelines.bin").c_str(), "rb")) {
                char buf[65536];
                for (size_t n; (n = fread(buf, 1, sizeof buf, f)) > 0;) data.insert(data.end(), buf, buf + n);
                fclose(f);
            }
        VkPipelineCacheCreateInfo ci{VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO};
        ci.initialDataSize = data.size();
        ci.pInitialData = data.empty() ? nullptr : data.data();
        if (vkCreatePipelineCache(dev, &ci, nullptr, &pipeCache_) != VK_SUCCESS) {
            ci.initialDataSize = 0;  // data from another driver
            ci.pInitialData = nullptr;
            vkCreatePipelineCache(dev, &ci, nullptr, &pipeCache_);
        }
    }
    std::vector<VkDescriptorSetLayoutBinding> b(14);
    for (uint32_t i = 0; i < 14; ++i) {
        b[i].binding = i;
        b[i].descriptorType = i < 2 ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        b[i].descriptorCount = 1;
        b[i].stageFlags = i == 0 ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    VkDescriptorSetLayoutCreateInfo lci{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    lci.bindingCount = static_cast<uint32_t>(b.size());
    lci.pBindings = b.data();
    vkCreateDescriptorSetLayout(dev, &lci, nullptr, &setLayout_);
    VkPipelineLayoutCreateInfo pci{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pci.setLayoutCount = 1;
    pci.pSetLayouts = &setLayout_;
    vkCreatePipelineLayout(dev, &pci, nullptr, &pipeLayout_);

    // Dummy textures for unused sampler bindings (opaque white).
    auto makeImage = [&](VkImageType type, VkImageCreateFlags flags, uint32_t layers, VkImage* img) {
        VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        ici.flags = flags;
        ici.imageType = type;
        ici.format = VK_FORMAT_B8G8R8A8_UNORM;
        ici.extent = {1, 1, 1};
        ici.mipLevels = 1;
        ici.arrayLayers = layers;
        ici.samples = VK_SAMPLE_COUNT_1_BIT;
        ici.tiling = VK_IMAGE_TILING_OPTIMAL;
        ici.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        vkCreateImage(dev, &ici, nullptr, img);
        VkMemoryRequirements req;
        vkGetImageMemoryRequirements(dev, *img, &req);
        VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        mai.allocationSize = req.size;
        mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        VkDeviceMemory mem;
        vkAllocateMemory(dev, &mai, nullptr, &mem);
        vkBindImageMemory(dev, *img, mem, 0);
        VkImageMemoryBarrier br{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        br.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        br.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        br.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        br.srcQueueFamilyIndex = br.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        br.image = *img;
        br.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, layers};
        vkCmdPipelineBarrier(cmd(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &br);
        VkClearColorValue white{{1, 1, 1, 1}};
        vkCmdClearColorImage(cmd(), *img, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &white, 1, &br.subresourceRange);
        br.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        br.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        br.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        br.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        vkCmdPipelineBarrier(cmd(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &br);
    };
    makeImage(VK_IMAGE_TYPE_2D, 0, 1, &dummyImage_);
    makeImage(VK_IMAGE_TYPE_2D, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, 6, &dummyCubeImage_);
    makeImage(VK_IMAGE_TYPE_3D, 0, 1, &dummy3DImage_);
    auto view = [&](VkImage img, VkImageViewType t, uint32_t layers) {
        VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        vci.image = img;
        vci.viewType = t;
        vci.format = VK_FORMAT_B8G8R8A8_UNORM;
        vci.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, layers};
        VkImageView v;
        vkCreateImageView(dev, &vci, nullptr, &v);
        return v;
    };
    dummy2D_ = view(dummyImage_, VK_IMAGE_VIEW_TYPE_2D, 1);
    dummyCube_ = view(dummyCubeImage_, VK_IMAGE_VIEW_TYPE_CUBE, 6);
    dummy3D_ = view(dummy3DImage_, VK_IMAGE_VIEW_TYPE_3D, 1);
}

// ============================================================================================
// render passes
// ============================================================================================
bool syncTrace();

void VkRenderer::endPass() {
    if (!pass_) return;
    if (syncTrace()) printf("SYNC end pass cb %p\n", static_cast<void*>(cmd())), fflush(stdout);
    vkCmdEndRenderPass(cmd());
    pass_ = VK_NULL_HANDLE;
}

void VkRenderer::beginPass() {
    Surface* c = target_.color ? findSurface(target_.color, false) : nullptr;
    Surface* z = target_.depth ? findSurface(target_.depth, true) : nullptr;
    if (!c && !z) return;
    if (c && z && (c->w != z->w || c->h != z->h)) z = nullptr;  // mismatched sizes: draw without depth
    const VkImage ci = c ? c->image : VK_NULL_HANDLE, zi = z ? z->image : VK_NULL_HANDLE;
    if (pass_ && passFb_ == framebuffers_[FbKey{ci, zi}]) return;
    endPass();
    if (c) transition(*c, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    if (z) transition(*z, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    VkDevice dev = ctx().device;
    const uint64_t rpKey = (static_cast<uint64_t>(c ? c->format : 0) << 32) | (z ? z->format : 0);
    VkRenderPass& rp = renderPasses_[rpKey];
    if (!rp) {
        VkAttachmentDescription att[2]{};
        VkAttachmentReference cref{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}, zref{c ? 1u : 0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
        uint32_t n = 0;
        if (c) {
            att[n].format = c->format;
            att[n].samples = VK_SAMPLE_COUNT_1_BIT;
            att[n].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            att[n].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            att[n].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            att[n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            att[n].initialLayout = att[n].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            ++n;
        }
        if (z) {
            att[n].format = z->format;
            att[n].samples = VK_SAMPLE_COUNT_1_BIT;
            att[n].loadOp = att[n].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            att[n].storeOp = att[n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            att[n].initialLayout = att[n].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            ++n;
        }
        VkSubpassDescription sub{};
        sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        sub.colorAttachmentCount = c ? 1 : 0;
        sub.pColorAttachments = c ? &cref : nullptr;
        sub.pDepthStencilAttachment = z ? &zref : nullptr;
        VkRenderPassCreateInfo rci{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
        rci.attachmentCount = n;
        rci.pAttachments = att;
        rci.subpassCount = 1;
        rci.pSubpasses = &sub;
        vkCreateRenderPass(dev, &rci, nullptr, &rp);
    }
    VkFramebuffer& fb = framebuffers_[FbKey{ci, zi}];
    if (!fb) {
        VkImageView views[2];
        uint32_t n = 0;
        if (c) views[n++] = c->view;
        if (z) views[n++] = z->view;
        VkFramebufferCreateInfo fci{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
        fci.renderPass = rp;
        fci.attachmentCount = n;
        fci.pAttachments = views;
        fci.width = c ? hostW(c->w, c->h) : hostW(z->w, z->h);
        fci.height = hostH(c ? c->h : z->h);
        fci.layers = 1;
        vkCreateFramebuffer(dev, &fci, nullptr, &fb);
    }
    VkRenderPassBeginInfo bi{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    bi.renderPass = rp;
    bi.framebuffer = fb;
    bi.renderArea.extent = {c ? hostW(c->w, c->h) : hostW(z->w, z->h), hostH(c ? c->h : z->h)};
    vkCmdBeginRenderPass(cmd(), &bi, VK_SUBPASS_CONTENTS_INLINE);
    if (syncTrace())
        printf("SYNC pass C %08X Z %08X %ux%u cb %p\n", c ? c->addr : 0, z ? z->addr : 0, bi.renderArea.extent.width, bi.renderArea.extent.height,
               static_cast<void*>(cmd())), fflush(stdout);
    ++g_perfPasses;
    pass_ = rp;
    passRp_ = rp;
    passFb_ = fb;
}

// 16:9 interface anchoring (see vk_renderer.hpp). A draw takes the anchor of last frame's cluster
// it lies in (HUD layouts persist from frame to frame), else one from its own extent.
namespace {
constexpr float kUiMargin = 12.0f;  // pieces this close belong to the same element
float anchorOfExtent(float x0, float x1) {
    if (x1 <= 330.0f && x0 < 213.0f) return -1.0f;
    if (x0 >= 310.0f && x1 > 427.0f) return 1.0f;
    return 0.0f;
}
}  // namespace

// A HUD piece: the same texture at the same place (4-pixel grid) frame after frame.
uint64_t VkRenderer::uiPieceKey(uint64_t tex, float x0, float x1, float y0, float y1) {
    auto q = [](float v) { return static_cast<uint64_t>(static_cast<int64_t>(std::floor(v / 4.0f)) & 0xFFF); };
    return (tex * 0x9E3779B97F4A7C15ull) ^ (q(x0) | q(x1) << 12 | q(y0) << 24 | q(y1) << 36);
}

float VkRenderer::uiAnchor(float x0, float x1, float y0, float y1, bool panel, uint64_t tex) {
    const uint64_t piece = uiPieceKey(tex, x0, x1, y0, y1);
    // Learned HUD pieces stay at their screen edge whatever else the frame draws (location
    // banners, glare, fades, menus over the HUD): see uiEndFrame.
    if (const auto it = uiHud_.find(piece); it != uiHud_.end() && it->second.sightings >= kHudSightings) {
        uiCur_.push_back({x0, x1, y0, y1, it->second.anchor, piece});
        return it->second.anchor;
    }
    const float a = uiAnchorLayout(x0, x1, y0, y1, panel);
    uiCur_.push_back({x0, x1, y0, y1, a, piece});
    return a;
}

float VkRenderer::uiAnchorLayout(float x0, float x1, float y0, float y1, bool panel) {
    // Menus (a full-screen panel) and cinematics (letterbox bars: screen-wide strips at the top or
    // bottom edge) keep their whole interface centred: subtitles and prompts stay together.
    // (Bars slid off screen after a conversation are still drawn: only visible ones count.)
    // Letterboxing is a pair of bars (a single screen-wide strip at one edge is a banner, such as
    // the location name at a region exit).
    if (x1 - x0 >= 560.0f && std::min(y1, 480.0f) - std::max(y0, 0.0f) >= 8.0f) {
        if (y0 <= 4.0f) uiBarTop_ = true;
        if (y1 >= 476.0f) uiBarBottom_ = true;
    }
    // FABLE_UI_FORCE_MENU=<flip> (debugging): every frame from that flip on is a menu frame.
    static const uint64_t forceMenu = getenv("FABLE_UI_FORCE_MENU") ? strtoull(getenv("FABLE_UI_FORCE_MENU"), nullptr, 10) : ~0ull;
    if (g_frameCount >= forceMenu) panel = true;
    const bool letterbox = uiBarTop_ && uiBarBottom_;
    if ((panel || letterbox) && !uiMenuCur_ && !uiMenuPrev_) {
        static int logged = 0;  // what switches the interface to its centred 4:3 layout
        if (logged++ < 40)
            XLOG(1, "UI: %s (x %.0f..%.0f, y %.0f..%.0f) at flip %llu: interface centred", panel ? "full-screen panel" : "letterbox bars", x0, x1, y0, y1,
                 static_cast<unsigned long long>(g_frameCount));
    }
    if (panel || letterbox) uiMenuCur_ = true;
    if (uiMenuCur_) return 0.0f;
    // Pieces seen last frame keep last frame's placement (centred if it was a menu or cinematic
    // frame: the panel or bars may come later in this frame); new pieces, such as the HUD coming
    // back after a conversation, are placed by their own extent.
    const float cx = (x0 + x1) * 0.5f, cy = (y0 + y1) * 0.5f;
    for (const UiRect& r : uiPrev_)
        if (cx >= r.x0 && cx <= r.x1 && cy >= r.y0 && cy <= r.y1) return uiMenuPrev_ ? 0.0f : r.anchor;
    return anchorOfExtent(x0, x1);
}

void VkRenderer::uiEndFrame() {
    // Union of overlapping / nearly touching rectangles (a few hundred per frame at most).
    if (uiCur_.empty()) {  // a frame without interface draws (repeated presents): keep the layout
        uiMenuCur_ = uiBarTop_ = uiBarBottom_ = false;
        return;
    }
    std::vector<UiRect> cl;
    for (const UiRect& r : uiCur_) {
        if (r.x1 < 0.0f || r.x0 > 640.0f) continue;  // parked off screen
        // Screen-wide strips (cutscene / conversation letterbox bars, banners) are centred on
        // their own and must not pull the HUD pieces they touch into one centred cluster.
        if (r.x1 - r.x0 >= 560.0f) continue;
        cl.push_back(r);
    }
    for (bool merged = true; merged;) {
        merged = false;
        for (size_t i = 0; i < cl.size(); ++i)
            for (size_t j = i + 1; j < cl.size(); ++j) {
                UiRect& a = cl[i];
                const UiRect& b = cl[j];
                if (b.x0 > a.x1 + kUiMargin || a.x0 > b.x1 + kUiMargin || b.y0 > a.y1 + kUiMargin || a.y0 > b.y1 + kUiMargin) continue;
                a = {std::min(a.x0, b.x0), std::max(a.x1, b.x1), std::min(a.y0, b.y0), std::max(a.y1, b.y1), 0.0f};
                cl[j] = cl.back();
                cl.pop_back();
                --j;
                merged = true;
            }
    }
    for (UiRect& r : cl) r.anchor = anchorOfExtent(r.x0, r.x1);
    // HUD learning: in plain gameplay frames (no menu panel or letterboxing, this frame or the
    // last), a piece placed at a screen edge counts a sighting; after kHudSightings it is a HUD
    // piece and keeps that edge from then on.
    if (!uiMenuCur_ && !uiMenuPrev_) {
        if (uiHud_.size() > 8192) uiHud_.clear();
        for (const UiRect& r : uiCur_) {
            if (r.anchor == 0.0f || r.x1 - r.x0 >= 560.0f) continue;
            HudPiece& h = uiHud_[r.piece];
            if (h.sightings > 0 && h.anchor != r.anchor) h.sightings = 0;
            h.anchor = r.anchor;
            if (h.sightings < kHudSightings) ++h.sightings;
        }
    }
    if (getenv("FABLE_UI_LOG")) {  // debugging: layout changes
        static std::string last;
        std::string cur = uiMenuCur_ ? "menu" : "hud";
        char b[64];
        for (const UiRect& r : cl) {
            std::snprintf(b, sizeof b, " %.0f..%.0f:%+.0f", r.x0, r.x1, r.anchor);
            cur += b;
        }
        if (cur != last) {
            XLOG(0, "UI layout (frame %llu): %s", static_cast<unsigned long long>(g_frameCount), cur.c_str());
            std::string raw;
            for (const UiRect& r : uiCur_) {
                std::snprintf(b, sizeof b, " [%.0f..%.0f,%.0f..%.0f]", r.x0, r.x1, r.y0, r.y1);
                raw += b;
            }
            XLOG(0, "UI rects: %s", raw.c_str());
        }
        last = cur;
    }
    uiPrev_ = std::move(cl);
    uiMenuPrev_ = uiMenuCur_;
    uiMenuCur_ = uiBarTop_ = uiBarBottom_ = false;
    uiCur_.clear();
}

// Side bars of a 16:9 frame beside a full-screen panel drawn into the centred 4:3 area.
void VkRenderer::fillSideBars(bool extendEdges) {
    Surface* s = target_.color ? findSurface(target_.color, false) : nullptr;
    if (!s) return;
    const int32_t w = static_cast<int32_t>(hostW(s->w, s->h)), h = static_cast<int32_t>(hostH(s->h));
    const int32_t bar = w / 8;  // (16:9 width - 4:3 width) / 2
    if (bar < 2) return;
    if (!extendEdges) {
        beginPass();
        if (!pass_) return;
        VkClearAttachment ca{VK_IMAGE_ASPECT_COLOR_BIT, 0, {}};
        ca.clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        VkClearRect rects[2] = {{{{0, 0}, {static_cast<uint32_t>(bar), static_cast<uint32_t>(h)}}, 0, 1},
                                {{{w - bar, 0}, {static_cast<uint32_t>(bar), static_cast<uint32_t>(h)}}, 0, 1}};
        vkCmdClearAttachments(cmd(), 1, &ca, 2, rects);
        return;
    }
    // Opaque panels: the bars take the panel's own colour (black bars where the format cannot be
    // blitted).
    VkFormatProperties fp;
    vkGetPhysicalDeviceFormatProperties(ctx().phys, s->format, &fp);
    if ((fp.optimalTilingFeatures & (VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT)) != (VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT))
        return fillSideBars(false);
    endPass();
    transition(*s, VK_IMAGE_LAYOUT_GENERAL);
    // The game's full-screen quads start a pixel in (x and y from 1): the uncovered first
    // row and the columns at the panel's edge are filled the same way.
    const int32_t inset = 2 * std::max(2, (w + 639) / 640), insetY = 2 * std::max(1, (h + 479) / 480);
    VkImageBlit blits[3]{};
    for (int i = 0; i < 3; ++i) blits[i].srcSubresource = blits[i].dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    // One colour per bar, the panel's top corner on that side: the intro's white background,
    // the black letterbox of the story movies (their pictures reach the edges further down, so
    // stretching whole edge columns would smear them into the bars).
    for (int i = 0; i < 2; ++i) {
        VkImageBlit& b = blits[i];
        const int32_t col = i == 0 ? bar + inset : w - bar - inset - 1;
        b.srcOffsets[0] = {col, insetY, 0};
        b.srcOffsets[1] = {col + 1, insetY + 1, 1};
        b.dstOffsets[0] = {i == 0 ? 0 : col + 1, 0, 0};
        b.dstOffsets[1] = {i == 0 ? col : w, h, 1};
    }
    blits[2].srcOffsets[0] = {0, insetY, 0};
    blits[2].srcOffsets[1] = {w, insetY + 1, 1};
    blits[2].dstOffsets[0] = {0, 0, 0};
    blits[2].dstOffsets[1] = {w, insetY, 1};
    vkCmdBlitImage(cmd(), s->image, VK_IMAGE_LAYOUT_GENERAL, s->image, VK_IMAGE_LAYOUT_GENERAL, 2, blits, VK_FILTER_NEAREST);
    VkMemoryBarrier mb{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
    mb.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    mb.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
    vkCmdPipelineBarrier(cmd(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &mb, 0, nullptr, 0, nullptr);
    vkCmdBlitImage(cmd(), s->image, VK_IMAGE_LAYOUT_GENERAL, s->image, VK_IMAGE_LAYOUT_GENERAL, 1, &blits[2], VK_FILTER_NEAREST);
}

VkDeviceSize VkRenderer::upload(const void* data, VkDeviceSize bytes, VkDeviceSize align) {
    Frame& f = frames_[frame_];
    VkDeviceSize off = (f.uploadPos + align - 1) / align * align;
    if (off + bytes > f.upload.size) {  // ring full: finish this frame's work and start over
        submitFrame(true);
        return upload(data, bytes, align);
    }
    if (data) std::memcpy(static_cast<uint8_t*>(f.mapped) + off, data, bytes);
    f.uploadPos = off + bytes;
    return off;
}

// ============================================================================================
// primitive assembly (Begin/End batches)
// ============================================================================================
void VkRenderer::begin(uint32_t primitive) {
    primitive_ = primitive;
    indices_.clear();
    inline_.clear();
    arrays_.clear();
    immediate_.clear();
}

void VkRenderer::drawArrays(uint32_t start, uint32_t count) { arrays_.emplace_back(start, count); }

void VkRenderer::arrayElements(const uint32_t* w, uint32_t n, bool sixteen) {
    for (uint32_t i = 0; i < n; ++i) {
        if (sixteen) {
            indices_.push_back(w[i] & 0xFFFF);
            indices_.push_back(w[i] >> 16);
        } else {
            indices_.push_back(w[i]);
        }
    }
}

void VkRenderer::inlineArray(const uint32_t* w, uint32_t n) {
    const size_t at = inline_.size();
    inline_.resize(at + 4 * n);
    std::memcpy(&inline_[at], w, 4 * n);
}

void VkRenderer::vertexAttribute(uint32_t method, uint32_t value) {
    // SET_VERTEX_DATA*: current attribute values; writing the last component of slot 0
    // emits an immediate-mode vertex.
    float v;
    std::memcpy(&v, &value, 4);
    int slot = -1;
    bool emit = false;
    if (method >= 0x1A00 && method < 0x1B00) {  // 4F_M
        slot = static_cast<int>((method - 0x1A00) / 16);
        const int comp = static_cast<int>((method - 0x1A00) / 4 % 4);
        attrib_[slot][comp] = v;
        emit = slot == 0 && comp == 3;
    } else if (method >= 0x1880 && method < 0x1900) {  // 2F_M
        slot = static_cast<int>((method - 0x1880) / 8);
        const int comp = static_cast<int>((method - 0x1880) / 4 % 2);
        attrib_[slot][comp] = v;
        if (comp == 1) { attrib_[slot][2] = 0; attrib_[slot][3] = 1; }
        emit = slot == 0 && comp == 1;
    } else if (method >= 0x1900 && method < 0x1940) {  // 2S
        slot = static_cast<int>((method - 0x1900) / 4);
        attrib_[slot][0] = static_cast<float>(static_cast<int16_t>(value));
        attrib_[slot][1] = static_cast<float>(static_cast<int16_t>(value >> 16));
        attrib_[slot][2] = 0;
        attrib_[slot][3] = 1;
        emit = slot == 0;
    } else if (method >= 0x1940 && method < 0x1980) {  // 4UB
        slot = static_cast<int>((method - 0x1940) / 4);
        for (int i = 0; i < 4; ++i) attrib_[slot][i] = ((value >> (8 * i)) & 0xFF) / 255.0f;
        emit = slot == 0;
    } else if (method >= 0x1980 && method < 0x1A00) {  // 4S_M
        slot = static_cast<int>((method - 0x1980) / 8);
        const int part = static_cast<int>((method - 0x1980) / 4 % 2);
        attrib_[slot][part * 2] = static_cast<float>(static_cast<int16_t>(value));
        attrib_[slot][part * 2 + 1] = static_cast<float>(static_cast<int16_t>(value >> 16));
        emit = slot == 0 && part == 1;
    }
    if (emit && primitive_) immediate_.insert(immediate_.end(), &attrib_[0][0], &attrib_[0][0] + 64);
}

void VkRenderer::end() {
    std::lock_guard<std::mutex> l(surfLock_);
    if (!arrays_.empty()) {
        for (const auto& [start, count] : arrays_) draw(nullptr, start, count, nullptr, 0);
    } else if (!indices_.empty()) {
        draw(&indices_, 0, static_cast<uint32_t>(indices_.size()), nullptr, 0);
    } else if (!inline_.empty()) {
        // Interleaved inline vertices: the enabled arrays' formats, in attribute order.
        uint32_t stride = 0;
        for (int i = 0; i < 16; ++i) {
            const uint32_t fmt = state().regs[NV097_SET_VERTEX_DATA_ARRAY_FORMAT / 4 + i];
            if ((fmt >> 4) & 0xF) stride += attribBytes(fmt);
        }
        if (stride) draw(nullptr, 0, static_cast<uint32_t>(inline_.size() / stride), inline_.data(), stride);
    } else if (!immediate_.empty()) {
        draw(nullptr, 0, static_cast<uint32_t>(immediate_.size() / 64), reinterpret_cast<const uint8_t*>(immediate_.data()), 0xFFFFFFFF);
    }
    primitive_ = 0;
}

// ============================================================================================
// shaders
// ============================================================================================
VkShaderModule VkRenderer::vertexShader(uint32_t attribMask, const uint32_t* fmts, uint64_t* keyOut) {
    const State& st = state();
    const uint32_t* R = st.regs;
    const bool program = (R[NV097_SET_TRANSFORM_EXECUTION_MODE / 4] & 3) == 2;
    // Keyed by the program's contents (from its start slot to the FINAL flag), not by upload
    // count or slot: the game re-uploads the same ~70 programs to different slots all the time.
    static uint64_t hashGen = 0, progHash = 0;
    static uint32_t hashStart = ~0u;
    const uint32_t start = R[NV097_SET_TRANSFORM_PROGRAM_START / 4];
    if (program && (hashGen != st.programGeneration || hashStart != start)) {
        hashGen = st.programGeneration;
        hashStart = start;
        progHash = 0x9E3779B97F4A7C15ull;
        for (uint32_t i = start; i < 136; ++i) {
            progHash = fnv(st.program[i], 16, progHash);
            if (st.program[i][3] & 1) break;  // FINAL
        }
    }
    uint64_t key = mix(1, program ? progHash : 0);
    for (int i = 0; i < 16; ++i) key = mix(key, (attribMask >> i) & 1 ? (fmts[i] & 0xF) == 6 : 2);
    if (!program) key = fnv(&R[NV097_SET_TEXTURE_MATRIX_ENABLE / 4], 16, mix(key, R[NV097_SET_LIGHTING_ENABLE / 4]));
    const bool fog = R[NV097_SET_FOG_ENABLE / 4] & 1;
    const uint32_t fogMode = R[NV097_SET_FOG_MODE / 4];
    key = mix(key, fog ? fogMode : 0);
    *keyOut = key;
    auto it = shaders_.find(key);
    if (it != shaders_.end()) return it->second;
    // invariant: a depth pre-pass and the colour pass after it use different vertex programs
    // with the same position math; without it the compiler may fuse/reorder differently per
    // shader, the depths differ slightly and the LEQUAL colour pass fails (targeting turned the
    // world into a white void). The NV2A computes them bit-identically.
    std::string src = "#version 450\ninvariant gl_Position;\n";
    for (int i = 0; i < 16; ++i) {
        const bool cmp = ((attribMask >> i) & 1) && (fmts[i] & 0xF) == 6;
        src += cmp ? "layout(location=" + std::to_string(i) + ") in uint vin" + std::to_string(i) + ";\n"
                   : "layout(location=" + std::to_string(i) + ") in vec4 vin" + std::to_string(i) + ";\n";
    }
    src += "layout(set=0, binding=0) uniform VC { vec4 c[192]; vec4 surface; vec4 clip; vec4 fog; vec4 ui; } cst;\n";
    src += "layout(location=0) out vec4 vD0; layout(location=1) out vec4 vD1; layout(location=2) out vec4 vB0;\n"
           "layout(location=3) out vec4 vB1; layout(location=4) out float vFog;\n"
           "layout(location=5) out vec4 vTex0; layout(location=6) out vec4 vTex1; layout(location=7) out vec4 vTex2;\n"
           "layout(location=8) out vec4 vTex3;\n";
    src += "vec4 oPos = vec4(0,0,0,1), oD0 = vec4(0,0,0,1), oD1 = vec4(0,0,0,1), oFog = vec4(0), oPts = vec4(1),\n"
           "     oB0 = vec4(0,0,0,1), oB1 = vec4(0,0,0,1), oT0 = vec4(0,0,0,1), oT1 = vec4(0,0,0,1), oT2 = vec4(0,0,0,1), oT3 = vec4(0,0,0,1);\n";
    src += vertexProgramPrelude();
    src += "vec4 cmp(uint v) { return vec4(float(int(v << 21) >> 21) / 1023.0, float(int(v << 10) >> 21) / 1023.0, float(int(v) >> 22) / 511.0, 1.0); }\n";
    src += "void main() {\n";
    for (int i = 0; i < 16; ++i) {
        const bool cmp = ((attribMask >> i) & 1) && (fmts[i] & 0xF) == 6;
        src += "  vec4 v" + std::to_string(i) + " = " + (cmp ? "cmp(vin" + std::to_string(i) + ")" : "vin" + std::to_string(i)) + ";\n";
    }
    if (program) {
        src += "  R0 = R1 = R2 = R3 = R4 = R5 = R6 = R7 = R8 = R9 = R10 = R11 = vec4(0.0);\n";
        src += translateVertexProgram(st.program, R[NV097_SET_TRANSFORM_PROGRAM_START / 4]);
    } else {
        // Fixed function: composite matrix (it includes the viewport), colours and texcoords
        // passed through (texture matrices applied when enabled). Lighting comes later.
        src += "  mat4 comp = mat4(cst.c[0], cst.c[1], cst.c[2], cst.c[3]);\n"
               "  oPos = v0 * comp;\n"
               "  if (oPos.w != 0.0) { oPos.xyz /= oPos.w; oPos.w = 1.0 / oPos.w; }\n"
               "  oD0 = v3; oD1 = v4; oB0 = v3; oB1 = v4; oFog = vec4(v5.x);\n"
               "  oT0 = v9; oT1 = v10; oT2 = v11; oT3 = v12;\n";
    }
    // Fog unit: the fog coordinate (oFog.x) becomes the fog factor, with the fog params
    // (NV097_SET_FOG_PARAMS x, y) per mode; 1 is no fog. Equations as in xemu (reference).
    if (getenv("FABLE_NOFOG")) {  // debugging
        src += "  oFog = vec4(0.0);\n";
    } else if (!fog) {
        src += "  oFog = vec4(1.0);\n";
    } else {
        src += "  float fogDistance = oFog.x, fogFactor;\n";
        switch (fogMode) {
        case NV097_SET_FOG_MODE_V_EXP: case NV097_SET_FOG_MODE_V_EXP_ABS:
            src += "  fogFactor = cst.fog.x + exp2(fogDistance * cst.fog.y * 16.0) - 1.5;\n";
            break;
        case NV097_SET_FOG_MODE_V_EXP2: case NV097_SET_FOG_MODE_V_EXP2_ABS:
            src += "  fogFactor = cst.fog.x + exp2(-fogDistance * fogDistance * cst.fog.y * cst.fog.y * 32.0) - 1.5;\n";
            break;
        default:  // linear
            src += "  fogFactor = cst.fog.x + fogDistance * cst.fog.y - 1.0;\n";
            break;
        }
        if (fogMode == NV097_SET_FOG_MODE_V_EXP_ABS || fogMode == NV097_SET_FOG_MODE_V_EXP2_ABS || fogMode == NV097_SET_FOG_MODE_V_LINEAR_ABS)
            src += "  fogFactor = abs(fogFactor);\n";
        src += "  oFog = vec4(isinf(fogDistance) ? 1.0 : (isnan(fogFactor) ? 1.0 : fogFactor));\n";
    }
    // Screen space -> Vulkan clip space.
    src += "  vec4 p = oPos;\n"
           "  if (p.w == 0.0 || isinf(p.w)) p.w = 1.0;\n"
           "  p.xy = (2.0 * p.xy - cst.surface.xy) / cst.surface.xy;\n"
           "  p.z = p.z / cst.clip.y;\n"
           "  if (abs(oPos.w - 1.0) > 1e-5) p.x *= cst.clip.z;  // widescreen: perspective (3D) vertices squeezed into the 4:3 buffer\n"
           "  else p.x = cst.ui.y + (p.x - cst.ui.y) * cst.ui.x;  // 16:9: a screen-space element narrowed into the 4:3 area at an edge or the centre\n"
           "  gl_Position = vec4(p.xyz * p.w, p.w);\n"
           "  vD0 = clamp(oD0, 0.0, 1.0); vD1 = clamp(oD1, 0.0, 1.0); vB0 = clamp(oB0, 0.0, 1.0); vB1 = clamp(oB1, 0.0, 1.0);\n"
           "  vFog = oFog.x; vTex0 = oT0; vTex1 = oT1; vTex2 = oT2; vTex3 = oT3;\n"
           "}\n";
    const std::vector<uint32_t> spv = compileGlsl(EShLangVertex, src);
    VkShaderModule m = VK_NULL_HANDLE;
    if (!spv.empty()) {
        VkShaderModuleCreateInfo ci{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        ci.codeSize = spv.size() * 4;
        ci.pCode = spv.data();
        vkCreateShaderModule(ctx().device, &ci, nullptr, &m);
    }
    shaders_[key] = m;
    return m;
}

VkShaderModule VkRenderer::fragmentShader(uint64_t* keyOut) {
    const uint32_t* R = state().regs;
    uint64_t key = 7;
    struct Last { uint64_t* k; ~Last() { g_lastFragKey = *k; } } last{keyOut};
    const uint32_t stages = R[NV097_SET_COMBINER_CONTROL / 4] & 0xFF;
    key = fnv(&R[NV097_SET_COMBINER_CONTROL / 4], 4, key);
    key = fnv(&R[NV097_SET_SHADER_STAGE_PROGRAM / 4], 4, key);
    key = fnv(&R[NV097_SET_COMBINER_COLOR_ICW / 4], 4 * stages, key);
    key = fnv(&R[NV097_SET_COMBINER_COLOR_OCW / 4], 4 * stages, key);
    key = fnv(&R[NV097_SET_COMBINER_ALPHA_ICW / 4], 4 * stages, key);
    key = fnv(&R[NV097_SET_COMBINER_ALPHA_OCW / 4], 4 * stages, key);
    key = fnv(&R[NV097_SET_COMBINER_SPECULAR_FOG_CW0 / 4], 8, key);
    const bool alphaTest = R[NV097_SET_ALPHA_TEST_ENABLE / 4] & 1;
    key = mix(key, alphaTest ? R[NV097_SET_ALPHA_FUNC / 4] : 0);
    key = mix(key, R[NV097_SET_FOG_ENABLE / 4] & 1);
    // Back-face colours (oB0/oB1) only with two-sided lighting (NV097_SET_TWO_SIDE_LIGHT_EN);
    // otherwise both faces use the front colours. Vertex programs rarely write oB0.
    static const bool oldBackFace = featureOff("backface");
    const bool twoSide = oldBackFace || (R[0x17C4 / 4] & 1);
    key = mix(key, twoSide);
    for (int i = 0; i < 4; ++i)  // alpha kill per enabled stage
        key = mix(key, R[NV097_SET_TEXTURE_CONTROL0 / 4 + i * 16] & ((1u << 30) | (1u << 2)));
    // Stages sampling a depth surface (a shadow map rendered earlier): depth compare.
    uint32_t shadowMask = 0;
    for (int i = 0; i < 4; ++i) {
        if (!(R[NV097_SET_TEXTURE_CONTROL0 / 4 + i * 16] & (1u << 30))) continue;
        const uint32_t base = NV097_SET_TEXTURE_OFFSET / 4 + i * 16, format = R[base + 1];
        const uint32_t dma = (format & 3) == 2 ? R[NV097_SET_CONTEXT_DMA_B / 4] : R[NV097_SET_CONTEXT_DMA_A / 4];
        const uint32_t addr = dmaAddress(dma, nullptr) + R[base];
        const uint32_t color = (format >> 8) & 0xFF;
        if (findSurface(addr, true) && (isDepthFormat(color) || !findSurface(addr, false))) {
            shadowMask |= 1u << i;
            key = mix(key, (format >> 8) & 0xFF);  // Z16 vs Z24 compare range
        }
    }
    if (shadowMask) key = mix(mix(key, shadowMask), R[NV097_SET_SHADOW_DEPTH_FUNC / 4] & 7);
    *keyOut = key;
    auto it = shaders_.find(key);
    if (it != shaders_.end()) return it->second;
    std::string src = "#version 450\n"
        "layout(location=0) in vec4 vD0; layout(location=1) in vec4 vD1; layout(location=2) in vec4 vB0;\n"
        "layout(location=3) in vec4 vB1; layout(location=4) in float vFog;\n"
        "layout(location=5) in vec4 vTex0; layout(location=6) in vec4 vTex1; layout(location=7) in vec4 vTex2;\n"
        "layout(location=8) in vec4 vTex3;\n"
        "layout(set=0, binding=1) uniform FC { vec4 c0[9]; vec4 c1[9]; vec4 fogColor; vec4 alphaRef; vec4 bump[4]; vec4 bumpLum[4]; vec4 texScale[4]; vec4 text; } cf;\n"
        "#define bump cf.bump\n#define bumpLum cf.bumpLum\n";
    for (int i = 0; i < 4; ++i) {
        src += "layout(set=0, binding=" + std::to_string(2 + i) + ") uniform sampler2D tex" + std::to_string(i) + ";\n";
        src += "layout(set=0, binding=" + std::to_string(6 + i) + ") uniform samplerCube tex" + std::to_string(i) + "cube;\n";
        src += "layout(set=0, binding=" + std::to_string(10 + i) + ") uniform sampler3D tex" + std::to_string(i) + "3d;\n";
    }
    src += "layout(location=0) out vec4 fragColor;\nvoid main() {\n"
           + std::string(twoSide ? "  vec4 v0 = gl_FrontFacing ? vD0 : vB0, v1 = gl_FrontFacing ? vD1 : vB1;\n"
                                 : "  vec4 v0 = vD0, v1 = vD1;\n") +
           "  vec4 pFog = vec4(cf.fogColor.rgb, clamp(vFog, 0.0, 1.0));\n";
    // 2D lookups use sTexN: linear (rect) textures take texel coordinates, scaled to 0..1 here.
    for (int i = 0; i < 4; ++i)
        src += "  vec4 sTex" + std::to_string(i) + " = vec4(vTex" + std::to_string(i) + ".xy * cf.texScale[" + std::to_string(i) + "].xy, vTex" +
               std::to_string(i) + ".zw);\n";
    {
        // Magnified text (cf.text.x, set per draw for AY8 glyph textures above 480p): the bilinear
        // glyph coverage goes through a narrow, screen-space-wide threshold, so letters stay crisp
        // and anti-aliased instead of blurring at 2-4x magnification.
        std::string comb = translateCombiners(state(), nullptr, shadowMask);
        const std::string t0 = "  vec4 t0 = textureProj(tex0, sTex0.xyw);\n";
        const size_t at = comb.find(t0);
        if (at != std::string::npos)
            comb.insert(at + t0.size(), "  if (cf.text.x > 0.5) { float a = t0.a, w = max(fwidth(a), 0.02) * 0.6; "
                                        "float s = smoothstep(0.42 - w, 0.42 + w, a); t0 = vec4(max(t0.rgb, vec3(s)), s); }\n");
        src += comb;
    }
    if (alphaTest) {
        const uint32_t func = R[NV097_SET_ALPHA_FUNC / 4] & 7;
        static const char* cmp[] = {"false", "a < r", "a == r", "a <= r", "a > r", "a != r", "a >= r", "true"};
        src += "  { float a = round(fragColor.a * 255.0), r = cf.alphaRef.x; if (!(" + std::string(cmp[func]) + ")) discard; }\n";
    }
    // Side bars beside a blended full-screen panel (cf.text.y, see drawSideBarShade): the panel's
    // own coverage in black.
    src += "  if (cf.text.y > 0.5) fragColor.rgb = vec3(0.0);\n";
    src += "}\n";
    fragSources()[key] = src;  // for the capture log (psh_<key>.frag)
    const std::vector<uint32_t> spv = compileGlsl(EShLangFragment, src);
    VkShaderModule m = VK_NULL_HANDLE;
    if (!spv.empty()) {
        VkShaderModuleCreateInfo ci{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        ci.codeSize = spv.size() * 4;
        ci.pCode = spv.data();
        vkCreateShaderModule(ctx().device, &ci, nullptr, &m);
    }
    shaders_[key] = m;
    return m;
}

// ============================================================================================
// pipelines
// ============================================================================================
VkPipeline VkRenderer::pipeline(uint64_t key, VkRenderPass pass, uint32_t attribMask, const uint32_t* fmts, uint32_t topology) {
    const uint32_t* R = state().regs;
    key = mix(key, reinterpret_cast<uint64_t>(pass));
    key = mix(key, topology);
    static const uint32_t stateMethods[] = {NV097_SET_BLEND_ENABLE, NV097_SET_BLEND_FUNC_SFACTOR, NV097_SET_BLEND_FUNC_DFACTOR,
                                            NV097_SET_BLEND_EQUATION, NV097_SET_DEPTH_TEST_ENABLE, NV097_SET_DEPTH_FUNC,
                                            NV097_SET_DEPTH_MASK, NV097_SET_STENCIL_TEST_ENABLE, NV097_SET_STENCIL_FUNC,
                                            NV097_SET_STENCIL_FUNC_MASK, NV097_SET_STENCIL_MASK, NV097_SET_STENCIL_OP_FAIL,
                                            NV097_SET_STENCIL_OP_ZFAIL, NV097_SET_STENCIL_OP_ZPASS, NV097_SET_CULL_FACE_ENABLE,
                                            NV097_SET_CULL_FACE, NV097_SET_FRONT_FACE, NV097_SET_COLOR_MASK,
                                            NV097_SET_FRONT_POLYGON_MODE, NV097_SET_POLY_OFFSET_FILL_ENABLE};
    for (uint32_t m : stateMethods) key = mix(key, R[m / 4]);
    for (int i = 0; i < 16; ++i) key = mix(key, (attribMask >> i) & 1 ? fmts[i] : 0);
    // The shaders are part of the pipeline: their keys (vertex program, combiners, texture
    // modes, alpha test...) go into the pipeline key.
    uint64_t vk, fk;
    VkShaderModule vs = vertexShader(attribMask, fmts, &vk), fs = fragmentShader(&fk);
    key = mix(mix(key, vk), fk);
    auto it = pipelines_.find(key);
    if (it != pipelines_.end()) return it->second;
    if (!vs || !fs) return pipelines_[key] = VK_NULL_HANDLE;
    VkPipelineShaderStageCreateInfo stages[2]{};
    stages[0].sType = stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = vs;
    stages[0].pName = "main";
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = fs;
    stages[1].pName = "main";

    VkVertexInputBindingDescription binds[16];
    VkVertexInputAttributeDescription attrs[16];
    for (uint32_t i = 0; i < 16; ++i) {
        const bool on = (attribMask >> i) & 1;
        binds[i] = {i, 0, VK_VERTEX_INPUT_RATE_VERTEX};  // strides are dynamic: set per draw below via separate pipelines
        attrs[i] = {i, i, on ? attribFormat(fmts[i]) : VK_FORMAT_R32G32B32A32_SFLOAT, 0};
        binds[i].stride = on ? (fmts[i] >> 8) : 0;
    }
    VkPipelineVertexInputStateCreateInfo vi{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
    vi.vertexBindingDescriptionCount = 16;
    vi.pVertexBindingDescriptions = binds;
    vi.vertexAttributeDescriptionCount = 16;
    vi.pVertexAttributeDescriptions = attrs;
    VkPipelineInputAssemblyStateCreateInfo ia{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    ia.topology = static_cast<VkPrimitiveTopology>(topology);
    VkPipelineViewportStateCreateInfo vp{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    vp.viewportCount = vp.scissorCount = 1;
    VkPipelineRasterizationStateCreateInfo rs{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rs.polygonMode = (R[NV097_SET_FRONT_POLYGON_MODE / 4] == 0x1B01 && ctx().features.fillModeNonSolid) ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
    static const bool noCull = featureOff("cull"), noDepth = featureOff("depthtest");  // debugging
    if ((R[NV097_SET_CULL_FACE_ENABLE / 4] & 1) && !noCull) {
        const uint32_t cf = R[NV097_SET_CULL_FACE / 4];
        rs.cullMode = cf == 0x404 ? VK_CULL_MODE_FRONT_BIT : cf == 0x408 ? VK_CULL_MODE_FRONT_AND_BACK : VK_CULL_MODE_BACK_BIT;
    }
    // Screen-space y points down in both; GL_CW (0x900) front faces are clockwise.
    rs.frontFace = R[NV097_SET_FRONT_FACE / 4] == 0x900 ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rs.depthBiasEnable = R[NV097_SET_POLY_OFFSET_FILL_ENABLE / 4] & 1;
    rs.depthClampEnable = ctx().features.depthClamp;
    rs.lineWidth = 1.0f;
    VkPipelineMultisampleStateCreateInfo ms{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPipelineDepthStencilStateCreateInfo ds{VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
    ds.depthTestEnable = R[NV097_SET_DEPTH_TEST_ENABLE / 4] & 1;
    ds.depthWriteEnable = ds.depthTestEnable && (R[NV097_SET_DEPTH_MASK / 4] & 1);
    if (noDepth) ds.depthTestEnable = ds.depthWriteEnable = VK_FALSE;
    ds.depthCompareOp = compareOp(R[NV097_SET_DEPTH_FUNC / 4]);
    ds.stencilTestEnable = R[NV097_SET_STENCIL_TEST_ENABLE / 4] & 1;
    VkStencilOpState so{};
    so.failOp = stencilOp(R[NV097_SET_STENCIL_OP_FAIL / 4]);
    so.depthFailOp = stencilOp(R[NV097_SET_STENCIL_OP_ZFAIL / 4]);
    so.passOp = stencilOp(R[NV097_SET_STENCIL_OP_ZPASS / 4]);
    so.compareOp = compareOp(R[NV097_SET_STENCIL_FUNC / 4]);
    so.compareMask = R[NV097_SET_STENCIL_FUNC_MASK / 4] & 0xFF;
    so.writeMask = R[NV097_SET_STENCIL_MASK / 4] & 0xFF;
    ds.front = ds.back = so;
    VkPipelineColorBlendAttachmentState cb{};
    cb.blendEnable = R[NV097_SET_BLEND_ENABLE / 4] & 1;
    cb.srcColorBlendFactor = cb.srcAlphaBlendFactor = blendFactor(R[NV097_SET_BLEND_FUNC_SFACTOR / 4]);
    cb.dstColorBlendFactor = cb.dstAlphaBlendFactor = blendFactor(R[NV097_SET_BLEND_FUNC_DFACTOR / 4]);
    cb.colorBlendOp = cb.alphaBlendOp = blendOp(R[NV097_SET_BLEND_EQUATION / 4]);
    const uint32_t mask = R[NV097_SET_COLOR_MASK / 4];
    cb.colorWriteMask = ((mask >> 16) & 0xFF ? VK_COLOR_COMPONENT_R_BIT : 0) | ((mask >> 8) & 0xFF ? VK_COLOR_COMPONENT_G_BIT : 0) |
                        (mask & 0xFF ? VK_COLOR_COMPONENT_B_BIT : 0) | ((mask >> 24) & 0xFF ? VK_COLOR_COMPONENT_A_BIT : 0);
    VkPipelineColorBlendStateCreateInfo cbs{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    cbs.attachmentCount = target_.color ? 1 : 0;
    cbs.pAttachments = &cb;
    VkDynamicState dyn[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_BLEND_CONSTANTS,
                            VK_DYNAMIC_STATE_STENCIL_REFERENCE, VK_DYNAMIC_STATE_DEPTH_BIAS};
    VkPipelineDynamicStateCreateInfo dy{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
    dy.dynamicStateCount = 5;
    dy.pDynamicStates = dyn;
    VkGraphicsPipelineCreateInfo pci{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    pci.stageCount = 2;
    pci.pStages = stages;
    pci.pVertexInputState = &vi;
    pci.pInputAssemblyState = &ia;
    pci.pViewportState = &vp;
    pci.pRasterizationState = &rs;
    pci.pMultisampleState = &ms;
    pci.pDepthStencilState = &ds;
    pci.pColorBlendState = &cbs;
    pci.pDynamicState = &dy;
    pci.layout = pipeLayout_;
    pci.renderPass = pass;
    VkPipeline p = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(ctx().device, pipeCache_, 1, &pci, nullptr, &p) != VK_SUCCESS) p = VK_NULL_HANDLE;
    pipeCacheDirty_ = true;
    XLOG(2, "Vulkan: pipeline %zu created", pipelines_.size() + 1);
    return pipelines_[key] = p;
}

// ============================================================================================
// textures
// ============================================================================================
VkSampler VkRenderer::sampler(int stage) {
    const uint32_t* R = state().regs;
    const uint32_t addr = R[NV097_SET_TEXTURE_ADDRESS / 4 + stage * 16];
    const uint32_t filter = R[NV097_SET_TEXTURE_FILTER / 4 + stage * 16];
    const uint64_t key = (static_cast<uint64_t>(addr & 0x00070707) << 32) | (filter & 0x0FFF0000) | static_cast<uint64_t>(settings().anisotropy);
    auto it = samplers_.find(key);
    if (it != samplers_.end()) return it->second;
    auto wrap = [](uint32_t m) {
        switch (m) {
        case 2: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case 3: case 5: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case 4: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        default: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    };
    const uint32_t minf = (filter >> 16) & 0xFF, magf = (filter >> 24) & 0xF;
    VkSamplerCreateInfo sci{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
    sci.magFilter = magf == 1 ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
    sci.minFilter = (minf == 1 || minf == 3 || minf == 5) ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
    sci.mipmapMode = (minf == 5 || minf == 6) ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;
    sci.addressModeU = wrap(addr & 7);
    sci.addressModeV = wrap((addr >> 8) & 7);
    sci.addressModeW = wrap((addr >> 16) & 7);
    sci.maxLod = (minf <= 2) ? 0.25f : 16.0f;  // no mipmapping for plain nearest/linear
    sci.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    const int aniso = settings().anisotropy;
    if (aniso > 1 && ctx().features.samplerAnisotropy && sci.minFilter == VK_FILTER_LINEAR) {
        sci.anisotropyEnable = VK_TRUE;
        sci.maxAnisotropy = std::min<float>(static_cast<float>(aniso), ctx().props.limits.maxSamplerAnisotropy);
    }
    VkSampler s;
    vkCreateSampler(ctx().device, &sci, nullptr, &s);
    return samplers_[key] = s;
}

// Returns the view for texture stage `stage` (kind: 0 2D, 1 cube, 2 3D), decoding guest
// memory when the content changed, or a render target at that address.
VkImageView VkRenderer::texture(int stage, uint32_t* kind) {
    const uint32_t* R = state().regs;
    const uint32_t base = NV097_SET_TEXTURE_OFFSET / 4 + stage * 16;
    const uint32_t offset = R[base], format = R[base + 1], ctl1 = R[base + 4], rect = R[base + 7];
    const uint32_t dma = (format & 3) == 2 ? R[NV097_SET_CONTEXT_DMA_B / 4] : R[NV097_SET_CONTEXT_DMA_A / 4];
    const uint32_t addr = dmaAddress(dma, nullptr) + offset;
    const uint32_t color = (format >> 8) & 0xFF;
    const bool cube = format & 4;
    const uint32_t dims = (format >> 4) & 0xF;
    uint32_t w, h, d = 1, levels = std::max<uint32_t>(1, (format >> 16) & 0xF);
    if (isLinear(color)) {
        w = rect >> 16;
        h = rect & 0xFFFF;
        levels = 1;
    } else {
        w = 1u << ((format >> 20) & 0xF);
        h = 1u << ((format >> 24) & 0xF);
        if (dims == 3) d = 1u << ((format >> 28) & 0xF);
    }
    *kind = cube ? 1 : dims == 3 ? 2 : 0;
    if (!w || !h || w > 4096 || h > 4096) return VK_NULL_HANDLE;
    // Render-to-texture: a surface rendered at this address. Depth texture formats read the
    // depth surface, others the colour one (both can exist at an address: a buffer used as
    // colour in the front end can later be the gameplay depth buffer).
    const bool depthFmt = isDepthFormat(color);
    if (Surface* s = findSurface(addr, depthFmt)) {
        transition(*s, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        if (syncTrace()) printf("SYNC sample %s %08X %ux%u stage %d cb %p\n", s->depth ? "Z" : "C", addr, s->w, s->h, stage, static_cast<void*>(cmd())), fflush(stdout);
        *kind = 0;
        return s->sampleView;
    }
    if (Surface* s = findSurface(addr, !depthFmt)) {
        transition(*s, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        if (syncTrace()) printf("SYNC sample %s %08X %ux%u stage %d cb %p (other kind)\n", s->depth ? "Z" : "C", addr, s->w, s->h, stage, static_cast<void*>(cmd())), fflush(stdout);
        *kind = 0;
        return s->sampleView;
    }
    const uint32_t pitch = ctl1 >> 16;
    const uint32_t faces = cube ? 6 : 1;
    // Guest bytes of all levels (and faces).
    uint32_t total = 0;
    for (uint32_t l = 0, lw = w, lh = h, ld = d; l < levels; ++l, lw = std::max(1u, lw / 2), lh = std::max(1u, lh / 2), ld = std::max(1u, ld / 2))
        total += isLinear(color) ? pitch * lh : levelBytes(color, lw, lh, ld);
    uint32_t faceStride = (total + 127) & ~127u;
    const uint32_t bytes = faceStride * faces;
    if (static_cast<uint64_t>(addr) + bytes > kPhysSize) return VK_NULL_HANDLE;
    const uint8_t* src = gp(kContigBase + addr);
    const uint64_t key = mix(mix(addr, format), rect);
    Texture& t = textures_[key];
    // Contents are checked at most once per frame (FABLE_DISABLE=texcheck: every use, FNV).
    static const bool oldCheck = featureOff("texcheck");
    if (!oldCheck && t.image && t.lastUse == frames_done_ && t.checked) return t.view;
    const bool dxt = color == 0x0C || color == 0x0E;
    // Compressed textures are loaded, not drawn into: once unchanged for a few checks, they are
    // checked every 16th frame (staggered), which saves hashing most texture memory each frame.
    if (!oldCheck && dxt && t.image && t.checked && t.stable >= 4 && t.pcGen == pctex::generation() &&
        ((frames_done_ + (key >> 7)) & 15) != 0) {
        t.lastUse = frames_done_;
        return t.view;
    }
    const uint64_t hash = oldCheck ? fnv(src, bytes) : fastHash(src, bytes);
    t.lastUse = frames_done_;
    t.checked = true;
    if (t.image && t.hash == hash && (!dxt || t.pcGen == pctex::generation())) {
        ++t.stable;
        return t.view;
    }
    VkDevice dev = ctx().device;
    if (t.image) {
        Texture old = t;
        frames_[frame_].garbage.push_back([dev, old] {
            vkDestroyImageView(dev, old.view, nullptr);
            vkDestroyImage(dev, old.image, nullptr);
            vkFreeMemory(dev, old.mem, nullptr);
        });
        t = Texture{};
    }
    t.addr = addr;
    t.format = format;
    t.w = w;
    t.h = h;
    t.d = d;
    t.levels = levels;
    t.hash = hash;
    t.pcGen = pctex::generation();
    t.lastUse = frames_done_;
    t.checked = true;
    // pc_textures: a DXT1/DXT3 texture whose mip 0 matches a known Xbox bank entry is replaced by
    // the PC version of that entry when it is larger (mips generated here by blits).
    if ((color == 0x0C || color == 0x0E) && !cube && d == 1 && faces == 1 && !settings().pcTextures.empty()) {
        const uint32_t m0 = levelBytes(color, w, h, 1);
        pctex::Replacement rep;
        const bool found = m0 <= bytes && pctex::lookup(pctex::hashBytes(src, m0), m0, w, h, rep);
        static const bool dumpMiss = getenv("FABLE_PCTEX_DUMP") != nullptr;  // debugging: unmatched textures to files
        static int nDump = 0;
        if (!found && dumpMiss && m0 <= bytes && nDump < 40) {
            char name[64];
            snprintf(name, sizeof name, "miss_%02d_%ux%u_%08X.bin", nDump++, w, h, addr);
            if (FILE* f = fopen(name, "wb")) { fwrite(src, 1, m0, f); fclose(f); }
        }
        if (found) {
            uint32_t mips = 1;
            while ((std::max(rep.w, rep.h) >> mips) > 0) ++mips;
            VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
            ici.imageType = VK_IMAGE_TYPE_2D;
            ici.format = VK_FORMAT_B8G8R8A8_UNORM;
            ici.extent = {rep.w, rep.h, 1};
            ici.mipLevels = mips;
            ici.arrayLayers = 1;
            ici.samples = VK_SAMPLE_COUNT_1_BIT;
            ici.tiling = VK_IMAGE_TILING_OPTIMAL;
            ici.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            vkCreateImage(dev, &ici, nullptr, &t.image);
            VkMemoryRequirements req;
            vkGetImageMemoryRequirements(dev, t.image, &req);
            VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
            mai.allocationSize = req.size;
            mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            vkAllocateMemory(dev, &mai, nullptr, &t.mem);
            vkBindImageMemory(dev, t.image, t.mem, 0);
            endPass();
            VkCommandBuffer cb = cmd();
            auto barrier = [&](uint32_t level, VkImageLayout from, VkImageLayout to, VkAccessFlags sa, VkAccessFlags da) {
                VkImageMemoryBarrier b{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
                b.srcAccessMask = sa;
                b.dstAccessMask = da;
                b.oldLayout = from;
                b.newLayout = to;
                b.srcQueueFamilyIndex = b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                b.image = t.image;
                b.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, level, 1, 0, 1};
                vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_TRANSFER_BIT | VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT |
                                     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &b);
            };
            for (uint32_t l = 0; l < mips; ++l)
                barrier(l, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT);
            VkBufferImageCopy c{};
            c.bufferOffset = upload(rep.bgra.data(), rep.bgra.size(), 16);
            c.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
            c.imageExtent = {rep.w, rep.h, 1};
            vkCmdCopyBufferToImage(cb, frames_[frame_].upload.buf, t.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &c);
            for (uint32_t l = 1; l < mips; ++l) {
                barrier(l - 1, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT,
                        VK_ACCESS_TRANSFER_READ_BIT);
                VkImageBlit bl{};
                bl.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, l - 1, 0, 1};
                bl.srcOffsets[1] = {static_cast<int32_t>(std::max(1u, rep.w >> (l - 1))), static_cast<int32_t>(std::max(1u, rep.h >> (l - 1))), 1};
                bl.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, l, 0, 1};
                bl.dstOffsets[1] = {static_cast<int32_t>(std::max(1u, rep.w >> l)), static_cast<int32_t>(std::max(1u, rep.h >> l)), 1};
                vkCmdBlitImage(cb, t.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, t.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bl, VK_FILTER_LINEAR);
                barrier(l - 1, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_READ_BIT,
                        VK_ACCESS_SHADER_READ_BIT);
            }
            barrier(mips - 1, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT,
                    VK_ACCESS_SHADER_READ_BIT);
            VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            vci.image = t.image;
            vci.viewType = VK_IMAGE_VIEW_TYPE_2D;
            vci.format = VK_FORMAT_B8G8R8A8_UNORM;
            vci.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, mips, 0, 1};
            vkCreateImageView(dev, &vci, nullptr, &t.view);
            return t.view;
        }
    }
    VkImageCreateInfo ici{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    ici.flags = cube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
    ici.imageType = d > 1 ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D;
    ici.format = VK_FORMAT_B8G8R8A8_UNORM;
    ici.extent = {w, h, d};
    ici.mipLevels = levels;
    ici.arrayLayers = faces;
    ici.samples = VK_SAMPLE_COUNT_1_BIT;
    ici.tiling = VK_IMAGE_TILING_OPTIMAL;
    ici.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vkCreateImage(dev, &ici, nullptr, &t.image);
    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(dev, t.image, &req);
    VkMemoryAllocateInfo mai{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mai.allocationSize = req.size;
    mai.memoryTypeIndex = memoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkAllocateMemory(dev, &mai, nullptr, &t.mem);
    vkBindImageMemory(dev, t.image, t.mem, 0);
    // Palette for I8 textures.
    uint32_t palette[256];
    const uint32_t* pal = nullptr;
    if (color == 0x0B) {
        const uint32_t pr = R[base + 8];
        const uint32_t pdma = (pr & 1) ? R[NV097_SET_CONTEXT_DMA_B / 4] : R[NV097_SET_CONTEXT_DMA_A / 4];
        const uint32_t pa = dmaAddress(pdma, nullptr) + (pr & ~0x3Fu);
        std::memcpy(palette, gp(kContigBase + pa), sizeof palette);
        pal = palette;
    }
    endPass();
    std::vector<VkBufferImageCopy> copies;
    std::vector<uint32_t> texels;
    for (uint32_t f = 0; f < faces; ++f) {
        uint32_t off = f * faceStride;
        for (uint32_t l = 0, lw = w, lh = h, ld = d; l < levels; ++l, lw = std::max(1u, lw / 2), lh = std::max(1u, lh / 2), ld = std::max(1u, ld / 2)) {
            texels.resize(static_cast<size_t>(lw) * lh * ld);
            decodeLevel(color, src + off, lw, lh, ld, pitch, pal, texels.data());
            off += isLinear(color) ? pitch * lh : levelBytes(color, lw, lh, ld);
            VkBufferImageCopy c{};
            c.bufferOffset = upload(texels.data(), texels.size() * 4, 16);
            c.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, l, f, 1};
            c.imageExtent = {lw, lh, ld};
            copies.push_back(c);
        }
    }
    VkImageMemoryBarrier br{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    br.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    br.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    br.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    br.srcQueueFamilyIndex = br.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    br.image = t.image;
    br.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, levels, 0, faces};
    vkCmdPipelineBarrier(cmd(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &br);
    vkCmdCopyBufferToImage(cmd(), frames_[frame_].upload.buf, t.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(copies.size()), copies.data());
    br.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    br.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    br.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    br.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    vkCmdPipelineBarrier(cmd(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &br);
    VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    vci.image = t.image;
    vci.viewType = cube ? VK_IMAGE_VIEW_TYPE_CUBE : d > 1 ? VK_IMAGE_VIEW_TYPE_3D : VK_IMAGE_VIEW_TYPE_2D;
    vci.format = VK_FORMAT_B8G8R8A8_UNORM;
    vci.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, levels, 0, faces};
    vkCreateImageView(dev, &vci, nullptr, &t.view);
    return t.view;
}

// ============================================================================================
// draws
// ============================================================================================
void VkRenderer::draw(const std::vector<uint32_t>* indices, uint32_t first, uint32_t count, const uint8_t* inl, uint32_t inlStride) {
    if (!count) return;
    const State& st = state();
    const uint32_t* R = st.regs;
    bindTargets();
    // Primitive topology; quads and polygons become triangle lists.
    static const VkPrimitiveTopology kTop[] = {VK_PRIMITIVE_TOPOLOGY_POINT_LIST, VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN};
    const uint32_t prim = std::min<uint32_t>(primitive_, 10);
    // Logical vertex sequence (indices into the arrays, before conversion).
    std::vector<uint32_t> seq;
    if (indices) seq = *indices;
    const bool needIndex = indices || prim == 8 || prim == 9 || prim == 3;
    if (!indices && needIndex) {
        seq.resize(count);
        for (uint32_t i = 0; i < count; ++i) seq[i] = first + i;
    }
    if (needIndex) {
        std::vector<uint32_t> out;
        if (prim == 8) {  // quads
            for (size_t i = 0; i + 3 < seq.size(); i += 4) out.insert(out.end(), {seq[i], seq[i + 1], seq[i + 2], seq[i], seq[i + 2], seq[i + 3]});
            seq.swap(out);
        } else if (prim == 9) {  // quad strip
            for (size_t i = 0; i + 3 < seq.size(); i += 2) out.insert(out.end(), {seq[i], seq[i + 1], seq[i + 3], seq[i], seq[i + 3], seq[i + 2]});
            seq.swap(out);
        } else if (prim == 3 && !seq.empty()) {  // line loop
            seq.push_back(seq.front());
        }
    }
    uint32_t minIdx = first, maxIdx = first + count - 1;
    if (needIndex && !seq.empty()) {
        minIdx = *std::min_element(seq.begin(), seq.end());
        maxIdx = *std::max_element(seq.begin(), seq.end());
    }
    if (maxIdx - minIdx > 0x100000) { ++g_ds.garbage; return; }  // garbage indices
    const uint32_t nverts = maxIdx - minIdx + 1;

    // Vertex attributes.
    uint32_t mask = 0, fmts[16];
    const uint8_t* src[16]{};  // where each attribute's data starts (for the float fallback)
    VkDeviceSize offs[16];
    VkBuffer bufs[16];
    const VkBuffer ring = frames_[frame_].upload.buf;
    uint32_t inlOff = 0;
    const uint32_t dmaA = dmaAddress(R[NV097_SET_CONTEXT_DMA_VERTEX_A / 4], nullptr);
    const uint32_t dmaB = dmaAddress(R[NV097_SET_CONTEXT_DMA_VERTEX_B / 4], nullptr);
    for (int i = 0; i < 16; ++i) {
        fmts[i] = R[NV097_SET_VERTEX_DATA_ARRAY_FORMAT / 4 + i];
        const uint32_t size = (fmts[i] >> 4) & 0xF;
        bufs[i] = ring;
        if (inlStride == 0xFFFFFFFF) {  // immediate mode: 16 vec4s per vertex
            fmts[i] = 0x42 | (256u << 8);
            mask |= 1u << i;
            offs[i] = upload(nullptr, 0, 16);
            continue;
        }
        if (!size) {  // disabled: the current constant value
            offs[i] = upload(attrib_[i], 16, 16);
            continue;
        }
        mask |= 1u << i;
        if (inl) {  // interleaved inline data
            fmts[i] = (fmts[i] & 0xFF) | (inlStride << 8);
            src[i] = inl + inlOff;
            offs[i] = inlOff;
            inlOff += attribBytes(fmts[i]);
            continue;
        }
        const uint32_t stride = fmts[i] >> 8;
        const uint32_t off = R[NV097_SET_VERTEX_DATA_ARRAY_OFFSET / 4 + i];
        const uint32_t base = ((off & 0x80000000u) ? dmaB : dmaA) + (off & 0x7FFFFFFFu);
        const uint64_t lo = static_cast<uint64_t>(base) + static_cast<uint64_t>(minIdx) * stride;
        const uint64_t bytes = static_cast<uint64_t>(nverts - 1) * stride + attribBytes(fmts[i]);
        if (lo + bytes > kPhysSize) { ++g_ds.oob; return; }
        src[i] = gp(kContigBase + static_cast<uint32_t>(lo));
        offs[i] = upload(src[i], bytes, 4);
    }
    if (inl) {
        const VkDeviceSize o = upload(inl, static_cast<VkDeviceSize>(count) * (inlStride == 0xFFFFFFFF ? 256 : inlStride), 16);
        for (int i = 0; i < 16; ++i)
            if ((mask >> i) & 1) offs[i] += o + (inlStride == 0xFFFFFFFF ? 16u * i : 0);
        minIdx = 0;
    }
    // 16:9 (native width): the game lays out screen-space elements (HUD, menus, text, movies) in
    // 640x480, which the wider image would stretch. Screen-space draws (every vertex at w = 1, depth
    // test off, not sampling a render target, not a full-screen untextured fade) are drawn into the
    // 4:3 proportions: menus (frames with a full-screen panel) and centre elements in the centred 4:3
    // area, HUD clusters at the left / right screen edges (uiAnchor; FABLE_DISABLE=uicorners keeps
    // everything centred). Narrowing each quad about its own centre split menus built from several
    // quads; full-screen 4:3 images become pillarboxed. Classified by running the vertex program on the CPU (the first vertex
    // decides whether a draw is screen-space at all). FABLE_DISABLE=uifix.
    float uiScale = 1.0f, uiCenter = 0.0f, textSharp = 0.0f, hudSqueeze = 1.0f;
    int sideBars = 0;  // a full-screen 4:3 panel at 16:9: 1 extend its edges into the side bars, 2 shade them by its alpha
    static const bool noUiFix = featureOff("uifix");
    if ((wide_ || outH_ > 480) && !noUiFix && target_.w == 640 && target_.h == 480 &&
        (R[NV097_SET_TRANSFORM_EXECUTION_MODE / 4] & 3) == 2) {
        const uint32_t nv = needIndex ? static_cast<uint32_t>(seq.size()) : count;
        bool flat = nv > 0 && nv <= 4096, textured = false, effect = false;
        uint64_t tex0 = 0;  // stage 0 texture (address, format): identifies HUD pieces
        for (int i = 0; i < 4 && flat; ++i) {
            if (!(R[NV097_SET_TEXTURE_CONTROL0 / 4 + i * 16] & (1u << 30))) continue;
            textured = true;
            const uint32_t tb = NV097_SET_TEXTURE_OFFSET / 4 + i * 16;
            const uint32_t tdma = (R[tb + 1] & 3) == 2 ? R[NV097_SET_CONTEXT_DMA_B / 4] : R[NV097_SET_CONTEXT_DMA_A / 4];
            const uint32_t taddr = dmaAddress(tdma, nullptr) + R[tb];
            if (i == 0) tex0 = (static_cast<uint64_t>(taddr) << 32) | R[NV097_SET_TEXTURE_FORMAT / 4];
            // Effect passes (bloom, outline composites) sample a render target on stage 0; HUD draws
            // leave later stages enabled on the bloom buffers without using them.
            if (i == 0 && (findSurface(taddr, false) || findSurface(taddr, true))) effect = true;
        }
        float minX = 1e30f, maxX = -1e30f, maxZ = -1e30f, minY = 1e30f, maxY = -1e30f;
        float minA = 1e30f, maxA = -1e30f;  // vertex diffuse alpha (input register 3)
        for (uint32_t k = 0; k < nv && flat && !effect; ++k) {
            const uint32_t rel = (needIndex ? seq[k] : first + k) - minIdx;
            float v[16][4];
            if (inlStride == 0xFFFFFFFF) {  // immediate mode: 16 float vec4s per vertex, in order
                std::memcpy(v, inl + static_cast<size_t>(needIndex ? seq[k] : k) * 256, sizeof v);
            } else for (int i = 0; i < 16; ++i) {
                std::memcpy(v[i], attrib_[i], 16);
                if (!((mask >> i) & 1) || !src[i]) continue;
                const uint32_t type = fmts[i] & 0xF, size = (fmts[i] >> 4) & 0xF;
                const uint8_t* p = src[i] + static_cast<size_t>(rel) * (fmts[i] >> 8);
                float d[4] = {0, 0, 0, 1};
                for (uint32_t c = 0; c < size && c < 4; ++c) {
                    if (type == 2) std::memcpy(&d[c], p + 4 * c, 4);
                    else if (type == 0) d[c] = p[size >= 3 ? (c < 3 ? 2 - c : 3) : c] / 255.0f;
                    else if (type == 4) d[c] = p[c] / 255.0f;
                    else if (type == 1 || type == 5) {
                        int16_t x;
                        std::memcpy(&x, p + 2 * c, 2);
                        d[c] = type == 1 ? std::max(x / 32767.0f, -1.0f) : static_cast<float>(x);
                    }
                }
                std::memcpy(v[i], d, 16);
            }
            float pos[4];
            evalVertexPosition(st.program, R[NV097_SET_TRANSFORM_PROGRAM_START / 4], st.constants, v, pos);
            minA = std::min(minA, v[3][3]);
            maxA = std::max(maxA, v[3][3]);
            if (!(std::fabs(pos[3] - 1.0f) <= 1e-5f) || !std::isfinite(pos[0])) flat = false;
            maxZ = std::max(maxZ, pos[2]);
            minX = std::min(minX, pos[0]);
            maxX = std::max(maxX, pos[0]);
            minY = std::min(minY, pos[1]);
            maxY = std::max(maxY, pos[1]);
        }
        const bool fullWidth = minX <= 4.0f && maxX >= 636.0f;
        if (getenv("FABLE_UI_LOG") && flat && !effect) {  // debugging: texture formats of screen-space draws
            static std::unordered_map<uint64_t, int> seen;
            const uint32_t tf = R[NV097_SET_TEXTURE_FORMAT / 4], tr = R[NV097_SET_TEXTURE_IMAGE_RECT / 4];
            const uint64_t k = (static_cast<uint64_t>(tf) << 32) | tr;
            if (seen[k]++ == 0 && seen.size() < 200)
                XLOG(0, "UI draw: tex0 fmt %08X rect %08X @%08X, %u verts, x %g..%g, zmax %g ztest %u, comb %08X", tf, tr, R[NV097_SET_TEXTURE_OFFSET / 4], nv,
                     minX, maxX, maxZ, R[NV097_SET_DEPTH_TEST_ENABLE / 4] & 1, R[NV097_SET_COMBINER_CONTROL / 4]);
        }
        // Depth-tested screen-space draws are interface too when they sit on the near plane (HUD
        // icons and menus: z = 0); sprites the game projects onto the world have a real depth.
        const bool depthTest = R[NV097_SET_DEPTH_TEST_ENABLE / 4] & 1;
        g_uiClass[0] = flat; g_uiClass[1] = effect; g_uiClass[2] = maxZ; g_uiClass[3] = minX; g_uiClass[4] = maxX;
        const bool screenSpace = flat && !effect && (!depthTest || maxZ <= 1.0f);
        if (wide_ && screenSpace && !(fullWidth && !textured)) {
            uiScale = 0.75f;
            static const bool noCornersEnv = featureOff("uicorners");
            const bool noCorners = noCornersEnv || !settings().hudCorners;
            // A menu background covers the screen; additive sprites (light glare, flares) that
            // happen to cover it are not one.
            const bool additive = (R[NV097_SET_BLEND_ENABLE / 4] & 1) && (R[NV097_SET_BLEND_FUNC_DFACTOR / 4] & 0xFFFF) == NV097_SET_BLEND_FUNC_SFACTOR_V_ONE;
            const bool panel = textured && !additive && fullWidth && minY <= 16.0f && maxY >= 464.0f;
            uiCenter = noCorners ? 0.0f : uiAnchor(minX, maxX, minY, maxY, panel, tex0);  // -1 left edge, 0 centre, 1 right edge
            // The side bars beside a full-screen 4:3 panel would show whatever is behind it (the
            // world, or the clear colour beside the intro movie): an opaque panel (movies, loading
            // screens) gives them its corner colour; one blended over the game (the fade at scene
            // cuts and skips, quest cards) darkens them by its own alpha, so they fade in and out
            // with it instead of turning black while the fade is still clear. FABLE_DISABLE=sidebars.
            static const bool noBars = featureOff("sidebars");
            if (panel && uiCenter == 0.0f && !noBars) sideBars = (R[NV097_SET_BLEND_ENABLE / 4] & 1) ? 2 : 1;
            if (panel && getenv("FABLE_UI_LOG"))  // debugging: what each full-screen panel is
                XLOG(0, "UI panel flip %llu: tex %08X fmt %08X rect %08X blend %u %04X/%04X alpha %.3f..%.3f comb %08X", static_cast<unsigned long long>(g_frameCount),
                     static_cast<uint32_t>(tex0 >> 32), R[NV097_SET_TEXTURE_FORMAT / 4], R[NV097_SET_TEXTURE_IMAGE_RECT / 4], R[NV097_SET_BLEND_ENABLE / 4] & 1,
                     R[NV097_SET_BLEND_FUNC_SFACTOR / 4] & 0xFFFF, R[NV097_SET_BLEND_FUNC_DFACTOR / 4] & 0xFFFF, minA, maxA, R[NV097_SET_COMBINER_CONTROL / 4]);
        }
        // Text (the glyph cache is an AY8 texture) above 480p: glyph edges are sharpened in the shader.
        static const bool noSharp = featureOff("textsharp");
        if (screenSpace && outH_ > 480 && !noSharp && settings().textSharpen && (R[NV097_SET_TEXTURE_CONTROL0 / 4] & (1u << 30)) &&
            ((R[NV097_SET_TEXTURE_FORMAT / 4] >> 8) & 0xFF) == 0x01)
            textSharp = 1.0f;
    }
    if (inlStride != 0xFFFFFFFF)
        for (int i = 0; i < 16; ++i) {
            const uint32_t type = fmts[i] & 0xF;
            if (!((mask >> i) & 1) || !src[i] || (type != 0 && type != 1 && type != 4 && type != 5)) continue;
            if (vertexFormatSupported(attribFormat(fmts[i]))) continue;
            static uint32_t logged[16];  // once per Kelvin type/size
            if (!(logged[type] & (1u << ((fmts[i] >> 4) & 0xF)))) {
                logged[type] |= 1u << ((fmts[i] >> 4) & 0xF);
                XLOG(1, "Vulkan: vertex format %d (Kelvin type %u, %u components) not supported: converting to floats",
                     static_cast<int>(attribFormat(fmts[i])), type, (fmts[i] >> 4) & 0xF);
            }
            static std::vector<float> conv;
            attribToFloat(src[i], fmts[i] >> 8, inl ? count : nverts, fmts[i], conv);
            const uint32_t size = (fmts[i] >> 4) & 0xF;
            offs[i] = upload(conv.data(), conv.size() * sizeof(float), 4);
            fmts[i] = 0x2 | (size << 4) | ((size * 4) << 8);
        }
    beginPass();
    if (!pass_) { ++g_ds.nopass; return; }
    VkPipeline p = pipeline(0, pass_, mask, fmts, kTop[prim]);
    if (!p) { ++g_ds.nopipe; return; }
    static const int skipClass = getenv("FABLE_SKIP") ? atoi(getenv("FABLE_SKIP")) : 0;  // debugging
    if (skipClass == 1 && (R[NV097_SET_BLEND_ENABLE / 4] & 1) && R[NV097_SET_BLEND_FUNC_DFACTOR / 4] == 1) return;
    if (skipClass == 2 && target_.w == 320) return;
    if (skipClass == 3 && (R[NV097_SET_COMBINER_CONTROL / 4] & 0xFF) == 2) return;
    ++g_ds.draws;
    g_ds.verts += needIndex ? seq.size() : count;
    static const long logFrame = getenv("FABLE_DRAWLOG") ? atol(getenv("FABLE_DRAWLOG")) : -1;
    if (logFrame >= 0 && static_cast<long>(frames_done_) == logFrame) {
        const uint32_t tex0 = (R[NV097_SET_TEXTURE_CONTROL0 / 4] >> 30) & 1;
        XLOG(0, "draw %llu: prim %u, %u verts, target %08X %ux%u fmt %X, blend %u (%X,%X eq %X), zfunc %X en %u, "
                "tex0 %u @%08X fmt %08X, prog %u, fog %u mode %X, comb %08X, final %08X/%08X, colormask %08X",
             static_cast<unsigned long long>(g_ds.draws), prim, needIndex ? static_cast<uint32_t>(seq.size()) : count, target_.color, target_.w,
             target_.h, R[NV097_SET_SURFACE_FORMAT / 4], R[NV097_SET_BLEND_ENABLE / 4], R[NV097_SET_BLEND_FUNC_SFACTOR / 4],
             R[NV097_SET_BLEND_FUNC_DFACTOR / 4], R[NV097_SET_BLEND_EQUATION / 4], R[0x354 / 4], R[0x30C / 4], tex0,
             R[NV097_SET_TEXTURE_OFFSET / 4], R[NV097_SET_TEXTURE_FORMAT / 4], (R[NV097_SET_TRANSFORM_EXECUTION_MODE / 4] & 3) == 2,
             R[NV097_SET_FOG_ENABLE / 4], R[NV097_SET_FOG_MODE / 4], R[NV097_SET_COMBINER_CONTROL / 4],
             R[NV097_SET_COMBINER_SPECULAR_FOG_CW0 / 4], R[NV097_SET_COMBINER_SPECULAR_FOG_CW1 / 4], R[0x358 / 4]);
        float fp[3];
        std::memcpy(fp, &R[NV097_SET_FOG_PARAMS / 4], 12);
        XLOG(0, "   fog params %g %g %g, gen %u, color %08X, specfog c0 %08X c1 %08X", fp[0], fp[1], fp[2], R[NV097_SET_FOG_GEN_MODE / 4],
             R[NV097_SET_FOG_COLOR / 4], R[NV097_SET_SPECULAR_FOG_FACTOR / 4], R[NV097_SET_SPECULAR_FOG_FACTOR / 4 + 1]);
    }
    if ((R[NV097_SET_TRANSFORM_EXECUTION_MODE / 4] & 3) == 2) ++g_ds.program;
    for (int i = 0; i < 4; ++i)
        if (R[NV097_SET_TEXTURE_CONTROL0 / 4 + i * 16] & (1u << 30)) { ++g_ds.textured; break; }
    g_ds.target = target_.color;
    g_ds.w = target_.w;
    g_ds.h = target_.h;
    g_ds.fmt = R[NV097_SET_SURFACE_FORMAT / 4];

    // Uniforms.
    struct VC { float c[192][4]; float surface[4]; float clip[4]; float fog[4]; float ui[4]; } vc{};
    std::memcpy(vc.c, st.constants, sizeof vc.c);
    if ((R[NV097_SET_TRANSFORM_EXECUTION_MODE / 4] & 3) != 2) std::memcpy(vc.c, &R[NV097_SET_COMPOSITE_MATRIX / 4], 64);
    vc.surface[0] = static_cast<float>(target_.w / target_.aaX);  // the clip size (AA surfaces are larger in memory)
    vc.surface[1] = static_cast<float>(target_.h / target_.aaY);
    // Test harness (FABLE_HEAVY_DRAWS): count main-target draws with a NaN vertex constant
    // (a NaN camera or bone matrix: the targeting void, vanishing NPCs), logged once a second.
    static const bool nanStats = getenv("FABLE_HEAVY_DRAWS") != nullptr;
    if (nanStats) {
        static uint64_t nanDraws = 0, lastFlip = 0;
        bool bad = false;
        for (int i = 0; i < 192 * 4 && !bad; ++i) bad = std::isnan(vc.c[i / 4][i % 4]);
        if (bad && target_.w >= 320) ++nanDraws;
        if (g_frameCount / 30 != lastFlip / 30) {
            if (nanDraws) XLOG(0, "NaN-constant draws: %llu by flip %llu", static_cast<unsigned long long>(nanDraws), static_cast<unsigned long long>(g_frameCount));
            nanDraws = 0;
        }
        lastFlip = g_frameCount;
    }
    float cmin, cmax;
    std::memcpy(&cmin, &R[NV097_SET_CLIP_MIN / 4], 4);
    std::memcpy(&cmax, &R[NV097_SET_CLIP_MAX / 4], 4);
    vc.clip[0] = cmin;
    vc.clip[1] = cmax > 0 ? cmax : 16777215.0f;
    std::memcpy(vc.fog, &R[NV097_SET_FOG_PARAMS / 4], 12);
    vc.ui[0] = uiScale;
    vc.ui[1] = uiCenter;
    // Widescreen (aspect = 16:9): the game's camera projects a 16:9 view into the 640x480 buffer
    // (hle_SetupGamut) and the presenter stretches the frame. The old fallback (FABLE_DISABLE=hor+)
    // squeezed perspective draws into the back buffer instead, which shows the game's 4:3 culling at
    // the edges and misplaces effects drawn through offscreen targets.
    static const bool squeeze = featureOff("hor+");
    vc.clip[2] = (squeeze && settings().widescreen && target_.w == 640 && target_.h == 480) ? 0.75f : hudSqueeze;
    const VkDeviceSize vcOff = upload(&vc, sizeof vc, ctx().props.limits.minUniformBufferOffsetAlignment);
    struct FC { float c0[9][4]; float c1[9][4]; float fog[4]; float aref[4]; float bump[4][4]; float lum[4][4]; float texScale[4][4]; float text[4]; } fc{};
    auto unpack = [](uint32_t v, float* o) {
        o[0] = ((v >> 16) & 0xFF) / 255.0f;
        o[1] = ((v >> 8) & 0xFF) / 255.0f;
        o[2] = (v & 0xFF) / 255.0f;
        o[3] = (v >> 24) / 255.0f;
    };
    for (int i = 0; i < 8; ++i) {
        unpack(R[NV097_SET_COMBINER_FACTOR0 / 4 + i], fc.c0[i]);
        unpack(R[NV097_SET_COMBINER_FACTOR1 / 4 + i], fc.c1[i]);
    }
    unpack(R[NV097_SET_SPECULAR_FOG_FACTOR / 4], fc.c0[8]);      // the final combiner's own constants
    unpack(R[NV097_SET_SPECULAR_FOG_FACTOR / 4 + 1], fc.c1[8]);
    {  // fog colour is ABGR (red in the low byte)
        const uint32_t v = R[NV097_SET_FOG_COLOR / 4];
        fc.fog[0] = (v & 0xFF) / 255.0f;
        fc.fog[1] = ((v >> 8) & 0xFF) / 255.0f;
        fc.fog[2] = ((v >> 16) & 0xFF) / 255.0f;
        fc.fog[3] = (v >> 24) / 255.0f;
    }
    fc.aref[0] = static_cast<float>(R[NV097_SET_ALPHA_REF / 4] & 0xFF);
    for (int i = 0; i < 4; ++i) {
        std::memcpy(fc.bump[i], &R[NV097_SET_TEXTURE_SET_BUMP_ENV_MAT / 4 + i * 16], 16);
        std::memcpy(&fc.lum[i][0], &R[NV097_SET_TEXTURE_SET_BUMP_ENV_SCALE / 4 + i * 16], 8);
        // Linear (rect) textures are addressed in texels (as xemu's texScale).
        const uint32_t base = NV097_SET_TEXTURE_OFFSET / 4 + i * 16, rect = R[base + 7];
        const bool linear = isLinear((R[base + 1] >> 8) & 0xFF) && (rect >> 16) && (rect & 0xFFFF);
        fc.texScale[i][0] = linear ? 1.0f / static_cast<float>(rect >> 16) : 1.0f;
        fc.texScale[i][1] = linear ? 1.0f / static_cast<float>(rect & 0xFFFF) : 1.0f;
    }
    fc.text[0] = textSharp;
    const VkDeviceSize fcOff = upload(&fc, sizeof fc, ctx().props.limits.minUniformBufferOffsetAlignment);

    // Descriptors.
    VkDescriptorSetAllocateInfo dai{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    dai.descriptorPool = frames_[frame_].descPool;
    dai.descriptorSetCount = 1;
    dai.pSetLayouts = &setLayout_;
    VkDescriptorSet set;
    if (vkAllocateDescriptorSets(ctx().device, &dai, &set) != VK_SUCCESS) {
        ++g_ds.noset;
        submitFrame(false);
        return;
    }
    VkDescriptorBufferInfo ub[2] = {{ring, 0, sizeof vc}, {ring, 0, sizeof fc}};
    VkDescriptorImageInfo img[12];
    for (int i = 0; i < 4; ++i) {
        img[i] = {sampler(i), dummy2D_, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
        img[4 + i] = {sampler(i), dummyCube_, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
        img[8 + i] = {sampler(i), dummy3D_, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
        if (R[NV097_SET_TEXTURE_CONTROL0 / 4 + i * 16] & (1u << 30)) {
            uint32_t kind = 0;
            // texture() ends the render pass itself when it has to (uploads, layout transitions
            // of sampled render targets); ending it for every textured draw cost a render pass
            // per draw, each a full tile store and load on mobile GPUs.
            if (VkImageView v = texture(i, &kind)) img[4 * kind + i].imageView = v;
        }
    }
    beginPass();
    VkWriteDescriptorSet wr[14]{};
    for (int i = 0; i < 14; ++i) {
        wr[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        wr[i].dstSet = set;
        wr[i].dstBinding = static_cast<uint32_t>(i);
        wr[i].descriptorCount = 1;
        if (i < 2) {
            wr[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            wr[i].pBufferInfo = &ub[i];
        } else {
            wr[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            wr[i].pImageInfo = &img[i - 2];
        }
    }
    vkUpdateDescriptorSets(ctx().device, 14, wr, 0, nullptr);

    VkCommandBuffer cb = cmd();
    vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, p);
    const uint32_t dynOff[2] = {static_cast<uint32_t>(vcOff), static_cast<uint32_t>(fcOff)};
    vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLayout_, 0, 1, &set, 2, dynOff);
    vkCmdBindVertexBuffers(cb, 0, 16, bufs, offs);
    VkViewport vp{0, 0, static_cast<float>(hostW(target_.w, target_.h)), static_cast<float>(hostH(target_.h)), 0.0f, 1.0f};
    vkCmdSetViewport(cb, 0, 1, &vp);
    VkRect2D sc{{0, 0}, {hostW(target_.w, target_.h), hostH(target_.h)}};
    vkCmdSetScissor(cb, 0, 1, &sc);
    float bc[4];
    const uint32_t bcol = R[NV097_SET_BLEND_COLOR / 4];
    bc[0] = ((bcol >> 16) & 0xFF) / 255.0f;
    bc[1] = ((bcol >> 8) & 0xFF) / 255.0f;
    bc[2] = (bcol & 0xFF) / 255.0f;
    bc[3] = (bcol >> 24) / 255.0f;
    vkCmdSetBlendConstants(cb, bc);
    vkCmdSetStencilReference(cb, VK_STENCIL_FACE_FRONT_AND_BACK, R[NV097_SET_STENCIL_FUNC_REF / 4] & 0xFF);
    float slope, bias;
    std::memcpy(&slope, &R[NV097_SET_POLYGON_OFFSET_SCALE_FACTOR / 4], 4);
    std::memcpy(&bias, &R[NV097_SET_POLYGON_OFFSET_BIAS / 4], 4);
    const float guestBias = bias, guestSlope = slope;
    // The NV2A adds the bias in the depth surface's own units (1/65535 for Z16, xemu: zvalue +=
    // depthOffset, then floor) and the slope term per guest pixel. Our depth images are 24-bit
    // (Vulkan counts the constant in 1/2^24) and upscaled (slope per host pixel), so a Z16 shadow
    // map got 1/256 of the game's bias: surfaces that both cast and receive (the deck beside an
    // NPC) shadowed themselves on alternate frames (confirmed on the phone). Quick menu / ini
    // switch shadow_bias_fix (on by default; FABLE_ZBIAS_UNITS=1 forces it).
    static const bool forceFix = getenv("FABLE_ZBIAS_UNITS") != nullptr;
    if ((forceFix || settings().shadowBiasFix) && target_.depth && target_.h) {
        if (((R[NV097_SET_SURFACE_FORMAT / 4] >> 4) & 0xF) == 1) bias *= 256.0f;  // Z16 in a 24-bit image
        slope *= static_cast<float>(hostH(target_.h)) / static_cast<float>(target_.h);
    }
    vkCmdSetDepthBias(cb, bias, 0.0f, slope);
    // FABLE_CAPTURE_FLIP=N1,N2: every draw of those frames to cap.txt (debugging).
    static const std::vector<uint64_t> capFlips = [] {
        std::vector<uint64_t> v;
        for (const char* p = getenv("FABLE_CAPTURE_FLIP"); p && *p; p = strchr(p, ',') ? strchr(p, ',') + 1 : "") v.push_back(strtoull(p, nullptr, 10));
        return v;
    }();
    static const long capAuto = getenv("FABLE_CAPTURE_IMAGES") ? atol(getenv("FABLE_CAPTURE_IMAGES")) : 0;
    static uint64_t textFrame = ~0ull;
    if (capAuto > 0 && textFrame == ~0ull && lastFrameDraws_ > static_cast<uint64_t>(capAuto)) textFrame = frames_done_;
    // FABLE_CAPTURE_HIGHLIGHT=1: capture (text + images) the frame after the first highlight pass
    // (an object drawn into a 256x256 colour-only target with a stage-1 PROJECT3D depth compare),
    // then exit.
    static const bool capHi = getenv("FABLE_CAPTURE_HIGHLIGHT") != nullptr;
    static uint64_t hiFrame = ~0ull;
    if (capHi) {
        if (hiFrame == ~0ull && target_.w == 256 && !target_.depth && ((R[NV097_SET_SHADER_STAGE_PROGRAM / 4] >> 5) & 0x1F) == 2) {
            hiFrame = frames_done_ + 1;
            XLOG(0, "highlight pass at flip %llu", static_cast<unsigned long long>(g_frameCount));
        }
        if (hiFrame != ~0ull && frames_done_ > hiFrame) {
            fflush(nullptr);
            _exit(0);
        }
        textFrame = hiFrame;
    }
    if ((!capFlips.empty() && std::find(capFlips.begin(), capFlips.end(), g_frameCount) != capFlips.end()) || frames_done_ == textFrame) {
        static FILE* cap = fopen("cap.txt", "w");
        // Once, on an auto-triggered frame: guest RAM addresses holding runs of NaN floats (where a
        // NaN matrix lives).
        static bool scanned = false;
        if (frames_done_ == textFrame && !scanned && cap) {
            scanned = true;
            int found = 0;
            for (uint32_t a = 0; a + 64 <= kPhysSize && found < 60; a += 4) {
                uint32_t run = 0;
                while (run < 16 && a + 4 * run < kPhysSize) {
                    uint32_t v;
                    std::memcpy(&v, gp(kContigBase + a + 4 * run), 4);
                    if ((v & 0x7F800000u) != 0x7F800000u || !(v & 0x7FFFFFu)) break;
                    ++run;
                }
                if (run >= 4) {
                    fprintf(cap, "NaN run of %u floats at phys %08X\n", run, a);
                    ++found;
                    a += 4 * run;
                }
            }
        }
        static uint64_t lastFlip = ~0ull;
        static uint32_t n = 0;
        if (lastFlip != g_frameCount) lastFlip = g_frameCount, n = 0;
        if (cap) {
            float cmn, cmx;
            std::memcpy(&cmn, &R[NV097_SET_CLIP_MIN / 4], 4);
            std::memcpy(&cmx, &R[NV097_SET_CLIP_MAX / 4], 4);
            fprintf(cap, "vp@%u c5 %g %g %g %g c8 %g %g %g %g ", R[NV097_SET_TRANSFORM_PROGRAM_START / 4], st.constants[5][0], st.constants[5][1],
                    st.constants[5][2], st.constants[5][3], st.constants[8][0], st.constants[8][1], st.constants[8][2], st.constants[8][3]);
            fprintf(cap, "sfmt %08X clip %g..%g c21 %g %g %g %g c22 %g %g %g %g c58 %g %g %g %g | ", R[NV097_SET_SURFACE_FORMAT / 4], cmn, cmx,
                    st.constants[21][0], st.constants[21][1], st.constants[21][2], st.constants[21][3], st.constants[22][0], st.constants[22][1],
                    st.constants[22][2], st.constants[22][3], st.constants[58][0], st.constants[58][1], st.constants[58][2], st.constants[58][3]);
            fprintf(cap, "%llu/%04u prim %u n %u tgt %08X %ux%u zt %08X blend %u %X/%X eq %X atest %u %X/%u z %u/%X/%u cull %u/%X cmask %08X comb %08X prog %08X stencil %u",
                    static_cast<unsigned long long>(g_frameCount), n++, prim, needIndex ? static_cast<uint32_t>(seq.size()) : count, target_.color, target_.w,
                    target_.h, target_.depth, R[NV097_SET_BLEND_ENABLE / 4] & 1, R[NV097_SET_BLEND_FUNC_SFACTOR / 4], R[NV097_SET_BLEND_FUNC_DFACTOR / 4],
                    R[NV097_SET_BLEND_EQUATION / 4], R[NV097_SET_ALPHA_TEST_ENABLE / 4] & 1, R[NV097_SET_ALPHA_FUNC / 4], R[NV097_SET_ALPHA_REF / 4],
                    R[NV097_SET_DEPTH_TEST_ENABLE / 4] & 1, R[NV097_SET_DEPTH_FUNC / 4], R[NV097_SET_DEPTH_MASK / 4] & 1, R[NV097_SET_CULL_FACE_ENABLE / 4] & 1,
                    R[NV097_SET_CULL_FACE / 4], R[NV097_SET_COLOR_MASK / 4], R[NV097_SET_COMBINER_CONTROL / 4], R[NV097_SET_SHADER_STAGE_PROGRAM / 4],
                    R[NV097_SET_STENCIL_TEST_ENABLE / 4] & 1);
            if (R[NV097_SET_STENCIL_TEST_ENABLE / 4] & 1)
                fprintf(cap, " [func %X ref %X mask %X/%X ops %X/%X/%X]", R[NV097_SET_STENCIL_FUNC / 4], R[NV097_SET_STENCIL_FUNC_REF / 4],
                        R[NV097_SET_STENCIL_FUNC_MASK / 4], R[NV097_SET_STENCIL_MASK / 4], R[NV097_SET_STENCIL_OP_FAIL / 4],
                        R[NV097_SET_STENCIL_OP_ZFAIL / 4], R[NV097_SET_STENCIL_OP_ZPASS / 4]);
            if (R[NV097_SET_ZPASS_PIXEL_COUNT_ENABLE / 4] & 1) fprintf(cap, " ZPASS");
            fprintf(cap, " poff %u bias %g slope %g", R[NV097_SET_POLY_OFFSET_FILL_ENABLE / 4] & 1, guestBias, guestSlope);
            fprintf(cap, " ui %g/%g hud %g text %g class %g/%g/z%g/x%g..%g", vc.ui[0], vc.ui[1], vc.clip[2], fc.text[0], g_uiClass[0], g_uiClass[1],
                    g_uiClass[2], g_uiClass[3], g_uiClass[4]);
            std::fill(g_uiClass, g_uiClass + 5, -1.0f);
            fprintf(cap, " fs %016llx c0 %08X c1 %08X final %08X/%08X", static_cast<unsigned long long>(g_lastFragKey), R[NV097_SET_COMBINER_FACTOR0 / 4],
                    R[NV097_SET_COMBINER_FACTOR1 / 4], R[NV097_SET_SPECULAR_FOG_FACTOR / 4], R[NV097_SET_SPECULAR_FOG_FACTOR / 4 + 1]);
            {
                static std::unordered_map<uint64_t, bool> written;
                if (!written[g_lastFragKey]) {
                    written[g_lastFragKey] = true;
                    char name[64];
                    snprintf(name, sizeof name, "psh_%016llx.frag", static_cast<unsigned long long>(g_lastFragKey));
                    if (FILE* f = fopen(name, "w")) {
                        fputs(fragSources()[g_lastFragKey].c_str(), f);
                        fclose(f);
                    }
                }
            }
            for (int i = 0; i < 16; ++i)
                if ((mask >> i) & 1) fprintf(cap, " a%d:%02X", i, fmts[i] & 0xFF);
            for (int i = 0; i < 4; ++i)
                if (R[NV097_SET_TEXTURE_CONTROL0 / 4 + i * 16] & (1u << 30))
                    fprintf(cap, " t%d:%08X@%08X addr %08X border %08X", i, R[NV097_SET_TEXTURE_FORMAT / 4 + i * 16], R[NV097_SET_TEXTURE_OFFSET / 4 + i * 16],
                            R[NV097_SET_TEXTURE_ADDRESS / 4 + i * 16], R[NV097_SET_TEXTURE_BORDER_COLOR / 4 + i * 16]);
            fputc('\n', cap);
            fflush(cap);
        }
    }
    VkDeviceSize io = 0;
    if (needIndex) {
        for (auto& v : seq) v -= minIdx;
        io = upload(seq.data(), seq.size() * 4, 4);
    }
    // Occlusion query around the draw while the zpass pixel count is enabled (see report()).
    Frame& qf = frames_[frame_];
    uint32_t query = ~0u;
    const bool counting = (R[NV097_SET_ZPASS_PIXEL_COUNT_ENABLE / 4] & 1) && qf.queries && settings().occlusion;
    if (counting) {
        if (!openCount_) {  // counting without a CLEAR_REPORT_VALUE: since the last report
            openCount_ = std::make_shared<ReportCount>();
            reportFirst_ = qf.queryCount;
        }
        if (qf.queryCount >= kMaxQueries || cb != qf.cmd) openCount_->unknown = true;
    }
    if (counting && qf.queryCount < kMaxQueries && cb == qf.cmd) {
        query = qf.queryCount++;
        ++openCount_->draws;
        if (R[NV097_SET_COLOR_MASK / 4]) ++openCount_->colorDraws;
        openCount_->depthFuncs |= (R[NV097_SET_DEPTH_TEST_ENABLE / 4] & 1) ? 1u << (R[NV097_SET_DEPTH_FUNC / 4] & 7) : 0x100u;
        openCount_->target = target_.color;
        const double hostSamples = static_cast<double>(hostW(target_.w, target_.h)) * hostH(target_.h);
        qf.queryScale.push_back(hostSamples > 0 ? static_cast<float>(static_cast<double>(target_.w) * target_.h / hostSamples) : 1.0f);
        vkCmdBeginQuery(cb, qf.queries, query, queryPrecise_ ? VK_QUERY_CONTROL_PRECISE_BIT : 0);
    }
    if (needIndex) {
        vkCmdBindIndexBuffer(cb, ring, io, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cb, static_cast<uint32_t>(seq.size()), 1, 0, 0, 0);
    } else {
        vkCmdDraw(cb, count, 1, inl ? 0 : first - minIdx, 0);
    }
    if (query != ~0u) vkCmdEndQuery(cb, qf.queries, query);
    if (sideBars == 2) {
        // The same quad at full width (no 4:3 narrowing), drawn only into the side bars with its
        // colour forced to black: the bars take the panel's alpha at its edge.
        VC vb = vc;
        vb.ui[0] = 1.0f;
        FC fb = fc;
        fb.text[1] = 1.0f;
        const uint32_t barOff[2] = {static_cast<uint32_t>(upload(&vb, sizeof vb, ctx().props.limits.minUniformBufferOffsetAlignment)),
                                    static_cast<uint32_t>(upload(&fb, sizeof fb, ctx().props.limits.minUniformBufferOffsetAlignment))};
        if (cb == cmd()) {  // (an upload that filled the ring started a new command buffer)
            vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLayout_, 0, 1, &set, 2, barOff);
            const uint32_t w = hostW(target_.w, target_.h), h = hostH(target_.h), bar = w / 8;
            for (int side = 0; side < 2; ++side) {
                const VkRect2D r{{side ? static_cast<int32_t>(w - bar) : 0, 0}, {bar, h}};
                vkCmdSetScissor(cb, 0, 1, &r);
                if (needIndex) vkCmdDrawIndexed(cb, static_cast<uint32_t>(seq.size()), 1, 0, 0, 0);
                else vkCmdDraw(cb, count, 1, inl ? 0 : first - minIdx, 0);
            }
            vkCmdSetScissor(cb, 0, 1, &sc);
        }
    } else if (sideBars) {
        fillSideBars(true);
    }
    // FABLE_CAPTURE_IMAGES=1 with FABLE_CAPTURE_FLIP: the target after each draw to the main
    // 640x480 target goes to capF_NNNN.png (debugging).
    // FABLE_CAPTURE_IMAGES=<draws>: instead, the first frame after one with more draws than that.
    static const long capImages = getenv("FABLE_CAPTURE_IMAGES") ? atol(getenv("FABLE_CAPTURE_IMAGES")) : -1;
    static uint64_t imageFrame = ~0ull;
    ++drawsThisFrame_;
    if (capImages > 0 && imageFrame == ~0ull && lastFrameDraws_ > static_cast<uint64_t>(capImages)) imageFrame = frames_done_;
    if (capHi) imageFrame = hiFrame;
    const bool imageThis = capImages == 0 ? (!capFlips.empty() && std::find(capFlips.begin(), capFlips.end(), g_frameCount) != capFlips.end())
                                          : frames_done_ == imageFrame;
    static const bool capAll = getenv("FABLE_CAPTURE_ALL") != nullptr;  // offscreen targets too, with alpha
    if (imageThis && (capAll || (target_.w == 640 && target_.h == 480))) captureImage();
}

void VkRenderer::captureImage() {
    const auto idx = static_cast<uint32_t>(drawsThisFrame_ - 1);  // matches the cap.txt draw number
    Surface* c = target_.color ? findSurface(target_.color, false) : nullptr;
    if (!c) return;
    const bool c16 = c->format == VK_FORMAT_A1R5G5B5_UNORM_PACK16;
    if (!c16 && c->format != VK_FORMAT_B8G8R8A8_UNORM) return;
    const uint32_t w = hostW(c->w, c->h), h = hostH(c->h);
    static Buffer buf;
    static void* mapped = nullptr;
    if (!buf.buf) {
        buf = createBuffer(2560ull * 1920 * 4, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vkMapMemory(ctx().device, buf.mem, 0, VK_WHOLE_SIZE, 0, &mapped);
    }
    endPass();
    transition(*c, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    VkBufferImageCopy copy{};
    copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    copy.imageExtent = {w, h, 1};
    vkCmdCopyImageToBuffer(cmd(), c->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buf.buf, 1, &copy);
    submitFrame(true);
    static const int ch = getenv("FABLE_CAPTURE_ALL") ? 4 : 3;
    std::vector<uint8_t> rgb(static_cast<size_t>(w) * h * ch);
    const auto* p = static_cast<const uint8_t*>(mapped);
    for (size_t i = 0; i < static_cast<size_t>(w) * h; ++i) {
        uint8_t* o = &rgb[ch * i];
        if (c16) {
            uint16_t v;
            std::memcpy(&v, p + 2 * i, 2);
            o[0] = static_cast<uint8_t>(((v >> 10) & 31) << 3);
            o[1] = static_cast<uint8_t>(((v >> 5) & 31) << 3);
            o[2] = static_cast<uint8_t>((v & 31) << 3);
            if (ch == 4) o[3] = (v & 0x8000) ? 255 : 0;
        } else {
            o[0] = p[4 * i + 2];
            o[1] = p[4 * i + 1];
            o[2] = p[4 * i];
            if (ch == 4) o[3] = p[4 * i + 3];
        }
    }
    char name[96];
    snprintf(name, sizeof name, "cap%llu_%04u_%08X.png", static_cast<unsigned long long>(g_frameCount), idx, target_.color);
    stbi_write_png(name, static_cast<int>(w), static_cast<int>(h), ch, rgb.data(), static_cast<int>(w * ch));
}

} // namespace xb::gpu
