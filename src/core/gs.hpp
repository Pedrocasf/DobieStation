#ifndef GS_HPP
#define GS_HPP
#include <cstdint>
#include "gscontext.hpp"

struct GIFtag
{
    uint16_t NLOOP;
    bool end_of_packet;
    bool output_PRIM;
    uint16_t PRIM;
    uint8_t format;
    uint8_t reg_count;
    uint64_t regs;

    uint8_t regs_left;
    uint32_t data_left;
};

struct PRIM_REG
{
    uint8_t prim_type;
    bool gourand_shading;
    bool texture_mapping;
    bool fog;
    bool alpha_blend;
    bool antialiasing;
    bool use_UV;
    bool use_context2;
    bool fix_fragment_value;
};

struct RGBAQ_REG
{
    uint8_t r, g, b, a;
    float q;
};

struct BITBLTBUF_REG
{
    uint32_t source_base;
    uint32_t source_width;
    uint8_t source_format;
    uint32_t dest_base;
    uint32_t dest_width;
    uint8_t dest_format;
};

struct TRXPOS_REG
{
    uint16_t source_x, source_y;
    uint16_t dest_x, dest_y;
    uint8_t trans_order;
};

//reg reg
struct TRXREG_REG
{
    uint16_t width;
    uint16_t height;
};

struct PMODE_REG
{
    bool circuit1;
    bool circuit2;
    uint8_t output_switching;
    bool use_ALP;
    bool out1_circuit2;
    bool blend_with_bg;
    uint8_t ALP;
};

struct SMODE
{
    bool interlaced;
    bool frame_mode;
    uint8_t power_mode;
};

struct DISPFB
{
    uint32_t frame_base;
    uint32_t width;
    uint8_t format;
    uint16_t x, y;
};

struct DISPLAY
{
    uint16_t x, y;
    uint8_t magnify_x, magnify_y;
    uint16_t width, height;
};

struct Vertex
{
    uint32_t coords[3];
    RGBAQ_REG rgbaq;
    UV_REG uv;
};

class GraphicsSynthesizer
{
    private:
        bool frame_complete;
        uint32_t* output_buffer;
        uint32_t* local_mem;
        uint8_t CRT_mode;

        GIFtag current_tag;
        bool processing_GIF_prim;

        GSContext context1, context2;
        GSContext* current_ctx;

        PRIM_REG PRIM;
        RGBAQ_REG RGBAQ;
        UV_REG UV;
        bool DTHE;
        bool COLCLAMP;
        bool use_PRIM;

        BITBLTBUF_REG BITBLTBUF;
        TRXPOS_REG TRXPOS;
        TRXREG_REG TRXREG;
        uint8_t TRXDIR;
        uint32_t transfer_addr;
        int transfer_bit_depth;
        int pixels_transferred;

        PMODE_REG PMODE;
        SMODE SMODE2;
        DISPFB DISPFB1, DISPFB2;
        DISPLAY DISPLAY1, DISPLAY2;

        Vertex current_vtx;
        Vertex vtx_queue[3];
        unsigned int num_vertices;

        static const unsigned int max_vertices[8];

        void vertex_kick(bool drawing_kick);
        void draw_pixel(uint32_t x, uint32_t y, uint32_t color, uint32_t z, bool alpha_blending);
        void render_primitive();
        void render_point();
        void render_line();
        void render_triangle();
        void render_sprite();
        void process_PACKED(uint64_t data[2]);
        void write_HWREG(uint64_t data);
        void host_to_host();
    public:
        GraphicsSynthesizer();
        ~GraphicsSynthesizer();
        void reset();
        void start_frame();
        bool is_frame_complete();
        uint32_t* get_framebuffer();
        void render_CRT();
        void get_resolution(int& w, int& h);

        void set_CRT(bool interlaced, int mode, bool frame_mode);

        uint32_t read32_privileged(uint32_t addr);
        uint64_t read64_privileged(uint32_t addr);
        void write32_privileged(uint32_t addr, uint32_t value);
        void write64_privileged(uint32_t addr, uint64_t value);
        void write64(uint32_t addr, uint64_t value);

        void feed_GIF(uint64_t data[2]);
        void send_PATH3(uint64_t data[2]);
};

#endif // GS_HPP
